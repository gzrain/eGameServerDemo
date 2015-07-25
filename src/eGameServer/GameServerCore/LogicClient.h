#include "BaseClient.h"

#include "GoodsData.h"

class CLogicClient: public CBaseClient
{
public:
	CLogicClient();
	virtual ~CLogicClient();

public:
	LRESULT UnInit();

	LRESULT GetUserCoordInfo(const COS_GUID* pUserGuid, LPUSER_COORDINATE_INFO_T lpstUserCoordInfo);
	LRESULT SetUserCoordInfo(const COS_GUID* pUserGuid, LPUSER_COORDINATE_INFO_T lpstUserCoordInfo);

	LRESULT GetUserProperty(const COS_GUID* pUserGuid, LPUSER_GAME_PROPERTY_T lpstUserProperty);
	LRESULT SetUserProperty(const COS_GUID* pUserGuid, LPUSER_GAME_PROPERTY_T lpstUserProperty);

	int GetUserBagGoodsCnt(const COS_GUID* pUserGuid);
	LRESULT GetUserBagGoodsInfoByIdx(int nGoodsIdx, LPGOODS_COMMON_INFO_T lpstGoodsInfoOut);

	int GetShopGoodsCnt();
	LRESULT GetShopGoodsInfoByIdx(int nGoodsIdx, LPGOODS_COMMON_INFO_T lpstGoodsInfoOut);

private:
	CGoodsMgr m_UserBagMgr;
	CGoodsMgr m_ShopMgr;
};
