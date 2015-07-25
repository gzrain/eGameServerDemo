// UserData.h
// by gaozhe
// 20150702
#ifndef _GOODSDATA_H_
#define _GOODSDATA_H_

#ifdef GAMESERVERCORE_EXPORTS
#define GOODSMGR_API __declspec(dllexport)
#else
#define GOODSMGR_API __declspec(dllimport)
#endif
#pragma warning( disable: 4251 )

#include "Protocol.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////
//由于是Demo，写死几个商品信息，具体信息在Billing数据库中体现


//////////////////////////////////////////////////////////////////////////
//商品信息
typedef struct _goods_common_info_t
{
	COS_GUID	guidGoods;		//商品唯一标识码
	TCHAR szGoodsName[MAX_GOODS_NAME_LEN];
	TCHAR szGoodsDesc[MAX_GOODS_DESC_LEN];
	unsigned short unGoodsCnt;
	float		fGoodsPrice;
}GOODS_COMMON_INFO_T, *LPGOODS_COMMON_INFO_T;

//商品信息列表数据结构
typedef vector<LPGOODS_COMMON_INFO_T> GOODS_ARRAY;
typedef GOODS_ARRAY::iterator GOODS_ARRAY_ITER;

class GOODSMGR_API CGoodsMgr
{
public:
	CGoodsMgr();
	virtual~CGoodsMgr();
public:
	
	LRESULT AddGoods(const LPGOODS_COMMON_INFO_T pstGoodsInfo);
	LRESULT DelGoods(const COS_GUID *guid);
	LRESULT GetGoodsInfoByGuid(const COS_GUID *guid, GOODS_COMMON_INFO_T &stGoodsInfoOut);

	int GetGoodsCnt();
	LRESULT GetGoodsInfoByIdx(int nIdx, LPGOODS_COMMON_INFO_T pstGoodsInfoOut);

	void ResetMgr();
protected:
	void DestroyArray();		//用于释放资源

private:
	GOODS_ARRAY m_vecGoodsArray;
	CRITICAL_SECTION m_csLock;			//用于多线程访问时，对商品列表加锁
};
#endif