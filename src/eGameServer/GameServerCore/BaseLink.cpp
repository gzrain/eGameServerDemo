
#include "BaseLink.h"


#pragma comment(lib,"ws2_32.lib")

CBaseLink::CBaseLink()
{
	m_hRecvThread = 0;
	m_bRecvThreadRun = FALSE;

	m_pParent = NULL;
	m_pErrProc = NULL;

	m_hSocket = 0;
	m_hRecvStart = 0;
	m_hRecvStop = 0;
	m_hRecvExit = 0;
}

CBaseLink::~CBaseLink()
{
	UnInit();
}

LRESULT CBaseLink::Init(void *pParent, UINT hSocket,
	DWORD(*pErrProc)(void* pParent, void* argv, LRESULT lErrCode))
{
	assert(HANDLE_IS_VALID(hSocket));
	m_hSocket = hSocket;
	m_pParent = pParent;
	m_pErrProc = pErrProc;

	m_hRecvStart = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hRecvStop = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hRecvExit = CreateEvent(NULL, TRUE, FALSE, NULL);

	DWORD dwThreadID = 0;
	m_bRecvThreadRun = TRUE;
	m_hRecvThread = ::CreateThread(NULL, 0, BaseLink_RecvThreadProc,
		(void *)this, 0, &dwThreadID);
	
	m_DBMgr.Init();
	return S_OK;
}

LRESULT CBaseLink::UnInit()
{
	SAFE_CLOSE_SOCKET(m_hSocket);
	if (m_bRecvThreadRun)
	{
		m_bRecvThreadRun = FALSE;
		SetEvent(m_hRecvExit);
		if (WaitForSingleObject(m_hRecvThread, 1000) != WAIT_OBJECT_0)
			TerminateThread(m_hRecvThread, 0);
	}
	SAFE_CLOSE_HANDLE(m_hRecvThread);

	SAFE_CLOSE_HANDLE(m_hRecvStart);
	SAFE_CLOSE_HANDLE(m_hRecvStop);
	SAFE_CLOSE_HANDLE(m_hRecvExit);

	m_DBMgr.UnInit();
	return S_OK;
}

BOOL CBaseLink::StartRecv()
{
	return SetEvent(m_hRecvStart);
}

BOOL CBaseLink::StopRecv()
{
	return SetEvent(m_hRecvStop);
}

DWORD CBaseLink::BaseLink_RecvThreadProc(void* argv)
{
	assert(argv);
	CBaseLink *pLink = (CBaseLink *)argv;

	HANDLE h1[2] = { pLink->m_hRecvExit, pLink->m_hRecvStart };
	HANDLE h2[2] = { pLink->m_hRecvExit, pLink->m_hRecvStop };
	DWORD dwResult1 = 0;

	for (;;)
	{
		dwResult1 = ::WaitForMultipleObjects(2, h1, FALSE, INFINITE);
		if (dwResult1 == WAIT_OBJECT_0)
		{
			ResetEvent(pLink->m_hRecvExit);
			pLink->m_bRecvThreadRun = FALSE;

			return 0;
		}
		else if (dwResult1 == WAIT_OBJECT_0 + 1)
		{
			ResetEvent(pLink->m_hRecvStart);

			DWORD dwResult2 = 0;
			for (;;)
			{
				dwResult2 = ::WaitForMultipleObjects(2, h2, FALSE, 100);
				if (dwResult2 == WAIT_OBJECT_0)
				{
					ResetEvent(pLink->m_hRecvExit);
					pLink->m_bRecvThreadRun = FALSE;
					return 0;
				}
				else if (dwResult2 == WAIT_OBJECT_0 + 1)
				{
					ResetEvent(pLink->m_hRecvStop);
					pLink->m_bRecvThreadRun = FALSE;
					break;
				}

				// 接收原始数据，保存到内存中，并做相应的业务处理

				char pRecvBuf[MAX_RECV_BUF_LEN] = { 0 };
				int nRecvBufLen = 0;

				if (S_OK != pLink->RecvRawPkg(pRecvBuf, &nRecvBufLen))
				{
					continue;
				}

				MSGHEAD stMsgHead = { 0 };
				int nOffset = 0;
				int nDataLen = nRecvBufLen - sizeof(MSGHEAD);

				char* pDataBuf = (char*)malloc(nDataLen);
				memset(pDataBuf, 0, nDataLen);

				nOffset = MemOP_Read(pRecvBuf, &stMsgHead, sizeof(stMsgHead), nOffset);
				nOffset = MemOP_Read(pRecvBuf, pDataBuf, nDataLen, nOffset);

				if (S_OK != pLink->ProcessRawData(&stMsgHead, pDataBuf, nDataLen))
				{
					SAFE_FREE(pDataBuf);
					continue;
				}

				SAFE_FREE(pDataBuf);
			}
		}
	}

	pLink->m_bRecvThreadRun = FALSE;
	return 0;
}

LRESULT CBaseLink::RecvRawPkg(char* pRecvBuf, int* pnRecvLen)
{
	LRESULT hr = S_OK;
	int nRecvLen = 0;
	u_long ulLen = 0;

	// 首先获取获取数据长度
	GetSockRecvDataLen(m_hSocket, &ulLen);

	// 如果没有数据，直接返回
	if (ulLen <= 0)
	{
		hr = S_FALSE;
	}
	CHECK_SOK(hr);

	nRecvLen = min(ulLen, MAX_RECV_BUF_LEN);

	if (!SockRecv((char *)pRecvBuf, nRecvLen))
	{
		hr = -1;
	}
	CHECK_SOK(hr);

	*pnRecvLen = nRecvLen;

done:
	if (FAILED(hr))
		m_pErrProc(m_pParent, this, ERR_SOCK_RECV_FAILED);

	return hr;
}

int CBaseLink::GetSockRecvDataLen(int sock, u_long *argp)
{
	return ioctlsocket(sock, FIONREAD, argp);
}

BOOL CBaseLink::SockRecv(char *pRecvData, long lDataLen)
{
	LONG lLeft = lDataLen;
	int nPos = 0;
	int nRet = 0;
	int nRecvCount = 0;
	int nErr = 0;
	assert(pRecvData);
	assert(lDataLen >= 0);
	memset(pRecvData, 0, lLeft);

	while (lLeft)
	{
		nRet = recv(m_hSocket, pRecvData + nPos, lLeft, 0);
		nErr = WSAGetLastError();
		if (nRet < 0)
			return FALSE;

		if (!nRet)
		{
			if (nErr != 0)
				return FALSE;

			nRecvCount++;
			if (nRecvCount > 100)
				return FALSE;
		}
		nPos += nRet;
		lLeft -= nRet;
	}

	return TRUE;
}

BOOL CBaseLink::SockSend(const char *pSendData, long lDataLen)
{
	BOOL bRt = TRUE;
	BOOL bStartup = FALSE;
	long lLeft = lDataLen;
	int nPos = 0;
	int nRet = 0;
	int nErr = 0;
	assert(pSendData);
	assert(lDataLen >= 0);

	WSADATA wsaData;
	nErr = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (0 == nErr)
		bStartup = TRUE;


	while (lLeft)
	{
		nRet = send(m_hSocket, (pSendData + nPos), lLeft, 0);

		if (nRet < 0)
		{
			int nErr = WSAGetLastError();

			TCHAR szLogStr[128] = { 0 };
			_stprintf_s(szLogStr, sizeof(szLogStr) / sizeof(szLogStr[0]), _T("Link:send err:%d\n socket:%d\n"), nErr, m_hSocket);
			OutputDebugString(szLogStr);

			m_pErrProc(m_pParent, this, ERR_SOCK_SEND_FAILED);
			bRt = FALSE;
			break;
		}
		if (!nRet)
			assert(0);

		nPos += nRet;
		lLeft -= nRet;
	}

	if (bStartup)
		WSACleanup();
	return bRt;
}

LRESULT CBaseLink::SendReply(DWORD dwMsgType,const void* pDataBuf, int nDataLen)
{
	assert(pDataBuf);

	LRESULT hr = S_OK;
	int nSendLen = 0;
	int nOffset = 0;
	nSendLen = sizeof(MSGHEAD)+nDataLen;

	// 首先申请一段内存空间
	char* pSendBuf = NULL;
	pSendBuf = (char*)malloc(nSendLen);

	nOffset = Protocol_SetMsgHead(pSendBuf, dwMsgType, nDataLen);
	nOffset = MemOP_Write(pSendBuf, (void*)pDataBuf, nDataLen, nOffset);

	if (!SockSend(pSendBuf, nSendLen))
		hr = ERR_SOCK_SEND_FAILED;

 	SAFE_FREE(pSendBuf);
	return hr;
}