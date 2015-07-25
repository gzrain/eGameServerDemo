#ifndef _LOGIN_CLIENT_H_
#define _LOGIN_CLIENT_H_

#include "BaseClient.h"

class CLoginClient : public CBaseClient
{
public:
	CLoginClient();
	virtual~CLoginClient();

public:
	LRESULT Login(LPCTSTR lpstrAccount, LPCTSTR lpstrPswd, LPUSER_ACCOUNT_T lpUserAccountInfo);
	LRESULT LogOut(const COS_GUID *pUserGuid);
	LRESULT SignUp(LPCTSTR lpstrAccount, LPCTSTR lpstrPswd, LPCTSTR lpstrName, LPCTSTR lpstrAlias);

};



#endif
