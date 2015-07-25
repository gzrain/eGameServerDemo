#include "stdafx.h"
#include "SqliteDBA.h"
#include <assert.h>
#include <time.h>
#include "UnicodeConverter.h"
#include <tchar.h>

#ifndef LINUX
#pragma comment(lib, "./DBA/SqliteSDK/sqlite3.lib")
#endif

#ifndef CP_GB2312
#define CP_GB2312 20936
#endif

#define MAX_FIELD_STR_LEN		256

CSqliteDBA::CSqliteDBA()
{
	m_pSqliteDB = NULL;
	m_ppszResultSet = NULL;
	
	m_nTimeOut = 0;
	m_nRowCnt = 0;
	m_nColCnt = 0;
	m_nCurResultPos = 0;//0 is not valid. the valid range is 1 to m_nRowCnt

}

CSqliteDBA::~CSqliteDBA()
{
}

BOOL CSqliteDBA::OpenDBA(LPCTSTR strSerIP, LPCTSTR strDBName, LPCTSTR strUser, 
	LPCTSTR strPwd, int nTimeOut, LPTSTR pstrErrOut, UINT unErrSizeInByte)
{
	int nErr = -1;
	BOOL bRet = FALSE;
	string strDBFileName;
	unicodeToUTF8(strDBName, strDBFileName);
	m_nTimeOut = nTimeOut;
	assert(nTimeOut > 0);

	//open database to which the strDATABASE parameter point, if the database file isn't exist, 
	//then create a new database file which name is the same as the strDATABASE
	//caution:the strDATABASE can't inclue chinese charaters, otherwise the database will be open failed.
	nErr = sqlite3_open(strDBFileName.c_str(), &m_pSqliteDB);
	if(0 == nErr)
	{
		assert(m_pSqliteDB);
		bRet = TRUE;
	}

	return bRet;
}

void CSqliteDBA::CloseDBA()
{
	if(m_pSqliteDB)
		sqlite3_close(m_pSqliteDB);
	m_pSqliteDB = NULL;
}

int CSqliteDBA::ExcuteSQL(LPCTSTR szSqlStr)
{
	int nRtBufLen = 0;
	string strSqlStrA;
	unicodeToUTF8(szSqlStr, strSqlStrA);

	assert(m_pSqliteDB);
	//if the data base is locked, then for a minute.
	sqlite3_busy_timeout(m_pSqliteDB,100);

	char *pszErrMsg = NULL;
	int nErr = sqlite3_exec(m_pSqliteDB, strSqlStrA.c_str(), 0, 0, &pszErrMsg);
	if(0 != nErr)
	{	
		sqlite3_free(pszErrMsg);
	}

	return nErr;
}

int CSqliteDBA::SelectSQL(LPCTSTR szSqlStr)
{
	string strSqlStrA;
	unicodeToUTF8(szSqlStr, strSqlStrA);

	//if user didn't call sqlite3_free_table, this can ensure sqlite3_free_table to be called
	CloseRecordset();
	
	char *pszErrMsg = NULL;
	int nErr = sqlite3_get_table(m_pSqliteDB, strSqlStrA.c_str(), &m_ppszResultSet, &m_nRowCnt, &m_nColCnt, &pszErrMsg);
	
	if (0 != nErr)
	{
		m_nRowCnt = 0;
		sqlite3_free(pszErrMsg);
	}
	else
	{
		assert(m_ppszResultSet);
	}
	
	return m_nRowCnt;
}

void CSqliteDBA::CloseRecordset()
{
	if(m_ppszResultSet)
		sqlite3_free_table(m_ppszResultSet);
	m_ppszResultSet = NULL;
	
	m_nCurResultPos = 0;
}

int CSqliteDBA::GetLastDBAErrno()
{
	return sqlite3_errcode(m_pSqliteDB);
}

BOOL CSqliteDBA::GetLastDBAError(LPTSTR strOutErrInfo, UINT unSizeInByte)
{
	BOOL bRet = FALSE;
	tstring strTemp;

	if (m_pSqliteDB)
		utf8ToUnicode(sqlite3_errmsg(m_pSqliteDB), strTemp);

	int nErrInfoLen = strTemp.length();
	if (nErrInfoLen > 0 && nErrInfoLen <= (int)unSizeInByte)
	{
		_stprintf_s(strOutErrInfo, unSizeInByte / sizeof(TCHAR), _T("%s"), strTemp.c_str());
		bRet = TRUE;
	}

	return bRet;
}

BOOL CSqliteDBA::GotoFirst()
{
	if(m_nRowCnt < 0)
		return FALSE;
	
	if(m_nRowCnt == 0)
		m_nCurResultPos = 0;
	else
		m_nCurResultPos = 1;//m_ppszResultSet[0] is not record, it is column name
	
	assert(m_nCurResultPos <= m_nRowCnt);
	return TRUE;
}

BOOL CSqliteDBA::GotoNext()
{
	BOOL bRet = TRUE;
	if(m_nCurResultPos == m_nRowCnt)
	{
		//it has be in last position, and cann't go to next again
		bRet = FALSE;
	}
	else
	{
		assert(m_nCurResultPos < m_nRowCnt);
		m_nCurResultPos++;
	}

	return bRet;
}

int CSqliteDBA::GetColIdxByColName(LPCTSTR szColName)
{
	if(!m_ppszResultSet)
		return -1;
	
	string strColNameA;
	unicodeToUTF8(szColName, strColNameA);

	int i = 0;
	for(i = 0;i < m_nColCnt;i++)
	{
		if(strcmp(strColNameA.c_str(), m_ppszResultSet[i]) == 0)
			return i;
	}
	
	return -1;
}

BOOL CSqliteDBA::GetFieldAsBool(LPCTSTR szColName)
{
	if(!m_ppszResultSet)
		return FALSE;
	
	if(m_nCurResultPos > m_nRowCnt)
		return FALSE;
	
	int nColIdx = GetColIdxByColName(szColName);
	if(nColIdx == -1)
		return FALSE;
	
	//here not check the column type, 
	char *pResult = m_ppszResultSet[m_nColCnt * m_nCurResultPos + nColIdx];
	if(NULL == pResult)
		return FALSE;

	int nResult = atoi(pResult);
	BOOL bRt = (BOOL)nResult;
	return bRt;
}

double CSqliteDBA::GetFieldAsDateTime(LPCTSTR szColName)
{
	if(!m_ppszResultSet)
		return FALSE;
	
	if(m_nCurResultPos > m_nRowCnt)
		return FALSE;
	
	int nColIdx = GetColIdxByColName(szColName);
	if(nColIdx == -1)
		return FALSE;
	
	int nYear = 0,nMonth = 0,nDay = 0,nHour = 0,nMin = 0,nSec = 0;
	
	//here not check the column type, 
	sscanf_s(m_ppszResultSet[m_nColCnt * m_nCurResultPos + nColIdx],"%d-%d-%d %d:%d:%d",
			&nYear,&nMonth,&nDay,&nHour,&nMin,&nSec);
	
	if(nYear < 1900 || nMonth <= 0 || nMonth > 12 || nDay <= 0 || nDay > 31 ||
		nHour < 0 || nHour > 23 || nMin < 0 || nMin > 59 ||nSec < 0 || nSec > 59)
		return FALSE;
	
	struct tm time = {0};
	time_t tTime = 0;
	time.tm_year=nYear-1900;//year. the value equal to the real year minus 1990
	time.tm_mon=nMonth-1;//month. start from Feb to Dec. 0 standard for Feb.the value range is:[0,11]
	time.tm_mday=nDay;//day. the value range is:[1,31]
	time.tm_hour=nHour;//hour. the value range is:[0,23]
	time.tm_min=nMin;//minute. the value range is:[0,59]
	time.tm_sec=nSec;//second. the value range is: [0,59]
	tTime=mktime(&time);
		
	return (double)tTime;
}

double CSqliteDBA::GetFieldAsDouble(LPCTSTR szColName)
{
	if(!m_ppszResultSet)
		return FALSE;
	
	if(m_nCurResultPos > m_nRowCnt)
		return FALSE;
	
	int nColIdx = GetColIdxByColName(szColName);
	if(nColIdx == -1)
		return FALSE;
	
	//here not check the column type, 
	return atof(m_ppszResultSet[m_nColCnt * m_nCurResultPos + nColIdx]);
}

int CSqliteDBA::GetFieldAsInt32(LPCTSTR szColName)
{
	if(!m_ppszResultSet)
		return FALSE;
	
	if(m_nCurResultPos > m_nRowCnt)
		return FALSE;
	
	int nColIdx = GetColIdxByColName(szColName);
	if(nColIdx == -1)
		return FALSE;
	
	//here not check the column type, 
	return atoi(m_ppszResultSet[m_nColCnt * m_nCurResultPos + nColIdx]);
}

UINT64 CSqliteDBA::GetFieldAsInt64(LPCTSTR szColName)
{
	if (!m_ppszResultSet)
		return FALSE;

	if (m_nCurResultPos > m_nRowCnt)
		return FALSE;

	int nColIdx = GetColIdxByColName(szColName);
	if (nColIdx == -1)
		return FALSE;

	//here not check the column type, 
	return atoll(m_ppszResultSet[m_nColCnt * m_nCurResultPos + nColIdx]);
}

tstring CSqliteDBA::GetFieldAsString(LPCTSTR szColName)
{
	tstring strOutVal;
	char strVal[MAX_FIELD_STR_LEN] = { 0 };

	int nColIdx = GetColIdxByColName(szColName);

	if (nColIdx >= 0)
	{
		char* pChVal = m_ppszResultSet[m_nColCnt * m_nCurResultPos + nColIdx];
		strcpy_s(strVal, strlen(pChVal)+1, pChVal);
		utf8ToUnicode(strVal, strOutVal);
	}

	return strOutVal;
}
