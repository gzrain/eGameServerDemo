#include "stdafx.h"
#include <assert.h>
#include "JumpUtil.h"
#include <tchar.h>
#include "MySqlDBA.h"
#include "UnicodeConverter.h"
#include <time.h>

#define MYSQL_SERVER_PORT_DEF			(3306)
#define MYSQL_CHARSET_NAME_DEF		("UTF8")

#ifndef LINUX
#pragma comment(lib, "./DBA/MySqlSDK/libmysql.lib")
#endif

CMySqlDBA::CMySqlDBA()
{
	m_pMySQL = NULL;
	m_pResRet = NULL;
	m_ppCurRow = NULL;
	m_llRecordCount = 0;
	m_nFieldCount = 0;
}

CMySqlDBA::~CMySqlDBA()
{
}

BOOL CMySqlDBA::OpenDBA(LPCTSTR strSerIP, LPCTSTR strDBName,
	LPCTSTR strUser, LPCTSTR strPwd, int nTimeOut, LPTSTR pstrErrOut, UINT unErrSizeInByte)
{
	int nResult = 0;
	BOOL bRet = TRUE;
	unsigned int unPort = MYSQL_SERVER_PORT_DEF;
	string strSerIPA, strDBNameA, strUserA, strPwdA;

	unicodeToUTF8(strSerIP, strSerIPA);
	unicodeToUTF8(strDBName, strDBNameA);
	unicodeToUTF8(strUser, strUserA);
	unicodeToUTF8(strPwd, strPwdA);

	CHECK_SOK(nResult = mysql_library_init(0, NULL, NULL));

	m_pMySQL = mysql_init(NULL);
	if (NULL == m_pMySQL)
	{
		nResult = -1;
	}
	CHECK_SOK(nResult);

	CHECK_SOK(nResult = mysql_options(m_pMySQL, 
		MYSQL_SET_CHARSET_NAME, MYSQL_CHARSET_NAME_DEF));			// must set the options

	if (NULL == mysql_real_connect(m_pMySQL, strSerIPA.c_str(), strUserA.c_str(), strPwdA.c_str(),
		strDBNameA.c_str(), unPort, NULL, CLIENT_FOUND_ROWS))
	{
		nResult = -1;
	}
	CHECK_SOK(nResult);

done:
	if (S_OK != nResult)
	{
		if (pstrErrOut)
		{
			tstring wstrErr(_T("mysql instance is null"));
			if (m_pMySQL)
				utf8ToUnicode(mysql_error(m_pMySQL), wstrErr);
			_stprintf_s(pstrErrOut, unErrSizeInByte / sizeof(TCHAR), _T("%s"), wstrErr.c_str());
		}
		CloseDBA();
		bRet = FALSE;
	}

	return bRet;
}

void CMySqlDBA::CloseDBA()
{
	if (m_pMySQL)
	{
		mysql_close(m_pMySQL);
		m_pMySQL = NULL;
	}
	mysql_library_end();
}

int CMySqlDBA::SelectSQL(LPCTSTR szSqlStr)
{
	int nRet = -1;
	int nResult = 0;
	string strSqlStrA;
	unicodeToUTF8(szSqlStr, strSqlStrA);

	CloseRecordset();
	assert(m_pMySQL);
	CHECK_SOK(nResult = mysql_query(m_pMySQL, strSqlStrA.c_str()));
	m_pResRet = mysql_store_result(m_pMySQL);

	assert(m_pResRet);
	m_llRecordCount = mysql_num_rows(m_pResRet);
	m_nFieldCount = mysql_num_fields(m_pResRet);

done:
	if (nResult == 0)
		nRet = (int)m_llRecordCount;

	return nRet;
}

int CMySqlDBA::ExcuteSQL(LPCTSTR szSqlStr)
{
	int nRet = 0;
	string strSqlStrA;
	unicodeToUTF8(szSqlStr, strSqlStrA);

	assert(m_pMySQL);
	CHECK_SOK(nRet = mysql_query(m_pMySQL, strSqlStrA.c_str()));

done:
	return nRet;
}

void CMySqlDBA::CloseRecordset()
{
	if (m_pResRet)
	{
		mysql_free_result(m_pResRet);
		m_pResRet = NULL;
	}
	m_llRecordCount = 0;
	m_nFieldCount = 0;
	m_ppCurRow = NULL;
}

 BOOL CMySqlDBA::GotoFirst()
 {
 	BOOL bRet = FALSE;
 
 	if (m_llRecordCount > 0)
 	{
 		mysql_data_seek(m_pResRet, 0);
		m_ppCurRow = mysql_fetch_row(m_pResRet);
 		bRet = TRUE;
 	}
 
 	return bRet;
 }

BOOL CMySqlDBA::GotoNext()
{
	BOOL bRet = FALSE;

	m_ppCurRow = mysql_fetch_row(m_pResRet);
	if (NULL != m_ppCurRow)
	{
		bRet = TRUE;
	}

	return bRet;
}

BOOL CMySqlDBA::GetFieldAsBool(LPCTSTR szColName)
{
	BOOL bReturn = FALSE;
	UINT unFieldType = 0;
	int nCol = GetColIdxAndTypeByName(szColName, unFieldType);
	CHECK_HR(nCol);

	assert(m_ppCurRow);
	char *cRet = m_ppCurRow[nCol];
	if (NULL != cRet)
	{
		int nBool = atoi(cRet);
		bReturn = (BOOL)(nBool);
	}

done:
	return bReturn;
}

double CMySqlDBA::GetFieldAsDateTime(LPCTSTR szColName)
{
	BOOL bReturn = FALSE;
	UINT unFieldType = 0;
	struct tm time = { 0 };
	time_t tTime = 0;
	int nYear = 0, nMonth = 0, nDay = 0, nHour = 0, nMin = 0, nSec = 0;

	int nCol = GetColIdxAndTypeByName(szColName, unFieldType);
	CHECK_HR(nCol);

	if (unFieldType != MYSQL_TYPE_DATETIME)
	{
		nCol = -1;
	}
	CHECK_HR(nCol);

	assert(m_ppCurRow);
	char *cFieldVal = m_ppCurRow[nCol];

	sscanf_s(cFieldVal, "%d-%d-%d %d:%d:%d",
		&nYear, &nMonth, &nDay, &nHour, &nMin, &nSec);

	if (nYear < 1900 || nMonth <= 0 || nMonth > 12 || nDay <= 0 || nDay > 31 ||
		nHour < 0 || nHour > 23 || nMin < 0 || nMin > 59 || nSec < 0 || nSec > 59)
		return FALSE;

	time.tm_year = nYear - 1900;//year. the value equal to the real year minus 1990
	time.tm_mon = nMonth - 1;//month. start from Feb to Dec. 0 standard for Feb.the value range is:[0,11]
	time.tm_mday = nDay;//day. the value range is:[1,31]
	time.tm_hour = nHour;//hour. the value range is:[0,23]
	time.tm_min = nMin;//minute. the value range is:[0,59]
	time.tm_sec = nSec;//second. the value range is: [0,59]
	tTime = mktime(&time);

done:
	return (double)tTime;
}

double CMySqlDBA::GetFieldAsDouble(LPCTSTR szColName)
{
	double dbReturn = -1;
	UINT unFieldType = 0;
	int nCol = GetColIdxAndTypeByName(szColName, unFieldType);
	CHECK_HR(nCol);

	assert(m_ppCurRow);
	char *cFieldVal = m_ppCurRow[nCol];
	if (NULL != cFieldVal)
	{
		dbReturn = atof(cFieldVal);
	}

done:
	return dbReturn;
}

int CMySqlDBA::GetFieldAsInt32(LPCTSTR szColName)
{
	int nReturn = -1;
	UINT unFieldType = 0;
	int nCol = GetColIdxAndTypeByName(szColName, unFieldType);
	CHECK_HR(nCol);

	assert(m_ppCurRow);
	char *cFieldVal = m_ppCurRow[nCol];
	if (NULL != cFieldVal)
	{
		nReturn = atoi(cFieldVal);
	}

done:
	return nReturn;
}

UINT64 CMySqlDBA::GetFieldAsInt64(LPCTSTR szColName)
{
	UINT64 nReturn = 0;
	UINT unFieldType = 0;
	int nCol = GetColIdxAndTypeByName(szColName, unFieldType);
	CHECK_HR(nCol);

	assert(m_ppCurRow);
	char *cFieldVal = m_ppCurRow[nCol];
	if (NULL != cFieldVal)
	{
		nReturn = atoll(cFieldVal);
	}

done:
	return nReturn;
}

tstring CMySqlDBA::GetFieldAsString(LPCTSTR szColName)
{
	tstring tstrReturn;
	UINT unFieldType = 0;
	int nCol = GetColIdxAndTypeByName(szColName, unFieldType);
	CHECK_HR(nCol);

	assert(m_ppCurRow);
	char *cFieldVal = m_ppCurRow[nCol];

	if (NULL != cFieldVal)
		utf8ToUnicode(cFieldVal, tstrReturn);
	else
		tstrReturn = _T("");

done:
	return tstrReturn;
}

int CMySqlDBA::GetColIdxAndTypeByName(LPCTSTR szColName, UINT &unFieldType)
{
	int nCurCol = 0;
	string strColName;
	MYSQL_FIELD* pCurField = NULL;

	if (!m_pResRet)
		nCurCol = -1;
	CHECK_HR(nCurCol);

	unicodeToUTF8(szColName, strColName);
	mysql_field_seek(m_pResRet, 0);
	while (pCurField = mysql_fetch_field(m_pResRet))
	{
		int nRet = strcmp(pCurField->name, strColName.c_str());
		if (0 == nRet)
		{
			unFieldType = pCurField->type;
			break;
		}
		nCurCol++;
	}

	if (nCurCol == m_nFieldCount)
		nCurCol = -1;

done:
	return nCurCol;
}

int CMySqlDBA::GetLastDBAErrno()
{
	int nErrNo = 0;
	if (m_pMySQL)
		nErrNo = mysql_errno(m_pMySQL);
	
	return nErrNo;
}

BOOL CMySqlDBA::GetLastDBAError(LPTSTR strOutErrInfo, UINT unSizeInByte)
{
	BOOL bRet = FALSE;
	tstring strTemp;

	string strErr("mysql instance is null");
	if(m_pMySQL)
		strErr = mysql_error(m_pMySQL);

	utf8ToUnicode(strErr, strTemp);

	int nErrInfoLen = strTemp.length();
	if (nErrInfoLen > 0 && nErrInfoLen <= (int)unSizeInByte)
	{
		_stprintf_s(strOutErrInfo, unSizeInByte / sizeof(TCHAR), _T("%s"), strTemp.c_str());
		bRet = TRUE;
	}

	return bRet;
}