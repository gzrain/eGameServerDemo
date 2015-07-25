#include "BaseClient.h"
#include "SafeRelease.h"
#include "JumpUtil.h"
#include "MemOP.h"
#include <tchar.h>
#include <assert.h>
#include <time.h>

CBaseClient::CBaseClient()
{
	m_bWSAStartup = FALSE;
	m_hSocket = INVALID_SOCKET;
	m_bServerConnected = FALSE;
}

CBaseClient::~CBaseClient()
{
	UnInit();
}

LRESULT CBaseClient::UnInit()
{
	SAFE_CLOSE_SOCKET(m_hSocket);

	if (m_bWSAStartup)
	{
		m_bWSAStartup = FALSE;
		WSACleanup();
	}

	m_bServerConnected = FALSE;
	return S_OK;
}

LRESULT CBaseClient::Init(LPCSTR pszSerIPStr, UINT unSerPort, UINT unStaticLocalPort)
{
	assert(pszSerIPStr);

	LRESULT hr = S_OK;

	strcpy_s(m_szSerIPStr, sizeof(m_szSerIPStr) / sizeof(m_szSerIPStr[0]), pszSerIPStr);
	m_unSerPort = unSerPort;
	m_unStaticLocalPort = unStaticLocalPort;

	return hr;
}

LRESULT CBaseClient::ConnectToServer()
{
	LRESULT hr = E_FAIL;
	if (m_bServerConnected)
		return S_OK;

	while (hr == E_FAIL)
	{
		hr = InitWinSocket(m_szSerIPStr, m_unSerPort, m_unStaticLocalPort);
	}
	CHECK_HR(hr);
	m_bServerConnected = TRUE;

done:

	return hr;
}

LRESULT CBaseClient::InitWinSocket(LPCSTR pszSerIPStr, UINT unSerPort, UINT unStaticLocalPort)
{
	HRESULT hr = S_OK;
	struct sockaddr_in address = { 0 };
	BOOL bReuse = TRUE;
	TCHAR szTempStr[256] = { 0 };
	UINT unAcceptSocket = INVALID_SOCKET;

	WSADATA wsaData;
	hr = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (S_OK != hr)
		OutputDebugString(_T("socket startup failed.\n"));
	CHECK_SOK(hr);
	m_bWSAStartup = TRUE;

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(pszSerIPStr);
	address.sin_port = htons(unSerPort);

	unAcceptSocket = INVALID_SOCKET;
	unAcceptSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == unAcceptSocket)
	{
		OutputDebugString(_T("Create socket failed.\n"));
		hr = WSAGetLastError();
	}
	CHECK_SOK(hr);

	//ÉèÖÃsocket¸´ÓÃ
	hr = setsockopt(unAcceptSocket, SOL_SOCKET, SO_REUSEADDR, (const char *)&bReuse, sizeof(bReuse));
	if (S_OK != hr)
	{
		hr = WSAGetLastError();
		_stprintf_s(szTempStr, sizeof(szTempStr) / sizeof(szTempStr[0]), _T("set socket reuse failed. Err:%d\n"), hr);
		OutputDebugString(szTempStr);
	}
	CHECK_SOK(hr);

	if (unStaticLocalPort > 0)
	{
		struct sockaddr_in addressBind = { 0 };
		addressBind.sin_family = AF_INET;
		addressBind.sin_addr.s_addr = htonl(INADDR_ANY);
		addressBind.sin_port = htons(unStaticLocalPort);

		if (bind(unAcceptSocket, (struct sockaddr *)&addressBind, sizeof(addressBind)) != 0)
		{
			hr = WSAGetLastError();
			_stprintf_s(szTempStr, sizeof(szTempStr) / sizeof(szTempStr[0]), _T("Bind socket failed. Err:%d\n"), hr);
			OutputDebugString(szTempStr);
		}
	}
	CHECK_SOK(hr);

	if (connect(unAcceptSocket, (struct sockaddr*)&address, sizeof(address)) != 0)
	{
		hr = WSAGetLastError();
		_stprintf_s(szTempStr, sizeof(szTempStr) / sizeof(szTempStr[0]), _T("socket connect failed IP:%s Port:%d. Err:%d\n"), pszSerIPStr, unSerPort, hr);
		OutputDebugString(szTempStr);
	}
	CHECK_SOK(hr);
	m_hSocket = unAcceptSocket;

done:
	if (S_OK != hr)
	{
		SAFE_CLOSE_SOCKET(unAcceptSocket);
		if (m_bWSAStartup)
			WSACleanup();
		m_bWSAStartup = FALSE;

		m_bServerConnected = FALSE;
	}
	return hr;
}

BOOL CBaseClient::SockRecv(char *pRecvData, long lDataLen)
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
		//if (nRet == SOCKET_ERROR)
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

BOOL CBaseClient::SockSend(const char *pSendData, long lDataLen)
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

LRESULT CBaseClient::SendCmd(DWORD dwCmdType, const char* pDataBuf, long lDataLen)
{
	LRESULT hr = E_FAIL;
	char* pSendBuf = NULL;
	int nOffset = 0;
	int nSendLength = sizeof(MSGHEAD)+lDataLen;

	hr = ConnectToServer();
	CHECK_SOK(hr);

	pSendBuf = (char*)malloc(nSendLength);
	nOffset = Protocol_SetMsgHead(pSendBuf, dwCmdType, lDataLen);
	nOffset = MemOP_Write(pSendBuf, (void*)pDataBuf, lDataLen, nOffset);

	assert(nOffset == nSendLength);
	if (SockSend(pSendBuf, nSendLength))
		hr = S_OK;

done:
	SAFE_FREE(pSendBuf);
	return hr;
}

LRESULT CBaseClient::GetTimeString(const time_t *ptmSrc, LPSTR lpstrReturn, DWORD dwSizeInWords)
{
	tm  tmSrc = { 0 };
	localtime_s(&tmSrc, ptmSrc);
	memset(lpstrReturn, 0, dwSizeInWords*sizeof(char));

	sprintf_s(lpstrReturn, dwSizeInWords, "%d-%02d-%02d %02d:%02d:%02d", tmSrc.tm_year + 1900, tmSrc.tm_mon + 1, tmSrc.tm_mday
		, tmSrc.tm_hour, tmSrc.tm_min, tmSrc.tm_sec);
	return S_OK;
}