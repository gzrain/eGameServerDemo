#pragma once

#include "UserData.h"
#include "BaseSrv.h"

typedef vector<CUserData*> ONLINE_USER_ARRAY;
typedef ONLINE_USER_ARRAY::iterator ONLINE_USERARRAY_ITER;

class CLoginSrv : public CBaseSrv
{
public:
	CLoginSrv();
	virtual ~CLoginSrv();

	LRESULT AddToOnlineList(const COS_GUID *guidUser);
	LRESULT DeleteFromOnlineList(const COS_GUID *guidUser);
	BOOL IsUserOnLine(const COS_GUID *guidUser);

protected:
	LRESULT LinkMgrProc(UINT unAccSocket);
	void Destroy();

private:
	ONLINE_USER_ARRAY m_vecOnlineUserArray;
	CRITICAL_SECTION 		m_csUserArray;				//多线程操作数组资源，加锁保护
};
