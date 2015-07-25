// ServerConsoleDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Protocol.h"
#include "ServerCore.h"
#include <iostream>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

CServerCore m_ServerCore;
char m_szSerIP[16] = { "127.0.0.1" };

int _tmain(int argc, _TCHAR* argv[])
{
	//初始化
	int nResult;
	UINT unSerIP = inet_addr(m_szSerIP);

	nResult = m_ServerCore.InitBillingServer(unSerIP, SRV_PORT_BILLING);
	if (nResult < 0)
	{
		wcout << _T("初始化BillingServer失败，错误码：") << nResult << endl;
		return nResult;
	}

	nResult = m_ServerCore.InitLoginServer(unSerIP, SRV_PORT_LOGIN);
	if (nResult < 0)
	{
		wcout << ("初始化LoginServer失败，错误码： ") << nResult << endl;
		return nResult;
	}

	nResult = m_ServerCore.InitLogicServer(unSerIP, SRV_PORT_LOGIC);
	if (nResult < 0)
	{
		wcout << ("初始化LogicServer失败，错误码： ") << nResult << endl;
		return nResult;
	}
	wcout << ("===========================================" )<< endl;
	wcout << ("初始化eGameServer成功") << endl;
	wcout << ("eGameServer已开始运行，按‘q’回车退出\n") << endl;



	//控制程序退出
	while (TRUE)
	{
		char szKey = 0;
		scanf_s("%c\n", &szKey);
		
		if (szKey == 'q')
		{
			cout << "正在退出Server，请等待………………" << endl;
			break;
		}
	}


	//销毁资源，及释放内存空间
	m_ServerCore.UnInit();
	cout << "退出Server" << endl;
	cout << "=============================" << endl;
	return 0;
}

int Init()
{
	int nResult;
	UINT unSerIP = inet_addr(m_szSerIP);

	nResult = m_ServerCore.InitBillingServer(unSerIP, SRV_PORT_BILLING);
	if (nResult < 0)
	{
		cout << "初始化BillingServer失败，错误码： " << nResult << endl;
		return nResult;
	}

	nResult = m_ServerCore.InitLoginServer(unSerIP, SRV_PORT_BILLING);
	if (nResult < 0)
	{
		cout << "初始化LoginServer失败，错误码： " << nResult << endl;
		return nResult;
	}

	nResult = m_ServerCore.InitLogicServer(unSerIP, SRV_PORT_BILLING);
	if (nResult < 0)
	{
		cout << "初始化LogicServer失败，错误码： " << nResult << endl;

		return nResult;
	}

	cout << "初始化Server成功" << endl;

	return nResult;
}

int UnInit()
{
	m_ServerCore.UnInit();
	cout << "退出Server" << endl;

	return 0;
}