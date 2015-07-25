#include "StdAfx.h"
#include "AutoLock.h"

CAutoLock::CAutoLock(LPCRITICAL_SECTION lpLock)
{
	m_pLock=lpLock;
	EnterCriticalSection(lpLock);
}

CAutoLock::~CAutoLock(void)
{
	LeaveCriticalSection(m_pLock);
}
