#pragma once

#include <windows.h>
#include "Protocol.h"
#include "SafeRelease.h"
#include "JumpUtil.h"
#include "MemOP.h"
#include <assert.h>
#include <tchar.h>
#include "DBMgr.h"

//����ͷ�ļ�����Ϊ���ն���־��ӡʹ��
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

	//���麯�����������ʵ��
	//��Ҫ���ܣ��޳�ͷ����Ϣ��ȡ����ʵ����
	virtual LRESULT ProcessRawData(const MSGHEAD* pstHead, const char* pDataBuf, int nDataLen) = 0;			

	LRESULT ResolveMsgHead(const char* pRecvBuf, MSGHEAD* pMsgHeadOut);

	BOOL SockSend(const char *pSendData, long lDataLen);
	BOOL SockRecv(char *pRecvData, long lDataLen);

	LRESULT SendReply(DWORD dwMsgType, const void* pDataBuf, int nDataLen);

	static int GetSockRecvDataLen(int sock, u_long *argp);
	static DWORD  WINAPI BaseLink_RecvThreadProc(LPVOID lpParameter);			//���������߳�

protected:
	void  						*m_pParent;

	// ���� ������ �����ݿ⽻��
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
