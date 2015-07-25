
#include "stdafx.h"
#include "CommonDBA.h"
#include "BaseDBA.h"
#include "MySqlDBA.h"
#include "SqliteDBA.h"
#include "assert.h"
#include <time.h>
#include "SafeRelease.h"
#include "JumpUtil.h"

#define INT_TIME_OUT		10

CComnDBAMgr::CComnDBAMgr()
:m_pDBA(NULL)
{
}

CComnDBAMgr::~CComnDBAMgr()
{
	CloseDBA();
}

LRESULT CComnDBAMgr::OpenDBA(LPCTSTR strIP /* = _T("")  */, 
	LPCTSTR strDBName /* = _T("eGameServerDB")  */, 
	LPCTSTR strUserName /* = _T("sa")  */, 
	LPCTSTR strPassword /* = _T("123456")  */, 
	UINT nDBAType /* = DBA_TYPE_MYSQL  */,
	LPTSTR pstrErrOut /* = NULL  */,
	UINT unErrSizeInByte /* = 0 */)
{
	BOOL bRes = FALSE;
	LRESULT hr = S_OK;
	m_nDBAType = nDBAType;

	if (nDBAType == DBA_TYPE_MYSQL)
		m_pDBA = new CMySqlDBA;
	else
	{
		m_pDBA = new CSqliteDBA;
	}

	assert(m_pDBA);
	bRes = m_pDBA->OpenDBA(strIP, strDBName, strUserName, strPassword, INT_TIME_OUT, pstrErrOut, unErrSizeInByte);

	if (!bRes)
	{	
		hr = E_FAIL;
	}

	return hr;
}

long CComnDBAMgr::CloseDBA()
{
	if (m_pDBA)
	{
		m_pDBA->CloseRecordset();
		m_pDBA->CloseDBA();
		SAFE_DELETE(m_pDBA);
	}
	return 0;
}

LRESULT CComnDBAMgr::GetFieldAsString(LPCTSTR szColumn, LPTSTR pszOut, UINT unSizeInWords)
{
	int nErr = -1;
	tstring szTemp = _T("");
	szTemp = m_pDBA->GetFieldAsString(szColumn);
	if (unSizeInWords > szTemp.length())
	{
		_stprintf_s(pszOut, unSizeInWords, _T("%s"), szTemp.c_str());
		nErr = 0;
	}
	else
	{
		OutputDebugString(_T("Error: pszOut buf length is too short"));
	}
	return nErr;
}

LRESULT CComnDBAMgr::GetFieldAsString(LPCTSTR szColName, tstring &strOut)
{
	int nErr = 0;
	strOut = _T("");
	strOut = m_pDBA->GetFieldAsString(szColName);
	return nErr;
}

BOOL CComnDBAMgr::GetFieldAsBool(LPCTSTR szColumn)//´íÎó·µ»Øfalse
{
	BOOL bResult=0;
	bResult = m_pDBA->GetFieldAsBool(szColumn);
	
	return bResult;
}

int CComnDBAMgr::GetFieldAsInt32(LPCTSTR szColumn)//´íÎó·µ»Ø-1
{
	return 	m_pDBA->GetFieldAsInt32(szColumn);
}

UINT64 CComnDBAMgr::GetFieldAsInt64(LPCTSTR szColumn)
{
	return m_pDBA->GetFieldAsInt64(szColumn);
}
double CComnDBAMgr::GetFieldAsDouble(LPCTSTR szColumn)//´íÎó·µ»Ø-1
{
	return 	m_pDBA->GetFieldAsDouble(szColumn);
}

long CComnDBAMgr::GetFieldAsDateTime(LPCTSTR szColumn)
{
	return (long)m_pDBA->GetFieldAsDateTime(szColumn);
}

LRESULT CComnDBAMgr::MoveToNext()
{
	if(m_pDBA->GotoNext())
		return 0;
	return -1;
}

LRESULT CComnDBAMgr::MoveToFirst()
{
	if(m_pDBA->GotoFirst())
		return 0;
	return -1;
}

void CComnDBAMgr::InitSelectTask()
{

}

void CComnDBAMgr::FinishSelectTask()
{
	m_pDBA->CloseRecordset();
}

LRESULT CComnDBAMgr::ExcuteSQL(LPCTSTR SqlStr)
{
	LRESULT hr = S_OK;
	int nResult = m_pDBA->ExcuteSQL(SqlStr);

	if (0 != nResult)
		hr = E_FAIL;

	return hr;
}

int CComnDBAMgr::SelectSQL(LPCTSTR SqlStr)
{
	return m_pDBA->SelectSQL(SqlStr);
}

BOOL CComnDBAMgr::GetLastDBAError(LPTSTR strOutErrInfo, UINT nSizeInByte)
{
	return m_pDBA->GetLastDBAError(strOutErrInfo, nSizeInByte);
}

int CComnDBAMgr::GetLastDBAErrno()
{
	return m_pDBA->GetLastDBAErrno();
}