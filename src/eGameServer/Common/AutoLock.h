#ifndef _AUTOLOCK_H_
#define _AUTOLOCK_H_


#ifdef COMMON_EXPORTS
#define AUTOLOCK_API __declspec(dllexport)
#else
#define AUTOLOCK_API __declspec(dllimport)
#endif

class AUTOLOCK_API CAutoLock
{
	LPCRITICAL_SECTION m_pLock;
public:

	CAutoLock(LPCRITICAL_SECTION lpLock);
	virtual ~CAutoLock(void);
};

#endif