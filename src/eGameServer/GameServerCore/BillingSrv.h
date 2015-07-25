#pragma once

#include "UserData.h"
#include "BaseSrv.h"

class CBillingSrv : public CBaseSrv
{
public:
	CBillingSrv();
	virtual ~CBillingSrv();

protected:
	LRESULT LinkMgrProc(UINT unAccSocket);
	void Destroy();

private:
};