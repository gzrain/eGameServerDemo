#ifndef _CLIENT_CORE_H_
#define _CLIENT_CORE_H_

#include "LoginClient.h"
#include "LogicClient.h"
#include "BillingClient.h"

#ifdef GAMESERVERCORE_EXPORTS
#define CLIENT_CORE_API __declspec(dllexport)
#else
#define CLIENT_CORE_API __declspec(dllimport)
#endif

/*
* 客户端对外接口
*/


class CLIENT_CORE_API CClientCore
{
public:
	CClientCore();
	~CClientCore();

	LRESULT Init(LPCSTR lpLoginSerIP,LPCSTR lpLogicSerIP, LPCSTR lpBillingSerIP);
	LRESULT UnInit();

	//Login test
	LRESULT Login(LPCTSTR lpstrAccount, LPCTSTR lpstrPswd);
	LRESULT LogOut();
	LRESULT SignUp(LPCTSTR lpstrAccount, LPCTSTR lpstrPswd, LPCTSTR lpstrName, LPCTSTR lpstrAlias);
	LRESULT GetUserAccountInfo(LPUSER_ACCOUNT_T lpstUserAccountInfo);

	//Logic test
	LRESULT GetUserProperty(LPUSER_GAME_PROPERTY_T lpstUserProperty);
	LRESULT SetUserProperty( LPUSER_GAME_PROPERTY_T lpstUserProperty);

	LRESULT GetCoordInfo(LPUSER_COORDINATE_INFO_T lpstUserCoordInfo);
	LRESULT SetCoordInfo(LPUSER_COORDINATE_INFO_T lpstUserCoordInfo);

	int GetUserBagGoodsCnt();
	LRESULT GetUserBagGoodsInfoByIdx(int nGoodsIdx, LPGOODS_COMMON_INFO_T lpstGoodsInfoOut);

	int GetShopGoodsCnt();
	LRESULT GetShopGoodsInfoByIdx(int nGoodsIdx, LPGOODS_COMMON_INFO_T lpstGoodsInfoOut);

	//Billing test
	LRESULT BuyGoods(const COS_GUID* pGoodsGuid, int nGoodsCnt);
	LRESULT SellGoods(const COS_GUID* pGoodsGuid, int nGoodsCnt);
	LRESULT DropGoods(const COS_GUID* pGoodsGuid, int nGoodsCnt);

protected:
	CLoginClient m_LoginClient;
	CLogicClient m_LogicClient;
	CBillingClient m_BillingClient;

	USER_ACCOUNT_T m_stUserAccount;
	USER_GAME_PROPERTY_T m_stUserPropery;
	USER_COORDINATE_INFO_T m_stUserCoordInfo;
};

#endif