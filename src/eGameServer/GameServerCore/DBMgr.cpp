
#include "DBMgr.h"
#include "JumpUtil.h"
#include "SafeRelease.h"
#include <assert.h>
#include <time.h>
#include "IniOp.h"

CDBMgr::CDBMgr()
{
	m_nDBType = DBA_TYPE_SQLITE;
	memset(m_strDBUserName, 0, sizeof(m_strDBUserName));
	memset(m_strDBUserPswd, 0, sizeof(m_strDBUserPswd));
	memset(m_strDBName, 0, sizeof(m_strDBName));
	memset(m_strMySqlHostIP, 0, sizeof(m_strMySqlHostIP));

}

CDBMgr::~CDBMgr()
{
	UnInit();
}

LRESULT CDBMgr::Init()
{
	return ReadCfgFromIni();
}

LRESULT CDBMgr::UnInit()
{
	return S_OK;
}

LRESULT CDBMgr::OpenDB()
{
	LRESULT hr = S_OK;

	hr = m_DBMgr.OpenDBA(m_strMySqlHostIP,
		m_strDBName, m_strDBUserName, m_strDBUserPswd);
	return hr;
}

LRESULT CDBMgr::CloseDB()
{
	LRESULT hr = S_OK;

	hr = m_DBMgr.CloseDBA();

	return hr;
}

//LoginServer
LRESULT CDBMgr::CheckAccount(LPCTSTR lpstrAccount, LPCTSTR lpstrPswd, LPUSER_ACCOUNT_T lpstUserAccountOut)
{
	LRESULT hr = S_OK;
	TCHAR strSheetName[] = { _T("user_account") };

	TCHAR strSql[1024] = { 0 };
	TCHAR strGuid[MIN_GUID_STR_LEN] = { 0 };
	int nSelectCnt = 0;
	int nSizeInWords = 0;

	TCHAR strName[MAX_USER_NAME_LEN] = { 0 };
	TCHAR strAlias[MAX_USER_ALIAS_LEN] = { 0 };
	TCHAR strDateTime[64] = { 0 };
	COS_GUID guid = GUID_NULL;

	CHECK_HR(hr = OpenDB());

	_stprintf_s(strSql, sizeof(strSql) / sizeof(strSql[0]), _T("select * from %s where account='%s'"), strSheetName, lpstrAccount);
	nSelectCnt = m_DBMgr.SelectSQL(strSql);

	hr = E_FAIL;
	m_DBMgr.MoveToFirst();

	for (int i = 0; i < nSelectCnt; i++)
	{

		nSizeInWords = sizeof(strGuid) / sizeof(strGuid[0]);
		m_DBMgr.GetFieldAsString(_T("guid"), strGuid, nSizeInWords);
		CrossPlatform_StringToGuid(strGuid, &guid);
		lpstUserAccountOut->guidUser = guid;

		nSizeInWords = sizeof(strAlias) / sizeof(strAlias[0]);
		m_DBMgr.GetFieldAsString(_T("alias"), strAlias, nSizeInWords);
		_tcscpy_s(lpstUserAccountOut->szUserAlias, nSizeInWords, strAlias);

		nSizeInWords = sizeof(strName) / sizeof(strName[0]);
		m_DBMgr.GetFieldAsString(_T("name"), strName, nSizeInWords);
		_tcscpy_s(lpstUserAccountOut->szUserName, nSizeInWords, strName);

		lpstUserAccountOut->tmLastLogin = m_DBMgr.GetFieldAsDateTime(_T("last_login_datetime"));

		hr = S_OK;
		break;
	}
	CHECK_HR(hr);
	m_DBMgr.FinishSelectTask();

	//更新最近登录时间
	GetTimeString(strDateTime, sizeof(strDateTime) / sizeof(strDateTime[0]));
	_stprintf_s(strSql, sizeof(strSql) / sizeof(strSql[0]), _T("UPDATE user_account SET last_login_datetime='%s' where guid = '%s' "),
		strDateTime, strGuid);
	hr = m_DBMgr.ExcuteSQL(strSql);
	
done:
	CloseDB();
	return hr;
}
LRESULT CDBMgr::UserSignup(LPCTSTR lpstrAccount, LPCTSTR lpstrPswd, LPCTSTR lpstrName, LPCTSTR lpstrAlias)
{
	LRESULT hr = S_OK;
	TCHAR strSheetName[] = { _T("user_account") };

	TCHAR strSql[1024] = { 0 };
	TCHAR strGuid[MIN_GUID_STR_LEN] = { 0 };
	int nSelectCnt = 0;
	int nSizeInWords = 0;

	TCHAR strDateTime[64] = { 0 };
	TCHAR strName[MAX_USER_NAME_LEN] = { 0 };
	TCHAR strAlias[MAX_USER_ALIAS_LEN] = { 0 };
	COS_GUID guid = GUID_NULL;

	CHECK_HR(hr = OpenDB());

	_stprintf_s(strSql, sizeof(strSql) / sizeof(strSql[0]), _T("select * from %s where account='%s'"), strSheetName, lpstrAccount);
	nSelectCnt = m_DBMgr.SelectSQL(strSql);

	if (nSelectCnt > 0)
	{
		// 用户名已存在
		hr = E_FAIL;
	}
	CHECK_HR(hr);

	// 清除记录集
	m_DBMgr.FinishSelectTask();

	//新插入一条用户
	CrossPlatform_CreateGuid(&guid);
	CrossPlatform_GUID2String(guid, strGuid, MIN_GUID_STR_LEN);
	GetTimeString(strDateTime, sizeof(strDateTime) / sizeof(strDateTime[0]));

	_stprintf_s(strSql, sizeof(strSql) / sizeof(strSql[0]), _T("INSERT INTO user_account(guid,account,password,name,alias,last_login_datetime) \
							VALUES('%s', '%s', '%s', '%s', '%s', '%s') "), strGuid, lpstrAccount, lpstrPswd, lpstrName, lpstrAlias, strDateTime);

	CHECK_HR(hr = m_DBMgr.ExcuteSQL(strSql));

done:
	CloseDB();
	return hr;
}

//LogicServer
LRESULT CDBMgr::GetUserCoordinate(const COS_GUID *guidUser, LPUSER_COORDINATE_INFO_T lpstUserCoordInfoOut)
{
	LRESULT hr = S_OK;
	TCHAR strSheetName[] = { _T("user_property") };

	TCHAR strSql[1024] = { 0 };
	TCHAR strGuid[MIN_GUID_STR_LEN] = { 0 };
	int nSelectCnt = 0;
	int nSizeInWords = 0;

	TCHAR strName[MAX_USER_NAME_LEN] = { 0 };
	TCHAR strAlias[MAX_USER_ALIAS_LEN] = { 0 };
	COS_GUID guid = GUID_NULL;
	CrossPlatform_GUID2String(*guidUser, strGuid, MIN_GUID_STR_LEN);

	CHECK_HR(hr = OpenDB());

	_stprintf_s(strSql, sizeof(strSql) / sizeof(strSql[0]), _T("select * from %s where user_guid='%s'"), strSheetName, strGuid);
	nSelectCnt = m_DBMgr.SelectSQL(strSql);

	hr = E_FAIL;
	m_DBMgr.MoveToFirst();

	for (int i = 0; i < nSelectCnt; i++)
	{
		lpstUserCoordInfoOut->nCoordX =  m_DBMgr.GetFieldAsInt32(_T("coordinate_x"));
		lpstUserCoordInfoOut->nCoordY = m_DBMgr.GetFieldAsInt32(_T("coordinate_y"));
		lpstUserCoordInfoOut->enumGameMap = (GAME_MAP_ENUM)m_DBMgr.GetFieldAsInt32(_T("map_index"));

		hr = S_OK;
		break;
	}
	CHECK_HR(hr);
	m_DBMgr.FinishSelectTask();

done:
	CloseDB();
	return hr;
}
LRESULT CDBMgr::SetUserCoordinate(const COS_GUID *guidUser, LPUSER_COORDINATE_INFO_T lpstUserCoordInfoIn)
{
	LRESULT hr = S_OK;
	TCHAR strSheetName[] = { _T("user_property") };

	TCHAR strSql[1024] = { 0 };
	TCHAR strGuid[MIN_GUID_STR_LEN] = { 0 };
	int nSelectCnt = 0;
	int nSizeInWords = 0;

	CrossPlatform_GUID2String(*guidUser, strGuid, MIN_GUID_STR_LEN);

	CHECK_HR(hr = OpenDB());

	_stprintf_s(strSql, sizeof(strSql) / sizeof(strSql[0]), _T("select * from %s where user_guid='%s'"), strSheetName, strGuid);
	nSelectCnt = m_DBMgr.SelectSQL(strSql);

	m_DBMgr.FinishSelectTask();
	if (0 == nSelectCnt)
	{
		//如果没有记录，就插入一条
		_stprintf_s(strSql, sizeof(strSql) / sizeof(strSql[0]), _T("INSERT INTO %s(user_guid, coordinate_x, coordinate_y, map_index) VALUES('%s', %d, %d, %d) "),
			strSheetName, strGuid, lpstUserCoordInfoIn->nCoordX, lpstUserCoordInfoIn->nCoordY, (int)lpstUserCoordInfoIn->enumGameMap);
	}
	else
	{
		_stprintf_s(strSql, sizeof(strSql) / sizeof(strSql[0]), _T("UPDATE %s SET coordinate_x=%d, coordinate_y=%d, map_index=%d  where user_guid='%s' "),
			strSheetName, lpstUserCoordInfoIn->nCoordX, lpstUserCoordInfoIn->nCoordY, (int)lpstUserCoordInfoIn->enumGameMap, strGuid);
	}
	hr = m_DBMgr.ExcuteSQL(strSql);

done:
	CloseDB();
	return hr;
}

LRESULT CDBMgr::GetUserBag(const COS_GUID *guidUser, CGoodsMgr* pBagMgr)
{
	LRESULT hr = S_OK;
	TCHAR strSheetName[] = { _T("user_bag")};

	TCHAR strSql[1024] = { 0 };
	TCHAR strGuid[MIN_GUID_STR_LEN] = { 0 };
	int nSelectCnt = 0;
	int nSizeInWords = 0;

	TCHAR strName[MAX_USER_NAME_LEN] = { 0 };
	TCHAR strAlias[MAX_USER_ALIAS_LEN] = { 0 };
	COS_GUID guid = GUID_NULL;
	CrossPlatform_GUID2String(*guidUser, strGuid, MIN_GUID_STR_LEN);

	CHECK_HR(hr = OpenDB());

	_stprintf_s(strSql, sizeof(strSql) / sizeof(strSql[0]), _T("select * from %s, shop_info where user_guid='%s' and guid=goods_guid "), strSheetName, strGuid);
	nSelectCnt = m_DBMgr.SelectSQL(strSql);

	m_DBMgr.MoveToFirst();

	for (int i = 0; i < nSelectCnt; i++)
	{
		GOODS_COMMON_INFO_T stGoodsInfo = { 0 };
		
		stGoodsInfo.fGoodsPrice = (float)m_DBMgr.GetFieldAsDouble(_T("price"));
		
		stGoodsInfo.unGoodsCnt = m_DBMgr.GetFieldAsInt32(_T("count"));

		nSizeInWords = sizeof(strGuid) / sizeof(strGuid[0]);
		m_DBMgr.GetFieldAsString(_T("goods_guid"), strGuid, nSizeInWords);
		CrossPlatform_StringToGuid(strGuid, &guid);
		stGoodsInfo.guidGoods = guid;

		nSizeInWords = sizeof(strName) / sizeof(strName[0]);
		m_DBMgr.GetFieldAsString(_T("name"), strName, nSizeInWords);
		_tcscpy_s(stGoodsInfo.szGoodsName, nSizeInWords, strName);

		nSizeInWords = sizeof(strAlias) / sizeof(strAlias[0]);
		m_DBMgr.GetFieldAsString(_T("desc"), strAlias, nSizeInWords);
		_tcscpy_s(stGoodsInfo.szGoodsDesc, nSizeInWords, strAlias);

		pBagMgr->AddGoods(&stGoodsInfo);
		m_DBMgr.MoveToNext();
	}
	m_DBMgr.FinishSelectTask();

done:
	CloseDB();
	return hr;
}

LRESULT CDBMgr::GetShop(CGoodsMgr *pShopMgr)
{
	LRESULT hr = S_OK;
	TCHAR strSheetName[] = { _T("shop_info") };

	TCHAR strSql[1024] = { 0 };
	TCHAR strGuid[MIN_GUID_STR_LEN] = { 0 };
	int nSelectCnt = 0;
	int nSizeInWords = 0;

	TCHAR strName[MAX_USER_NAME_LEN] = { 0 };
	TCHAR strAlias[MAX_USER_ALIAS_LEN] = { 0 };
	COS_GUID guid = GUID_NULL;

	CHECK_HR(hr = OpenDB());

	_stprintf_s(strSql, sizeof(strSql) / sizeof(strSql[0]), _T("select * from %s "), strSheetName, strGuid);
	nSelectCnt = m_DBMgr.SelectSQL(strSql);

	m_DBMgr.MoveToFirst();

	for (int i = 0; i < nSelectCnt; i++)
	{
		GOODS_COMMON_INFO_T stGoodsInfo = { 0 };

		stGoodsInfo.fGoodsPrice = (float)m_DBMgr.GetFieldAsDouble(_T("price"));

		nSizeInWords = sizeof(strGuid) / sizeof(strGuid[0]);
		m_DBMgr.GetFieldAsString(_T("guid"), strGuid, nSizeInWords);
		CrossPlatform_StringToGuid(strGuid, &guid);
		stGoodsInfo.guidGoods = guid;

		nSizeInWords = sizeof(strName) / sizeof(strName[0]);
		m_DBMgr.GetFieldAsString(_T("name"), strName, nSizeInWords);
		_tcscpy_s(stGoodsInfo.szGoodsName, nSizeInWords, strName);

		nSizeInWords = sizeof(strAlias) / sizeof(strAlias[0]);
		m_DBMgr.GetFieldAsString(_T("desc"), strAlias, nSizeInWords);
		_tcscpy_s(stGoodsInfo.szGoodsDesc, nSizeInWords, strAlias);

		pShopMgr->AddGoods(&stGoodsInfo);
		m_DBMgr.MoveToNext();
	}
	m_DBMgr.FinishSelectTask();

done:
	CloseDB();
	return hr;
}

LRESULT CDBMgr::GetUserProperty(const COS_GUID *guidUser, LPUSER_GAME_PROPERTY_T lpstUserPropertyOut)
{
	LRESULT hr = S_OK;
	TCHAR strSheetName[] = { _T("user_property") };

	TCHAR strSql[1024] = { 0 };
	TCHAR strGuid[MIN_GUID_STR_LEN] = { 0 };
	int nSelectCnt = 0;
	int nSizeInWords = 0;

	TCHAR strName[MAX_USER_NAME_LEN] = { 0 };
	TCHAR strAlias[MAX_USER_ALIAS_LEN] = { 0 };
	CrossPlatform_GUID2String(*guidUser, strGuid, MIN_GUID_STR_LEN);

	CHECK_HR(hr = OpenDB());

	_stprintf_s(strSql, sizeof(strSql) / sizeof(strSql[0]), _T("select * from %s where user_guid='%s'"), strSheetName, strGuid);
	nSelectCnt = m_DBMgr.SelectSQL(strSql);

	hr = E_FAIL;
	m_DBMgr.MoveToFirst();

	for (int i = 0; i < nSelectCnt; i++)
	{
		lpstUserPropertyOut->guidUser = *guidUser;

		lpstUserPropertyOut->unBagType = m_DBMgr.GetFieldAsInt32(_T("bag_type"));
		lpstUserPropertyOut->unUserLevel = m_DBMgr.GetFieldAsInt32(_T("level"));
		lpstUserPropertyOut->fUserMoney = (float)m_DBMgr.GetFieldAsDouble(_T("money"));
		hr = S_OK;
		break;
	}
	CHECK_HR(hr);
	m_DBMgr.FinishSelectTask();

done:
	CloseDB();
	return hr;

}
LRESULT CDBMgr::SetUserProperty(const COS_GUID *guidUser, LPUSER_GAME_PROPERTY_T lpstUserPropertyIn)
{
	LRESULT hr = S_OK;
	TCHAR strSheetName[] = { _T("user_property") };

	TCHAR strSql[1024] = { 0 };
	TCHAR strGuid[MIN_GUID_STR_LEN] = { 0 };
	int nSelectCnt = 0;
	int nSizeInWords = 0;

	CrossPlatform_GUID2String(*guidUser, strGuid, MIN_GUID_STR_LEN);

	CHECK_HR(hr = OpenDB());

	_stprintf_s(strSql, sizeof(strSql) / sizeof(strSql[0]), _T("UPDATE %s SET level=%d, bag_type=%d, money=%f  where user_guid='%s' "),
		strSheetName, lpstUserPropertyIn->unUserLevel, lpstUserPropertyIn->unBagType, lpstUserPropertyIn->fUserMoney, strGuid);
	hr = m_DBMgr.ExcuteSQL(strSql);

done:
	CloseDB();
	return hr;

}

//BillingServer  
/*
*	由于是Demo，所以交易验证模块未做，购买或者出售，直接返回正确
*/
LRESULT CDBMgr::UserBuyGoods(const COS_GUID *guidUser, const COS_GUID*guidGoods, int nGoodsCnt)
{
	LRESULT hr = S_OK;
	TCHAR strSheetName[] = { _T("user_bag") };

	TCHAR strSql[1024] = { 0 };
	TCHAR strGuid[MIN_GUID_STR_LEN] = { 0 };
	int nSelectCnt = 0;
	int nSizeInWords = 0;

	CrossPlatform_GUID2String(*guidUser, strGuid, MIN_GUID_STR_LEN);
	TCHAR strGuidGoods[MIN_GUID_STR_LEN] = { 0 };
	CrossPlatform_GUID2String(*guidGoods, strGuidGoods, MIN_GUID_STR_LEN);
	int nOldCnt = 0;
	int nNewCnt = 0;
	float fGoodsPrice = 0;


	CHECK_HR(hr = OpenDB());

	_stprintf_s(strSql, sizeof(strSql) / sizeof(strSql[0]), _T("select * from %s where user_guid='%s' and goods_guid='%s' "), strSheetName, strGuid, strGuidGoods);
	nSelectCnt = m_DBMgr.SelectSQL(strSql);

	m_DBMgr.MoveToFirst();

	if (0 == nSelectCnt)
	{
		//商品之前未购买过，增加一条记录
		_stprintf_s(strSql, sizeof(strSql) / sizeof(strSql[0]), _T("INSERT INTO %s(user_guid,goods_guid,count) VALUES('%s', '%s', %d) "), strSheetName, strGuid, strGuidGoods, nGoodsCnt);

	}
	else if (1 == nSelectCnt)
	{
		//商品之前购买过，修改记录
		nOldCnt = m_DBMgr.GetFieldAsInt32(_T("count"));
		nNewCnt = nGoodsCnt + nOldCnt;
		_stprintf_s(strSql, sizeof(strSql) / sizeof(strSql[0]), _T("UPDATE %s SET count=%d where user_guid='%s' and goods_guid='%s' "),
			strSheetName, nNewCnt, strGuid, strGuidGoods);

	}
	else
	{
		assert(0);
	}

	hr = m_DBMgr.ExcuteSQL(strSql);

done:
	CloseDB();
	return hr;
}

LRESULT CDBMgr::UserSellGoods(const COS_GUID *guidUser, const COS_GUID*guidGoods, int nGoodsCnt)
{
	LRESULT hr = S_OK;
	TCHAR strSheetName[] = { _T("user_bag") };

	TCHAR strSql[1024] = { 0 };
	TCHAR strGuid[MIN_GUID_STR_LEN] = { 0 };
	int nSelectCnt = 0;
	int nSizeInWords = 0;

	CrossPlatform_GUID2String(*guidUser, strGuid, MIN_GUID_STR_LEN);
	TCHAR strGuidGoods[MIN_GUID_STR_LEN] = { 0 };
	CrossPlatform_GUID2String(*guidGoods, strGuidGoods, MIN_GUID_STR_LEN);
	int nOldCnt = 0;
	int nNewCnt = 0;
	float fGoodsPrice = 0;


	CHECK_HR(hr = OpenDB());

	_stprintf_s(strSql, sizeof(strSql) / sizeof(strSql[0]), _T("select * from %s where user_guid='%s' and goods_guid='%s' "), strSheetName, strGuid, strGuidGoods);
	nSelectCnt = m_DBMgr.SelectSQL(strSql);

	m_DBMgr.MoveToFirst();

	if (0 == nSelectCnt)
	{
		//商品之前未购买过,无法卖出
		hr = E_FAIL;
	}
	else if (1 == nSelectCnt)
	{
		//商品之前购买过，修改记录
		nOldCnt = m_DBMgr.GetFieldAsInt32(_T("count"));
		nNewCnt = (nOldCnt - nGoodsCnt) > 0 ? (nOldCnt-nGoodsCnt) : 0;
		_stprintf_s(strSql, sizeof(strSql) / sizeof(strSql[0]), _T("UPDATE %s SET count=%d where user_guid='%s' and goods_guid='%s' "),
			strSheetName, nNewCnt, strGuid, strGuidGoods);

	}
	else
	{
		assert(0);
	}

	hr = m_DBMgr.ExcuteSQL(strSql);

done:
	CloseDB();
	return hr;
}
LRESULT CDBMgr::UserDropGoods(const COS_GUID *guidUser, const COS_GUID*guidGoods, int nGoodsCnt)
{
	LRESULT hr = S_OK;
	TCHAR strSheetName[] = { _T("user_bag") };

	TCHAR strSql[1024] = { 0 };
	TCHAR strGuid[MIN_GUID_STR_LEN] = { 0 };
	int nSelectCnt = 0;
	int nSizeInWords = 0;

	CrossPlatform_GUID2String(*guidUser, strGuid, MIN_GUID_STR_LEN);
	TCHAR strGuidGoods[MIN_GUID_STR_LEN] = { 0 };
	CrossPlatform_GUID2String(*guidGoods, strGuidGoods, MIN_GUID_STR_LEN);
	int nOldCnt = 0;
	int nNewCnt = 0;
	float fGoodsPrice = 0;


	CHECK_HR(hr = OpenDB());

	_stprintf_s(strSql, sizeof(strSql) / sizeof(strSql[0]), _T("select * from %s where user_guid='%s' and goods_guid='%s' "), strSheetName, strGuid, strGuidGoods);
	nSelectCnt = m_DBMgr.SelectSQL(strSql);

	m_DBMgr.MoveToFirst();

	if (0 == nSelectCnt)
	{
		//商品之前未购买过,无法卖出
		hr = E_FAIL;
	}
	else if (1 == nSelectCnt)
	{
		//商品之前购买过，修改记录
		nOldCnt = m_DBMgr.GetFieldAsInt32(_T("count"));
		nNewCnt = (nOldCnt - nGoodsCnt) > 0 ? (nOldCnt - nGoodsCnt) : 0;
		_stprintf_s(strSql, sizeof(strSql) / sizeof(strSql[0]), _T("UPDATE %s SET count=%d where user_guid='%s' and goods_guid='%s' "),
			strSheetName, nNewCnt, strGuid, strGuidGoods);
	}
	else
	{
		assert(0);
	}

	hr = m_DBMgr.ExcuteSQL(strSql);

done:
	CloseDB();
	return hr;
}

LRESULT CDBMgr::GetTimeString(LPTSTR lpstrReturn, DWORD dwSizeInWords)
{
	SYSTEMTIME timeSys = { 0 };
	GetLocalTime(&timeSys);
	memset(lpstrReturn, 0, dwSizeInWords*sizeof(char));

	_stprintf_s(lpstrReturn, dwSizeInWords, _T("%d-%02d-%02d %02d:%02d:%02d"), timeSys.wYear, timeSys.wMonth, timeSys.wDay
		, timeSys.wHour, timeSys.wMinute, timeSys.wSecond);
	return S_OK;
}

LRESULT CDBMgr::ReadCfgFromIni()
{
	//读ini文件
	TCHAR strIPDefault[] = { _T("127.0.0.1") };

	INI_GetString(_T("DBMgr"), _T("db_ip"), strIPDefault, m_strMySqlHostIP, MAX_IPSTR_LEN, _T("Setting.ini"));
	INI_GetString(_T("DBMgr"), _T("db_name"), _T("egameserver"), m_strDBName, MAX_DBPARAM_LEN, _T("Setting.ini"));
	INI_GetString(_T("DBMgr"), _T("db_user_name"), _T("root"), m_strDBUserName, MAX_DBPARAM_LEN, _T("Setting.ini"));
	INI_GetString(_T("DBMgr"), _T("db_user_password"), _T("test"), m_strDBUserPswd, MAX_DBPARAM_LEN, _T("Setting.ini"));

	//写ini文件
	INI_WriteString(_T("DBMgr"), _T("db_ip"), m_strMySqlHostIP,  _T("Setting.ini"));
	INI_WriteString(_T("DBMgr"), _T("db_name"), m_strDBName, _T("Setting.ini"));
	INI_WriteString(_T("DBMgr"), _T("db_user_name"), m_strDBUserName, _T("Setting.ini"));
	INI_WriteString(_T("DBMgr"), _T("db_user_password"), m_strDBUserPswd, _T("Setting.ini"));

	return S_OK;
}
