#include <assert.h>
#include "LoginLink.h"
#include "LoginSrv.h"
#include <tchar.h>

CLoginLink::CLoginLink()
{
}

CLoginLink::~CLoginLink()
{
}

LRESULT CLoginLink::ProcessRawData(const MSGHEAD* pstHead, const char* pDataBuf, int nDataLen)
 {
 	LRESULT hr = S_OK;

	switch (pstHead->dwMsgType)
	{
	case CMD_USER_LOGIN_REQUEST:
		hr = ProcLoginRequset(pDataBuf, nDataLen);
		break;
	case CMD_USER_LOGOUT_REQUEST:
		hr = ProcLogoutRequset(pDataBuf, nDataLen);
		break;
	case CMD_USER_SIGNGUP_REQUEST:
		hr = ProcSignUpRequset(pDataBuf, nDataLen);
		break;
	default:
		break;
	}
 
	//////////////////////////////////////////////////////////////////////////
	//������������Ͽ�����
	//
	//

 	return hr;
 }

LRESULT CLoginLink::ProcLoginRequset(const char* pDataBuf, int nDatalen)
{
	LRESULT hr = S_OK;

	assert(pDataBuf);
	assert(sizeof(REQUEST_LOGIN_PKG_T) == nDatalen);

	REQUEST_LOGIN_PKG_T stPkg = { 0 };
	memcpy(&stPkg, pDataBuf, nDatalen);

	USER_ACCOUNT_T stAccountInfo;
	hr = LoginVerify(stPkg.szAccount, stPkg.szPswd, &stAccountInfo);

	//����˴�����ϣ����ݷ��ؽ�������ͻظ����ͻ���
	REPLY_LOGIN_PKG_T stReply = { 0 };
	if (FAILED(hr))
		stReply.nResult = -1;
	else
	{
		stReply.nResult = 0;
		stReply.guidUser = stAccountInfo.guidUser;
		stReply.tmLastLogin = stAccountInfo.tmLastLogin;

		_tcscpy_s(stReply.szUserAlias, sizeof(stReply.szUserAlias) / sizeof(stReply.szUserAlias[0]), stAccountInfo.szUserAlias);
		_tcscpy_s(stReply.szUserName, sizeof(stReply.szUserName) / sizeof(stReply.szUserName[0]), stAccountInfo.szUserName);

		// �ص����࣬ˢ�������б�
		CLoginSrv* pParent = (CLoginSrv*)m_pParent;
		pParent->AddToOnlineList(&stReply.guidUser);
	}

	// ���ͻ�Ӧ
	SendReply(CMD_USER_LOGIN_REPLY, &stReply, sizeof(stReply));

	return hr;
}

LRESULT CLoginLink::ProcLogoutRequset(const char* pDataBuf, int nDatalen)
{
	LRESULT hr = S_OK;

	assert(pDataBuf);
 	assert(sizeof(REQUEST_LOGOUT_PKG_T) == nDatalen);

	REQUEST_LOGOUT_PKG_T stPkg = { 0 };
	memcpy(&stPkg, pDataBuf, nDatalen);

	// �ص����࣬ˢ�������б�
	CLoginSrv* pParent = (CLoginSrv*)m_pParent;
	pParent->DeleteFromOnlineList(&stPkg.guidUser);

	//����˴�����ϣ����ݷ��ؽ�������ͻظ����ͻ���
	REPLY_LOGOUT_PKG_T stReply = { 0 };
	stReply.nResult = 0;

	// ���ͻ�Ӧ
	SendReply(CMD_USER_LOGOUT_REPLY, &stReply, sizeof(stReply));

	cout << "user offline" << endl;
	return hr;
}

LRESULT CLoginLink::ProcSignUpRequset(const char* pDataBuf, int nDatalen)
{
	LRESULT hr = S_OK;
	assert(pDataBuf);
// 	assert(sizeof(REQUEST_SIGNUP_PKG_T) == nDatalen);

	REQUEST_SIGNUP_PKG_T stPkg = { 0 };
	memcpy(&stPkg, pDataBuf, nDatalen);

	// ע�ᴦ��
	hr = Singup(stPkg.szAccount, stPkg.szName, stPkg.szAlias, stPkg.szPswd);

	//����˴�����ϣ����ݷ��ؽ�������ͻظ����ͻ���
	REPLY_SIGNUP_PKG_T stReply = { 0 };
	if (FAILED(hr))
		stReply.nResult = -1;
	else
		stReply.nResult = 0;

	// ���ͻ�Ӧ
	SendReply(CMD_USER_SIGNUP_REPLY, &stReply, sizeof(stReply));

	return hr;
}

LRESULT CLoginLink::LoginVerify(LPCTSTR strAccount, LPCTSTR strPswd, LPUSER_ACCOUNT_T lpAccountInfoOut)
{
	LRESULT hr = S_OK;

	hr = m_DBMgr.CheckAccount(strAccount, strPswd, lpAccountInfoOut);
	//////////////////////////////////////////////////////////////////////////
	//test
	// 	COS_GUID guid = GUID_NULL;
	// 	CrossPlatform_CreateGuid(&guid);
	// 	lpAccountInfoOut->guidUser = guid;
	// 	lpAccountInfoOut->tmLastLogin = 10000;
	// 
	// 	_stprintf_s(lpAccountInfoOut->szUserAlias, 256, _T("testAlias"));
	// 	_stprintf_s(lpAccountInfoOut->szUserName, 256, _T("testName"));
	//////////////////////////////////////////////////////////////////////////

	return hr;
}
LRESULT CLoginLink::Singup(LPCTSTR strAccount, LPCTSTR strName, LPCTSTR strAlias, LPCTSTR strPswd)
{
	LRESULT hr = S_OK;

	hr = m_DBMgr.UserSignup(strAccount, strPswd, strName, strAlias);

	cout << "new user sign up��" << strAlias << "return�� " << (int)hr << endl;

	return hr;
}
