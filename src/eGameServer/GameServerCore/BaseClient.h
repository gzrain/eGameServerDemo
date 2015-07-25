#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_


#define ERR_NONE					0
#define ERR_CLIENT					-1000
#define ERR_CLIENT_SENDCMD_FAILED			ERR_CLIENT - 1

#define ERR_CLIENT_SER_REPLY_TRUE				ERR_NONE
#define ERR_CLIENT_SER_REPLY_FALSE			ERR_CLIENT - 2
#define ERR_CLIENT_SER_REPLY_NOREPLY		ERR_CLIENT - 3

#include <windows.h>
#include "UserData.h"
#include <assert.h>
#include "JumpUtil.h"
#include "SafeRelease.h"
#include "MemOP.h"
#include <tchar.h>

class  CBaseClient
{
public:
	CBaseClient();
	virtual ~CBaseClient();

public:
	LRESULT Init(LPCSTR pszSerIPStr, UINT unSerPort, UINT unStaticLocalPort);
	virtual LRESULT UnInit();
	BOOL IsConnected() { return m_bServerConnected; };

protected:
	LRESULT ConnectToServer();

	LRESULT SendCmd(DWORD dwCmdType, const char* pDataBuf, long lDataLen);

	LRESULT InitWinSocket(LPCSTR pszSerIPStr, UINT unSerPort, UINT unStaticLocalPort);
	BOOL SockSend(const char *pSendData, long lDataLen);
	BOOL SockRecv(char *pSendData, long lDataLen);

	LRESULT GetTimeString(const time_t *ptmSrc, LPSTR lpstrReturn, DWORD dwSizeInWords);

private:
	char						m_szSerIPStr[16];
	UINT					m_unSerPort;
	UINT					m_unStaticLocalPort;

	UINT 					m_hSocket;
	BOOL					m_bWSAStartup;
	void  					*m_pParent;
	BOOL					m_bServerConnected;

};


#endif