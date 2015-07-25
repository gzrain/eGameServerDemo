#include "GoodsData.h"
#include "SafeRelease.h"
#include "JumpUtil.h"
#include "AutoLock.h"

CGoodsMgr::CGoodsMgr()
{
 	InitializeCriticalSection(&m_csLock);
}

CGoodsMgr::~CGoodsMgr()
{
	DestroyArray();
	DeleteCriticalSection(&m_csLock);
}

void CGoodsMgr::DestroyArray()
{
	//加锁
	CAutoLock autoLock(&m_csLock);
	for (GOODS_ARRAY_ITER it = m_vecGoodsArray.begin(); it != m_vecGoodsArray.end(); it++)
	{
		SAFE_DELETE(*it);
	}
	m_vecGoodsArray.clear();
}

void CGoodsMgr::ResetMgr()
{
	DestroyArray();
}

LRESULT CGoodsMgr::AddGoods(const LPGOODS_COMMON_INFO_T pstGoodsInfo)
{
	LRESULT hr = S_OK;

	//add lock
	CAutoLock autoLock(&m_csLock);

	GOODS_ARRAY_ITER it = m_vecGoodsArray.begin();
	for (it; it != m_vecGoodsArray.end(); it++)
	{
		if ((*it)->guidGoods == pstGoodsInfo->guidGoods)
		{
			//商品已存在
			hr = E_FAIL;		
			break;
		}
	}

	CHECK_HR(hr);	//当商品已存在，即 hr != S_OK时，直接跳转至done结束；否则，顺序执行

	GOODS_COMMON_INFO_T* pstNew = new GOODS_COMMON_INFO_T;
	memcpy(pstNew, pstGoodsInfo, sizeof(GOODS_COMMON_INFO_T));
	m_vecGoodsArray.push_back(pstNew);

	//////////////////////////////////////////////////////////////////////////
	//note:
	//pstNew 将会在数组销毁或删除时，被相应释放
	//////////////////////////////////////////////////////////////////////////

done:
	return hr;
}

LRESULT CGoodsMgr::DelGoods(const COS_GUID *guid)
{
	LRESULT hr = E_FAIL;

	//add lock
	CAutoLock autoLock(&m_csLock);

	GOODS_ARRAY_ITER it = m_vecGoodsArray.begin();
	for (it; it != m_vecGoodsArray.end(); it++)
	{
		if ((*it)->guidGoods == *guid)
		{
			// 先释放内存资源
			SAFE_DELETE(*it);
			// 后释放列表资源
			m_vecGoodsArray.erase(it);
			hr = S_OK;
			break;
		}
	}

	return hr;
}

LRESULT CGoodsMgr::GetGoodsInfoByGuid(const COS_GUID *guid, GOODS_COMMON_INFO_T &stGoodsInfoOut)
{
	LRESULT hr = E_FAIL;

	//add lock
	CAutoLock autoLock(&m_csLock);

	GOODS_ARRAY_ITER it = m_vecGoodsArray.begin();
	for (it; it != m_vecGoodsArray.end(); it++)
	{
		if ((*it)->guidGoods == *guid)
		{
			memcpy(&stGoodsInfoOut, *it, sizeof(GOODS_COMMON_INFO_T));
			hr = S_OK;
			break;
		}
	}

	return hr;
}

int CGoodsMgr::GetGoodsCnt()
{
	return m_vecGoodsArray.size();
}

LRESULT CGoodsMgr::GetGoodsInfoByIdx(int nIdx, LPGOODS_COMMON_INFO_T pstGoodsInfoOut)
{
	LRESULT hr = S_OK;
	LPGOODS_COMMON_INFO_T lpGoodsInfo = NULL;

	lpGoodsInfo = m_vecGoodsArray.at(nIdx);
	if (NULL == lpGoodsInfo)
		hr = E_FAIL;
	CHECK_HR(hr);

	memcpy(pstGoodsInfoOut, lpGoodsInfo, sizeof(*lpGoodsInfo));

done:
	return hr;
}