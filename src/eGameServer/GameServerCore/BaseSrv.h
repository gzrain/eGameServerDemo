#ifndef _BASE_SRV_H_
#define _BASE_SRV_H_

#include <windows.h>
#include "BaseLink.h"
#include "SafeRelease.h"
#include "JumpUtil.h"
#include "AutoLock.h"

#include <vector>
using namespace std;

typedef vector<CBaseLink*> LINK_ARRAY;
typedef LINK_ARRAY::iterator	 LINK_ITER;

class CBaseSrv
{
	friend DWORD ErrProc_CB(void* pParent, void* argv, LRESULT lErrCode);

public:
	CBaseSrv();
	~CBaseSrv();

public:
	LRESULT Init(void* pParent, UINT unSerIP, UINT unSerPort);
	LRESULT UnInit();

protected:
	//���麯�����������ʵ��
	virtual LRESULT LinkMgrProc(UINT unAccSocket) = 0;
	
	// �رճ���ʱ��������Դ
	virtual void Destroy();	

	LRESULT InitWinSocket(UINT *punSocket, UINT unIP, UINT16 unSerPort);
	DWORD ListenThreadProcPrivate();
	static DWORD  WINAPI ListenThreadProc(LPVOID lpParameter);			//���������߳�

protected:
	LINK_ARRAY				m_vecLinkArray;
	void*						m_pParent;
	CRITICAL_SECTION 		m_csLinkArray;

private:
	UINT						m_unSocket;
	BOOL						m_bWSAStartup;
	HANDLE 					m_hListenThread;
	BOOL volatile 			m_bListenThreadRun;

	HANDLE					m_hStart;
	HANDLE					m_hExit;
	HANDLE					m_hStop;

};

#endif
