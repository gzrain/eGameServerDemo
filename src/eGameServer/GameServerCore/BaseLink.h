#pragma once

#include <windows.h>
#include "Protocol.h"
#include "SafeRelease.h"
#include "JumpUtil.h"
#include "MemOP.h"
#include <assert.h>
#include <tchar.h>
#include "DBMgr.h"

//以下头文件，仅为了终端日志打印使用
#include <iostream>
using namespace std;

#define MAX_RECV_BUF_LEN 2048

#define ERR_NONE								0
#define ERR_SOCK_RECV_FAILED			ERR_NONE - 1
#define ERR_SOCK_SEND_FAILED			ERR_NONE - 2

class CBaseLink
{
public:
	CBaseLink();
	virtual ~CBaseLink();

public:
	LRESULT Init(void *pParent, UINT hSocket,
		DWORD(*pErrProc)(void* pParent, void* argv, LRESULT lErrCode));
	LRESULT UnInit();

	BOOL StartRecv();
	BOOL StopRecv();

	inline void* GetParent(){ return m_pParent; }

protected:

	LRESULT RecvRawPkg(char* pRecvBuf, int* pnRecvLen);

	//纯虚函数，子类具体实现
	//主要功能：剔除头部信息，取出真实数据
	virtual LRESULT ProcessRawData(const MSGHEAD* pstHead, const char* pDataBuf, int nDataLen) = 0;			

	LRESULT ResolveMsgHead(const char* pRecvBuf, MSGHEAD* pMsgHeadOut);

	BOOL SockSend(const char *pSendData, long lDataLen);
	BOOL SockRecv(char *pRecvData, long lDataLen);

	LRESULT SendReply(DWORD dwMsgType, const void* pDataBuf, int nDataLen);

	static int GetSockRecvDataLen(int sock, u_long *argp);
	static DWORD  WINAPI BaseLink_RecvThreadProc(LPVOID lpParameter);			//接收数据线程

protected:
	void  						*m_pParent;

	// 用于 服务器 与数据库交互
	CDBMgr					m_DBMgr;

private:
	UINT 						m_hSocket;
	HANDLE 					m_hRecvThread;
	BOOL volatile 			m_bRecvThreadRun;

	HANDLE					m_hRecvStart;
	HANDLE					m_hRecvStop;
	HANDLE					m_hRecvExit;

	DWORD(*m_pErrProc)(void* pParent, void* argv, LRESULT lErrCode);

	};
