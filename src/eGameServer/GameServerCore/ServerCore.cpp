#include "ServerCore.h"


CServerCore::CServerCore()
{
}

CServerCore::~CServerCore()
{
}

LRESULT CServerCore::InitLoginServer(UINT unSerIP, UINT unSerPort)
{
	return m_LoginSrv.Init(this, unSerIP, unSerPort);
}

LRESULT CServerCore::InitLogicServer(UINT unSerIP, UINT unSerPort)
{
	return m_LogicSrv.Init(this, unSerIP, unSerPort);
}

LRESULT CServerCore::InitBillingServer(UINT unSerIP, UINT unSerPort)
{
	return m_BillingSrv.Init(this, unSerIP, unSerPort);
}

LRESULT CServerCore::UnInit()
{
	 m_LoginSrv.UnInit();
	 m_LogicSrv.UnInit();
	 m_BillingSrv.UnInit();

	 return S_OK;
}
