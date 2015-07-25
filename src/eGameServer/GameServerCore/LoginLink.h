#include "BaseLink.h"

#include "UserData.h"

#define ERR_LOGIN					-100
#define ERR_LOGIN_FAILED		(ERR_LOGIN - 1)
#define ERR_SIGNUP_FAILED	(ERR_LOGIN - 2)

class CLoginLink: public CBaseLink
{
public:
	CLoginLink();
	virtual ~CLoginLink();

protected:
	LRESULT ProcessRawData(const MSGHEAD* pstHead, const char* pDataBuf, int nDataLen);

	//具体业务请求处理
	LRESULT ProcLoginRequset(const char* pDataBuf, int nDatalen);
	LRESULT ProcLogoutRequset(const char* pDataBuf, int nDatalen);
	LRESULT ProcSignUpRequset(const char* pDataBuf, int nDatalen);

	LRESULT LoginVerify(LPCTSTR strAccount, LPCTSTR strPswd, LPUSER_ACCOUNT_T lpAccountInfoOut);
	LRESULT Singup(LPCTSTR strAccount, LPCTSTR strName, LPCTSTR strAlias, LPCTSTR strPswd);

};
