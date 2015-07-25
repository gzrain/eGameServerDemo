#pragma  once

#ifndef _DBMGR_H_
#define  _DBMGR_H_

#include <windows.h>
#include "CommonDBA.h"

#include "UserData.h"

#define MIN_GUID_STR_LEN 39
#define MAX_IPSTR_LEN 24
#define MAX_DBPARAM_LEN 64
/*
* 对数据库操作模块化封装
* 用于游戏服务器与数据库交互
*/

class CDBMgr
{
public:
	CDBMgr();
	virtual ~CDBMgr();

	LRESULT Init();
	LRESULT UnInit();

	//LoginServer
	LRESULT CheckAccount(LPCTSTR lpstrAccount, LPCTSTR lpstrPswd, LPUSER_ACCOUNT_T lpstUserAccountOut);
	LRESULT UserSignup(LPCTSTR lpstrAccount, LPCTSTR lpstrPswd, LPCTSTR lpstrName, LPCTSTR lpstrAlias);

	//LogicServer
	LRESULT GetUserCoordinate(const COS_GUID *guidUser, LPUSER_COORDINATE_INFO_T lpstUserCoordInfoOut);
	LRESULT SetUserCoordinate(const COS_GUID *guidUser, LPUSER_COORDINATE_INFO_T lpstUserCoordInfoIn);

	LRESULT GetUserBag(const COS_GUID *guidUser, CGoodsMgr* pBagMgr);
	LRESULT GetShop(CGoodsMgr *pShopMgr);

	LRESULT GetUserProperty(const COS_GUID *guidUser, LPUSER_GAME_PROPERTY_T lpstUserPropertyOut);
	LRESULT SetUserProperty(const COS_GUID *guidUser, LPUSER_GAME_PROPERTY_T lpstUserPropertyIn);

	//BillingServer
	LRESULT UserBuyGoods(const COS_GUID *guidUser, const COS_GUID*guidGoods, int nGoodsCnt);
	LRESULT UserSellGoods(const COS_GUID *guidUser, const COS_GUID*guidGoods, int nGoodsCnt);
	LRESULT UserDropGoods(const COS_GUID *guidUser, const COS_GUID*guidGoods, int nGoodsCnt);


protected:
	LRESULT OpenDB();
	LRESULT CloseDB();

	LRESULT ReadCfgFromIni();
	LRESULT GetTimeString(LPTSTR lpstrReturn, DWORD dwSizeInWords);

private:
	CComnDBAMgr m_DBMgr;
	
	TCHAR m_strDBUserName[MAX_DBPARAM_LEN];
	TCHAR m_strDBUserPswd[MAX_DBPARAM_LEN];
	int m_nDBType;
	TCHAR m_strMySqlHostIP[MAX_IPSTR_LEN];
	TCHAR m_strDBName[MAX_DBPARAM_LEN];


};


#endif