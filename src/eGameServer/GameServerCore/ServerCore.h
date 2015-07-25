#ifndef _SERVER_CORE_H_
#define _SERVER_CORE_H_

#ifdef GAMESERVERCORE_EXPORTS
#define SERVERCORE_API __declspec(dllexport)
#else
#define SERVERCORE_API __declspec(dllimport)
#endif

#include "LoginSrv.h"
#include "LogicSrv.h"
#include "BillingSrv.h"

class SERVERCORE_API  CServerCore
{
public:
	CServerCore();
	~CServerCore();

public:
	LRESULT InitLoginServer(UINT unSerIP, UINT unSerPort);
	LRESULT InitLogicServer( UINT unSerIP, UINT unSerPort);
	LRESULT InitBillingServer(UINT unSerIP, UINT unSerPort);

	LRESULT UnInit();

private:
	CLoginSrv m_LoginSrv;
	CLogicSrv m_LogicSrv;
	CBillingSrv m_BillingSrv;
};

#endif