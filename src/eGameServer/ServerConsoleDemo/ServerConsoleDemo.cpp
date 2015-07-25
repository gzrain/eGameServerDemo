// ServerConsoleDemo.cpp : �������̨Ӧ�ó������ڵ㡣
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
	//��ʼ��
	int nResult;
	UINT unSerIP = inet_addr(m_szSerIP);

	nResult = m_ServerCore.InitBillingServer(unSerIP, SRV_PORT_BILLING);
	if (nResult < 0)
	{
		wcout << _T("��ʼ��BillingServerʧ�ܣ������룺") << nResult << endl;
		return nResult;
	}

	nResult = m_ServerCore.InitLoginServer(unSerIP, SRV_PORT_LOGIN);
	if (nResult < 0)
	{
		wcout << ("��ʼ��LoginServerʧ�ܣ������룺 ") << nResult << endl;
		return nResult;
	}

	nResult = m_ServerCore.InitLogicServer(unSerIP, SRV_PORT_LOGIC);
	if (nResult < 0)
	{
		wcout << ("��ʼ��LogicServerʧ�ܣ������룺 ") << nResult << endl;
		return nResult;
	}
	wcout << ("===========================================" )<< endl;
	wcout << ("��ʼ��eGameServer�ɹ�") << endl;
	wcout << ("eGameServer�ѿ�ʼ���У�����q���س��˳�\n") << endl;



	//���Ƴ����˳�
	while (TRUE)
	{
		char szKey = 0;
		scanf_s("%c\n", &szKey);
		
		if (szKey == 'q')
		{
			cout << "�����˳�Server����ȴ�������������" << endl;
			break;
		}
	}


	//������Դ�����ͷ��ڴ�ռ�
	m_ServerCore.UnInit();
	cout << "�˳�Server" << endl;
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
		cout << "��ʼ��BillingServerʧ�ܣ������룺 " << nResult << endl;
		return nResult;
	}

	nResult = m_ServerCore.InitLoginServer(unSerIP, SRV_PORT_BILLING);
	if (nResult < 0)
	{
		cout << "��ʼ��LoginServerʧ�ܣ������룺 " << nResult << endl;
		return nResult;
	}

	nResult = m_ServerCore.InitLogicServer(unSerIP, SRV_PORT_BILLING);
	if (nResult < 0)
	{
		cout << "��ʼ��LogicServerʧ�ܣ������룺 " << nResult << endl;

		return nResult;
	}

	cout << "��ʼ��Server�ɹ�" << endl;

	return nResult;
}

int UnInit()
{
	m_ServerCore.UnInit();
	cout << "�˳�Server" << endl;

	return 0;
}