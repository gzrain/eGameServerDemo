#include "BaseClient.h"

class CBillingClient : public CBaseClient
{
public:
	CBillingClient();
	virtual ~CBillingClient();

public:

	LRESULT BuyGoods(const COS_GUID*pUserGuid, const COS_GUID* pGoodsGuid, int nGoodsCnt);
	LRESULT SellGoods(const COS_GUID*pUserGuid, const COS_GUID* pGoodsGuid, int nGoodsCnt);
	LRESULT DropGoods(const COS_GUID*pUserGuid, const COS_GUID* pGoodsGuid, int nGoodsCnt);

};
