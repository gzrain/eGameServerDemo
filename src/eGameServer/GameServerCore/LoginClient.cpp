
#include "LoginClient.h"


CLoginClient::CLoginClient()
{
}

CLoginClient::~CLoginClient()
{

}

LRESULT CLoginClient::LogOut(const COS_GUID *pUserGuid)
{
	assert(pUserGuid);

	LRESULT hr = S_OK;
	REQUEST_LOGOUT_PKG_T stPkg = { 0 };
	REPLY_LOGOUT_PKG_T stPkgReply = { 0 };
	MSGHEAD stReplyHead = { 0 };

	int nReplyBufLen = 0;
	char* pReplyBuf = NULL;
	int nOffset = 0;

	//填充数据
	stPkg.guidUser = *pUserGuid;
	hr = SendCmd(CMD_USER_LOGOUT_REQUEST, (char*)&stPkg, sizeof(stPkg));
	CHECK_SOK(hr);

	// wait for server reply
	nReplyBufLen = sizeof(MSGHEAD)+sizeof(REPLY_LOGOUT_PKG_T);
	pReplyBuf = (char*)malloc(nReplyBufLen);

	if (!SockRecv(pReplyBuf, nReplyBufLen))
	{
		hr = ERR_CLIENT_SER_REPLY_NOREPLY;
	}
	CHECK_SOK(hr);

	nOffset = MemOP_Read(pReplyBuf, &stReplyHead, sizeof(stReplyHead), nOffset);
	nOffset = MemOP_Read(pReplyBuf, &stPkgReply, sizeof(stPkgReply), nOffset);

	if (stReplyHead.dwMsgType != CMD_USER_LOGOUT_REPLY ||
		stReplyHead.dwDataLen != sizeof(stPkgReply))
	{
		hr = ERR_CLIENT_SER_REPLY_NOREPLY;
	}
	CHECK_SOK(hr);

	if (stPkgReply.nResult != 0)
		hr = ERR_CLIENT_SER_REPLY_FALSE;
	else
		hr = ERR_CLIENT_SER_REPLY_TRUE;
	CHECK_SOK(hr);

done:
	SAFE_FREE(pReplyBuf);
	return hr;

}

LRESULT CLoginClient::Login(LPCTSTR lpstrAccount, LPCTSTR lpstrPswd, LPUSER_ACCOUNT_T lpUserAccountInfo)
{
	assert(lpstrAccount);
	assert(lpstrPswd);
	assert(lpUserAccountInfo);

	LRESULT hr = S_OK;
	REQUEST_LOGIN_PKG_T stPkg = { 0 };
	REPLY_LOGIN_PKG_T stPkgReply = { 0 };
	MSGHEAD stReplyHead = { 0 };

	int nReplyBufLen = 0;
	char* pReplyBuf = NULL;
	int nOffset = 0;

	_stprintf_s(stPkg.szAccount, MAX_ACCOUNT_LEN, _T("%s"), lpstrAccount);
	_stprintf_s(stPkg.szPswd, MAX_PASSWORD_LEN, _T("%s"), lpstrPswd);

	hr = SendCmd(CMD_USER_LOGIN_REQUEST, (char*)&stPkg, sizeof(stPkg));
	CHECK_SOK(hr);

	// wait for server reply
	nReplyBufLen = sizeof(MSGHEAD)+sizeof(REPLY_LOGIN_PKG_T);
	pReplyBuf = (char*)malloc(nReplyBufLen);

	if (!SockRecv(pReplyBuf, nReplyBufLen))
	{
		hr = ERR_CLIENT_SER_REPLY_NOREPLY;
	}
	CHECK_SOK(hr);

	nOffset = MemOP_Read(pReplyBuf, &stReplyHead, sizeof(stReplyHead), nOffset);
	nOffset = MemOP_Read(pReplyBuf, &stPkgReply, sizeof(stPkgReply), nOffset);

	if (stReplyHead.dwMsgType != CMD_USER_LOGIN_REPLY ||
		stReplyHead.dwDataLen != sizeof(stPkgReply))
	{
		hr = ERR_CLIENT_SER_REPLY_NOREPLY;
	}
	CHECK_SOK(hr);

	if (stPkgReply.nResult != 0)
		hr = ERR_CLIENT_SER_REPLY_FALSE;
	else
		hr = ERR_CLIENT_SER_REPLY_TRUE;
	CHECK_SOK(hr);

	// 最终登录成功，才会返回用户数据
	lpUserAccountInfo->guidUser = stPkgReply.guidUser;
	memcpy(lpUserAccountInfo->szUserAlias, stPkgReply.szUserAlias, sizeof(stPkgReply.szUserAlias));
	memcpy(lpUserAccountInfo->szUserName, stPkgReply.szUserName, sizeof(stPkgReply.szUserName));
	lpUserAccountInfo->tmLastLogin = stPkgReply.tmLastLogin;

done:
	SAFE_FREE(pReplyBuf);
	return hr;

}

LRESULT CLoginClient::SignUp(LPCTSTR lpstrAccount, LPCTSTR lpstrPswd, LPCTSTR lpstrName, LPCTSTR lpstrAlias)
{
	assert(lpstrAccount);
	assert(lpstrPswd);
	assert(lpstrAlias);
	assert(lpstrName);

	LRESULT hr = S_OK;
	REQUEST_SIGNUP_PKG_T stPkg = { 0 };
	REPLY_SIGNUP_PKG_T stPkgReply = { 0 };
	MSGHEAD stReplyHead = { 0 };

	int nReplyBufLen = 0;
	char* pReplyBuf = NULL;
	int nOffset = 0;

	_stprintf_s(stPkg.szAccount, MAX_ACCOUNT_LEN, _T("%s"), lpstrAccount);
	_stprintf_s(stPkg.szPswd, MAX_PASSWORD_LEN, _T("%s"), lpstrPswd);
	_tcscpy_s(stPkg.szName, MAX_USER_NAME_LEN, lpstrName);
	_tcscpy_s(stPkg.szAlias, MAX_USER_ALIAS_LEN, lpstrAlias);

	hr = SendCmd(CMD_USER_SIGNGUP_REQUEST, (char*)&stPkg, sizeof(stPkg));
	CHECK_SOK(hr);

	// wait for server reply
	nReplyBufLen = sizeof(MSGHEAD)+sizeof(REPLY_SIGNUP_PKG_T);
	pReplyBuf = (char*)malloc(nReplyBufLen);

	if (!SockRecv(pReplyBuf, nReplyBufLen))
	{
		hr = ERR_CLIENT_SER_REPLY_NOREPLY;
	}
	CHECK_SOK(hr);

	nOffset = MemOP_Read(pReplyBuf, &stReplyHead, sizeof(stReplyHead), nOffset);
	nOffset = MemOP_Read(pReplyBuf, &stPkgReply, sizeof(stPkgReply), nOffset);

	if (stReplyHead.dwMsgType != CMD_USER_SIGNUP_REPLY ||
		stReplyHead.dwDataLen != sizeof(stPkgReply))
	{
		hr = ERR_CLIENT_SER_REPLY_NOREPLY;
	}
	CHECK_SOK(hr);

	if (stPkgReply.nResult != 0)
		hr = ERR_CLIENT_SER_REPLY_FALSE;
	else
		hr = ERR_CLIENT_SER_REPLY_TRUE;
	CHECK_SOK(hr);

done:
	SAFE_FREE(pReplyBuf);
	return hr;

}