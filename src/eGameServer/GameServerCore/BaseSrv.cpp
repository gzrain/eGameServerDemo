#include "BaseSrv.h"
#include "AutoLock.h"
#include <assert.h>
#include "JumpUtil.h"
#include "SafeRelease.h"
#include <tchar.h>

CBaseSrv::CBaseSrv()
{
	InitializeCriticalSection(&m_csLinkArray);
	m_unSocket = 0;
	m_hListenThread = 0;
	m_bListenThreadRun = FALSE;
	m_bWSAStartup = FALSE;
	m_hStart = NULL;
	m_hExit = NULL;
	m_hStop = NULL;
	m_pParent = NULL;
}

CBaseSrv::~CBaseSrv()
{
	UnInit();
	DeleteCriticalSection(&m_csLinkArray);
}

void CBaseSrv::Destroy()
{
	//销毁数组
	for (LINK_ITER it = m_vecLinkArray.begin(); it != m_vecLinkArray.end(); it++)
	{
		(*it)->UnInit();
		SAFE_DELETE(*it);
	}
	m_vecLinkArray.clear();
}

LRESULT CBaseSrv::Init(void* pParent, UINT unSerIP, UINT unSerPort)
{
	LRESULT hr = S_OK;
	assert(pParent);
	m_pParent = pParent;

	m_hStart = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hExit = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hStop = CreateEvent(NULL, TRUE, FALSE, NULL);

	hr = InitWinSocket(&m_unSocket, unSerIP, unSerPort);
	
	//start listen
	SetEvent(m_hStart);

	return hr;
}

LRESULT CBaseSrv::UnInit()
{
	if (m_bListenThreadRun)
	{
		OutputDebugString(_T("CBaseSrv::UnInit()-- exit listen thread\n"));

		m_bListenThreadRun = FALSE;
		SetEvent(m_hExit);
		if (WaitForSingleObject(m_hListenThread, 1000) != WAIT_OBJECT_0)
			TerminateThread(m_hListenThread, 0);
	}
	SAFE_CLOSE_HANDLE(m_hListenThread);
	SAFE_CLOSE_HANDLE(m_hStart);
	SAFE_CLOSE_HANDLE(m_hExit);
	SAFE_CLOSE_HANDLE(m_hStop);

	if (m_bWSAStartup)
	{
		m_bWSAStartup = FALSE;
		WSACleanup();
	}

	Destroy();
	return S_OK;
}

DWORD CBaseSrv::ListenThreadProc(void* argv)
{
	assert(argv);
	CBaseSrv *pSer = (CBaseSrv *)argv;
	return pSer->ListenThreadProcPrivate();
}

LRESULT CBaseSrv::InitWinSocket(UINT *punSocket, UINT unIP, UINT16 unSerPort)
{
	HRESULT hr = S_OK;
	struct sockaddr_in address = { 0 };
	BOOL bReuse = TRUE;
	struct linger li = { 0 };
	TCHAR szTempStr[256] = { 0 };
	DWORD dwThreadID = 0;
	int flags = 0;
	u_long ul = 1;

	WSADATA wsaData;
	hr = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (S_OK != hr)
		OutputDebugString(_T("socket startup failed.\n"));
	CHECK_HR(hr);
	m_bWSAStartup = TRUE;

	address.sin_family = AF_INET;
	if (unIP != 0)
		address.sin_addr.s_addr = unIP;//网络字节序IP
	else
		address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(unSerPort);

	*punSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == *punSocket)
	{
		OutputDebugString(_T("Create socket failed.\n"));
		hr = WSAGetLastError();
	}
	CHECK_HR(hr);

	//设置socket复用
	hr = setsockopt(*punSocket, SOL_SOCKET, SO_REUSEADDR, (const char *)&bReuse, sizeof(bReuse));
	if (S_OK != hr)
	{
		hr = WSAGetLastError();
		wsprintf(szTempStr, _T("set socket reuse failed. Err:%d\n"), hr);
		OutputDebugString(szTempStr);
	}
	CHECK_HR(hr);

	if (bind(*punSocket, (struct sockaddr *)&address, sizeof(address)) != 0)
	{
		OutputDebugString(_T("Bind socket failed.\n"));
		hr = WSAGetLastError();
	}
	CHECK_HR(hr);

	if (listen(*punSocket, SOMAXCONN) != 0)
	{
		wsprintf(szTempStr, _T("Listen socket failed on port: %d\n"), unSerPort);
		OutputDebugString(szTempStr);
		hr = WSAGetLastError();
	}
	CHECK_HR(hr);

	//设置为非阻塞模式
	//ioctlsocket(m_hListenSocket, FIONBIO, &ul); 
	m_bListenThreadRun = TRUE;

	m_hListenThread = ::CreateThread(NULL, 0, ListenThreadProc,
		(void *)this, 0, &dwThreadID	);

done:
	if (S_OK != hr)
	{
		SAFE_CLOSE_SOCKET(*punSocket);
		if (m_bWSAStartup)
			WSACleanup();
		m_bWSAStartup = FALSE;
	}
	return hr;
}

DWORD CBaseSrv::ListenThreadProcPrivate()
{
	HANDLE h1[2] = { m_hExit, m_hStart };
	HANDLE h2[2] = { m_hExit, m_hStop };
	DWORD dwResult1 = 0;
	DWORD dwResult2 = 0;

	for (;;)
	{
		dwResult1 = ::WaitForMultipleObjects(2, h1, FALSE, INFINITE);
		if (dwResult1 == WAIT_OBJECT_0)
		{
			ResetEvent(m_hExit);
			m_bListenThreadRun = FALSE;
			return 0;
		}
		else if (dwResult1 == WAIT_OBJECT_0 + 1)
		{
			ResetEvent(m_hStart);

			struct 	sockaddr_in	client_address = { 0 };//sockaddr
			int  nAddrLen;
			UINT unAccSocket = INVALID_SOCKET;

			OutputDebugString(_T("ListenThreadProc线程开始运行\n"));

			for (;;)
			{
				dwResult2 = ::WaitForMultipleObjects(2, h2, FALSE, 100);
				if (dwResult2 == WAIT_OBJECT_0)
				{
					ResetEvent(m_hExit);
					m_bListenThreadRun = FALSE;
					return 0;
				}
				else if (dwResult2 == WAIT_OBJECT_0 + 1)
				{
					ResetEvent(m_hStop);
					m_bListenThreadRun = FALSE;
					break;
				}

				nAddrLen = sizeof(client_address);
				unAccSocket = accept(m_unSocket, (struct sockaddr *)&client_address, &nAddrLen);
				if (INVALID_SOCKET == unAccSocket)
				{
					TCHAR strTemp[256] = { 0 };
					wsprintf(strTemp, _T("Accept failed.%d\n"), WSAGetLastError());
					OutputDebugString(strTemp);
					break;
				}

				
				LinkMgrProc(unAccSocket);

			}
		}
	}

	return 0;
}

DWORD ErrProc_CB(void* pParent, void* argv, LRESULT lErrCode)
{


	//此处必须 Post出去，做异步处理
	//必须由外部释放link资源
	//
	//暂未做20150707

	return 0;
}