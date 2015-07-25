#include "BaseLink.h"

#include "UserData.h"

#define ERR_LOGIC				-100
#define ERR_LOGIN_FAILED		(ERR_LOGIC - 1)
#define ERR_SIGNUP_FAILED	(ERR_LOGIC - 2)

class CLogicLink : public CBaseLink
{
public:
	CLogicLink();
	virtual ~CLogicLink();

protected:
	LRESULT ProcessRawData(const MSGHEAD* pstHead, const char* pDataBuf, int nDataLen);

private:
	//具体业务请求处理
	LRESULT ProcUpdateUserCoord(const char* pDataBuf, int nDatalen);
	LRESULT ProcUpdateUserBag(const char* pDataBuf, int nDatalen);
	LRESULT ProcUpdateShop(const char* pDataBuf, int nDatalen);

	LRESULT ProcUpdateUserProperty(const char* pDataBuf, int nDatalen);


	LRESULT GetCoordinate(const COS_GUID *pUserGuid, LPUSER_COORDINATE_INFO_T lpUserCoordInfo);
	LRESULT SetCoordinate(const COS_GUID *pUserGuid, LPUSER_COORDINATE_INFO_T lpUserCoordInfo);

};
