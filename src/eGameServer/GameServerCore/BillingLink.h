#include "BaseLink.h"

#include "UserData.h"

#define ERR_BILLING			-300

class CBillingLink : public CBaseLink
{
public:
	CBillingLink();
	virtual ~CBillingLink();

protected:
	LRESULT ProcessRawData(const MSGHEAD* pstHead, const char* pDataBuf, int nDataLen);

private:
	//具体业务请求处理
	LRESULT ProcSellGoods(const char* pDataBuf, int nDatalen);
	LRESULT ProcBuyGoods(const char* pDataBuf, int nDatalen);
	LRESULT ProcDropGoods(const char* pDataBuf, int nDatalen);


	LRESULT ProcessGoods(const COS_GUID* pGuidUser, const COS_GUID * pGuidGoods, int nGoodsCnt, int nProcessType);
};
