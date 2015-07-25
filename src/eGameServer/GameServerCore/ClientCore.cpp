#include "ClientCore.h"
#include "Protocol.h"
#include <tchar.h>
#include "JumpUtil.h"
#include "SafeRelease.h"

CClientCore::CClientCore()
{
	ZeroMemory(&m_stUserAccount, sizeof(m_stUserAccount));
	ZeroMemory(&m_stUserCoordInfo, sizeof(m_stUserCoordInfo));
	ZeroMemory(&m_stUserPropery, sizeof(m_stUserPropery));
}

CClientCore::~CClientCore()
{
}

LRESULT CClientCore::Init(LPCSTR lpLoginSerIP, LPCSTR lpLogicSerIP, LPCSTR lpBillingSerIP)
{
	LRESULT hr = S_OK;
	UINT unLocalPort = 0;
	hr = m_LoginClient.Init(lpLoginSerIP, SRV_PORT_LOGIN, unLocalPort);
	hr = m_LogicClient.Init(lpLogicSerIP, SRV_PORT_LOGIC, unLocalPort);
	hr = m_BillingClient.Init(lpLogicSerIP, SRV_PORT_BILLING, unLocalPort);

	return hr;
}

LRESULT CClientCore::UnInit()
{
	LRESULT hr = S_OK;
	m_LogicClient.UnInit();
	m_LoginClient.UnInit();
	m_BillingClient.UnInit();
	return hr;
}

//////////////////////////////////////////////////////////////////////////
//Login test
LRESULT CClientCore::Login(LPCTSTR lpstrAccount, LPCTSTR lpstrPswd)
{
	LRESULT hr = S_OK;

	hr = m_LoginClient.Login(lpstrAccount, lpstrPswd, &m_stUserAccount);

	return hr;
}

LRESULT CClientCore::LogOut()
{
	LRESULT hr = S_OK;
	hr = m_LoginClient.LogOut(&m_stUserAccount.guidUser);
	return hr;
}

LRESULT CClientCore::SignUp(LPCTSTR lpstrAccount, LPCTSTR lpstrPswd, LPCTSTR lpstrName, LPCTSTR lpstrAlias)
{
	LRESULT hr = S_OK;
	hr = m_LoginClient.SignUp(lpstrAccount, lpstrPswd, lpstrName, lpstrAlias);
	return hr;
}

LRESULT CClientCore::GetUserAccountInfo(LPUSER_ACCOUNT_T lpstUserAccountInfo)
{
	LRESULT hr = S_OK;
	memcpy(lpstUserAccountInfo, &m_stUserAccount, sizeof(m_stUserAccount));
	return hr;
}


//////////////////////////////////////////////////////////////////////////
//Logic test
LRESULT CClientCore::GetUserProperty(LPUSER_GAME_PROPERTY_T lpstUserProperty)
{
	return m_LogicClient.GetUserProperty(&m_stUserAccount.guidUser, lpstUserProperty);
}

LRESULT CClientCore::SetUserProperty( LPUSER_GAME_PROPERTY_T lpstUserProperty)
{
	return m_LogicClient.SetUserProperty(&m_stUserAccount.guidUser, lpstUserProperty);
}

LRESULT CClientCore::GetCoordInfo(LPUSER_COORDINATE_INFO_T lpstUserCoordInfo)
{
	LRESULT hr = S_OK;

	hr = m_LogicClient.GetUserCoordInfo(&m_stUserAccount.guidUser, &m_stUserCoordInfo);

	memcpy(lpstUserCoordInfo, &m_stUserCoordInfo, sizeof(m_stUserCoordInfo));
	return hr;
}

LRESULT CClientCore::SetCoordInfo(LPUSER_COORDINATE_INFO_T lpstUserCoordInfo)
{
	LRESULT hr = S_OK;
	memcpy(&m_stUserCoordInfo, lpstUserCoordInfo, sizeof(m_stUserCoordInfo));
	hr = m_LogicClient.SetUserCoordInfo(&m_stUserAccount.guidUser, &m_stUserCoordInfo);
	return hr;
}

int CClientCore::GetUserBagGoodsCnt()
{
	return m_LogicClient.GetUserBagGoodsCnt(&m_stUserAccount.guidUser);
}

LRESULT CClientCore::GetUserBagGoodsInfoByIdx(int nGoodsIdx, LPGOODS_COMMON_INFO_T lpstGoodsInfoOut)
{
	return m_LogicClient.GetUserBagGoodsInfoByIdx(nGoodsIdx, lpstGoodsInfoOut);
}

int CClientCore::GetShopGoodsCnt()
{
	return m_LogicClient.GetShopGoodsCnt();
}

LRESULT CClientCore::GetShopGoodsInfoByIdx(int nGoodsIdx, LPGOODS_COMMON_INFO_T lpstGoodsInfoOut)
{
	return m_LogicClient.GetShopGoodsInfoByIdx(nGoodsIdx, lpstGoodsInfoOut);
}

//////////////////////////////////////////////////////////////////////////
//Billing test
LRESULT CClientCore::BuyGoods(const COS_GUID* pGoodsGuid, int nGoodsCnt)
{
	return m_BillingClient.BuyGoods(&m_stUserAccount.guidUser, pGoodsGuid, nGoodsCnt);
}

LRESULT CClientCore::SellGoods(const COS_GUID* pGoodsGuid, int nGoodsCnt)
{
	return m_BillingClient.SellGoods(&m_stUserAccount.guidUser, pGoodsGuid, nGoodsCnt);
}

LRESULT CClientCore::DropGoods(const COS_GUID* pGoodsGuid, int nGoodsCnt)
{
	return m_BillingClient.DropGoods(&m_stUserAccount.guidUser, pGoodsGuid, nGoodsCnt);
}

