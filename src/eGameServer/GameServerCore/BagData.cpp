#include "BagData.h"
#include "JumpUtil.h"
#include "SafeRelease.h"

CBagMgr::CBagMgr()
{
	memset(&m_stUserBag, 0, sizeof(m_stUserBag));
}

CBagMgr::~CBagMgr()
{
	DestroyBag();
}

LRESULT CBagMgr::SetBagProperty(UINT nBagType)
{
	LRESULT hr = S_OK;
	if (nBagType <= E_BAG_TYPE_START ||
		nBagType >= E_BAG_TYPE_END)
	{
		hr = E_FAIL;
	}
	CHECK_HR(hr);

	// set bag type
	m_stUserBag.nBagType = nBagType;

	// set bag capacity by bag type
	switch (nBagType)
	{
	case E_BAG_TYPE_NORMAL:
		m_stUserBag.nBagCapacity = INT_CAPACITY_NORMAL_BAG;
		break;
	case E_BAG_TYPE_LARGE:
		m_stUserBag.nBagCapacity = INT_CAPACITY_LARGE_BAG;
		break;
	case E_BAG_TYPE_XLARGE:
		m_stUserBag.nBagCapacity = INT_CAPACITY_XLARGE_BAG;
		break;
	default:
		m_stUserBag.nBagCapacity = INT_CAPACITY_SMALL_BAG;
		break;
	}

done:
	return hr;
}

LRESULT CBagMgr::GetBagProperty(UINT &nBagType, UINT &nBagCapacity)
{
	LRESULT hr = S_OK;
	if (m_stUserBag.nBagType <= E_BAG_TYPE_START ||
		m_stUserBag.nBagType >= E_BAG_TYPE_LARGE)
	{
		hr = E_FAIL;
	}
	CHECK_HR(hr);
	nBagType = m_stUserBag.nBagType;
	nBagCapacity = m_stUserBag.nBagCapacity;

done:
	return hr;
}

void CBagMgr::DestroyBag()
{
	if (m_stUserBag.pGoodsMgr)
	{
		SAFE_DELETE(m_stUserBag.pGoodsMgr);
	}
}

LRESULT CBagMgr::AddShopGoods(const LPGOODS_COMMON_INFO_T pstGoodsInfo)
{
	if (!m_stUserBag.pGoodsMgr)
		m_stUserBag.pGoodsMgr = new CGoodsMgr();

	return m_stUserBag.pGoodsMgr->AddGoods(pstGoodsInfo);
}

LRESULT CBagMgr::DelUserGoods(const COS_GUID* guid)
{
	LRESULT hr = S_OK;
	if (!m_stUserBag.pGoodsMgr)
		hr = E_FAIL;
	CHECK_HR(hr);

	hr = m_stUserBag.pGoodsMgr->DelGoods(guid);
done:
	return hr;
}

LRESULT CBagMgr::GetGoodsInfo(const COS_GUID* guid, GOODS_COMMON_INFO_T& stGoodsInfoOut)
{
	LRESULT hr = S_OK;
	if (!m_stUserBag.pGoodsMgr)
		hr = E_FAIL;
	CHECK_HR(hr);

	hr = m_stUserBag.pGoodsMgr->GetGoodsInfoByGuid(guid, stGoodsInfoOut);
done:
	return hr;
}