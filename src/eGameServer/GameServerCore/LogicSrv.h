#pragma once

#include "UserData.h"
#include "BaseSrv.h"

class CLogicSrv : public CBaseSrv
{
public:
	CLogicSrv();
	virtual ~CLogicSrv();

protected:
	LRESULT LinkMgrProc(UINT unAccSocket);
	void Destroy();

private:
};
