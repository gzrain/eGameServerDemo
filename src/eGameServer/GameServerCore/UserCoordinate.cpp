#include "UserCoordinate.h"

CUserCoordinate::CUserCoordinate()
{
}

CUserCoordinate::~CUserCoordinate()
{
}

LRESULT CUserCoordinate::GetUserCoodinfo(USER_COORDINATE_INFO_T &stCoordInfo)
{
	LRESULT hr = E_FAIL;
	if (m_stCoordInfo.enumGameMap != GAME_MAP_START)
	{
		stCoordInfo = m_stCoordInfo;
		hr = S_OK;
	}

	return hr;
}

LRESULT CUserCoordinate::SetUserCoordInfo(LPUSER_COORDINATE_INFO_T lpCoordInfo)
{
	LRESULT hr = E_FAIL;
	if (lpCoordInfo->enumGameMap > GAME_MAP_START &&
		lpCoordInfo->enumGameMap < GAME_MAP_END)
	{
		m_stCoordInfo = lpCoordInfo;
		hr = S_OK;
	}

	return hr;
}