// UserData.h
// by gaozhe
// 20150702

#include "UserData.h"
#include <assert.h>

CUserData::CUserData()
{
	memset(&m_stUserData, 0, sizeof(m_stUserData));
	memset(&m_stUserGameProperty, 0, sizeof(m_stUserGameProperty));

}


CUserData::~CUserData()
{
}

void CUserData::SetUserAccount(LPUSER_ACCOUNT_T pstUserData)
{
	assert(pstUserData);

	memcpy(&m_stUserData, pstUserData, sizeof(m_stUserData));
}

LRESULT CUserData::GetUserAccount(LPUSER_ACCOUNT_T pstUserDataOut)
{
	LRESULT hr = E_FAIL;
	
	// if guid is null , return E_FAIL

	if (GUID_NULL != m_stUserData.guidUser)
	{
		pstUserDataOut = &m_stUserData;
		hr = S_OK;
	}

	return hr;
}

unsigned short CUserData::GetUserLevel()
{
	return m_stUserGameProperty.unUserLevel;
}

LRESULT CUserData::SetUserLevel(unsigned short nLevel)
{
	LRESULT hr = E_FAIL;

	if (nLevel <= MAX_USER_LEVEL)
	{
		m_stUserGameProperty.unUserLevel = nLevel;
		hr = S_OK;
	}

	return hr;
}

// CBagMgr* CUserData::GetUserBag()
// {
// 	return m_stUserGameProperty.pBagMgr;
// }
// 
// LRESULT CUserData::SetUserBag(const CBagMgr* pBagMgr)
// {
// 	assert(pBagMgr);
// 
// 	m_stUserGameProperty.pBagMgr = (CBagMgr *)pBagMgr;
// 
// 	return S_OK;
// }
// 
// CUserCoordinate *CUserData::GetUserCoordInfo()
// {
// 	return m;
// }
// 
// LRESULT CUserData::SetUserCoordInfo(const CUserCoordinate* pUserCoordInfo)
// {
// 	assert(pUserCoordInfo);
// 
// 	m_stUserGameProperty.pCoordInfo = (CUserCoordinate*)pUserCoordInfo;
// 
// 	return S_OK;
// }

