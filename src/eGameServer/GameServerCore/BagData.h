// UserData.h
// by gaozhe
// 20150702

#ifndef _BAGDATA_H_
#define _BAGDATA_H_

#ifdef GAMESERVERCORE_EXPORTS
#define BAGMGR_API __declspec(dllexport)
#else
#define BAGMGR_API __declspec(dllimport)
#endif

#include "GoodsData.h"

// 玩家背包类型： 小、中、大、超大
#define	INT_CAPACITY_SMALL_BAG			10
#define	INT_CAPACITY_NORMAL_BAG		20
#define	INT_CAPACITY_LARGE_BAG			50
#define	INT_CAPACITY_XLARGE_BAG		100

typedef enum _user_bag_type_e
{
	E_BAG_TYPE_START = 0,
	E_BAG_TYPE_SMALL,
	E_BAG_TYPE_NORMAL,
	E_BAG_TYPE_LARGE,
	E_BAG_TYPE_XLARGE,
	E_BAG_TYPE_END
}USER_BAG_TYPE_E;

typedef struct _user_bag_content_t
{
	unsigned int nBagType;
	unsigned int nBagCapacity;
	CGoodsMgr* pGoodsMgr;

}USER_BAG_CONTENT_T;


class BAGMGR_API  CBagMgr
{
public:
	CBagMgr();
	~CBagMgr();

public:
	LRESULT SetBagProperty(UINT nBagType);
	LRESULT GetBagProperty(UINT &nBagType, UINT &nBagCapacity);

	LRESULT AddShopGoods(const LPGOODS_COMMON_INFO_T pstGoodsInfo);

	LRESULT DelUserGoods(const COS_GUID* guid);

	LRESULT GetGoodsInfo(const COS_GUID* guid, GOODS_COMMON_INFO_T& stGoodsInfoOut);

protected:
	void DestroyBag();
private:
	USER_BAG_CONTENT_T m_stUserBag;

};

#endif
