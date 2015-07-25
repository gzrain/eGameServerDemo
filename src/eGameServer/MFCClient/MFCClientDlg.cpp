
// MFCClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCClient.h"
#include "MFCClientDlg.h"
#include "afxdialogex.h"

#include "JumpUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCClientDlg �Ի���



CMFCClientDlg::CMFCClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	memset(&m_stUserProperty, 0, sizeof(m_stUserProperty));
	memset(&m_stUserCoord, 0, sizeof(m_stUserCoord));
	memset(&m_stUserAccount, 0, sizeof(m_stUserAccount));
}

void CMFCClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editAccount);
	DDX_Control(pDX, IDC_EDIT3, m_editPswd);
	DDX_Control(pDX, IDC_EDIT2, m_editName);
	DDX_Control(pDX, IDC_EDIT4, m_editAlias);
	DDX_Control(pDX, IDC_STATIC1, m_staticLevel);
	DDX_Control(pDX, IDC_STATIC2, m_staticBagType);
	DDX_Control(pDX, IDC_STATIC3, m_staticMoney);
	DDX_Control(pDX, IDC_STATIC4, m_staticCoordx);
	DDX_Control(pDX, IDC_STATIC5, m_staticCoordY);
	DDX_Control(pDX, IDC_STATIC6, m_staticMap);
	DDX_Control(pDX, IDC_LIST2, m_listShop);
	DDX_Control(pDX, IDC_LIST3, m_listBag);
	DDX_Control(pDX, IDC_STATIC_GOODSINFO, m_staticGoodsInfo);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ipServer);
}

BEGIN_MESSAGE_MAP(CMFCClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_BUY, &CMFCClientDlg::OnBnClickedBtnBuy)
	ON_BN_CLICKED(IDC_BTN_SELL, &CMFCClientDlg::OnBnClickedBtnSell)
	ON_BN_CLICKED(IDC_BTN_DROP, &CMFCClientDlg::OnBnClickedBtnDrop)
	ON_BN_CLICKED(IDC_BTN_LOGOUT, &CMFCClientDlg::OnBnClickedBtnLogout)
	ON_BN_CLICKED(IDC_BTN_LOGIN, &CMFCClientDlg::OnBnClickedBtnLogin)
	ON_BN_CLICKED(IDC_BTN_SIGNUP, &CMFCClientDlg::OnBnClickedBtnSignup)
	ON_BN_CLICKED(IDC_BTN_LEVEL_UP, &CMFCClientDlg::OnBnClickedBtnLevelUp)
	ON_BN_CLICKED(IDC_BTN_BAG_BUP, &CMFCClientDlg::OnBnClickedBtnBagBup)
	ON_BN_CLICKED(IDC_BTN_MONEY_UP, &CMFCClientDlg::OnBnClickedBtnMoneyUp)
	ON_BN_CLICKED(IDC_BTN_GETBASE, &CMFCClientDlg::OnBnClickedBtnGetbase)
	ON_BN_CLICKED(IDC_BTN_SETBASE, &CMFCClientDlg::OnBnClickedBtnSetbase)
	ON_BN_CLICKED(IDC_BTN_GETCOORD, &CMFCClientDlg::OnBnClickedBtnGetcoord)
	ON_BN_CLICKED(IDC_BTN_MOVEUP, &CMFCClientDlg::OnBnClickedBtnMoveup)
	ON_BN_CLICKED(IDC_BTN_MOVElEFT, &CMFCClientDlg::OnBnClickedBtnMoveleft)
	ON_BN_CLICKED(IDC_BTN_MOVERIGHT, &CMFCClientDlg::OnBnClickedBtnMoveright)
	ON_BN_CLICKED(IDC_BTN_MOVEBOTOOM, &CMFCClientDlg::OnBnClickedBtnMovebotoom)
	ON_BN_CLICKED(IDC_BTN_MAP_UP, &CMFCClientDlg::OnBnClickedBtnMapUp)
	ON_BN_CLICKED(IDC_BTN_MAP_DOWN, &CMFCClientDlg::OnBnClickedBtnMapDown)
	ON_BN_CLICKED(IDC_BTN_SETCOORD, &CMFCClientDlg::OnBnClickedBtnSetcoord)
	ON_BN_CLICKED(IDC_BTN_UPDATEBAG, &CMFCClientDlg::OnBnClickedBtnUpdatebag)
	ON_BN_CLICKED(IDC_BTN_UPDATESHOP, &CMFCClientDlg::OnBnClickedBtnUpdateshop)
	ON_LBN_SELCHANGE(IDC_LIST2, &CMFCClientDlg::OnLbnSelchangeList2)
	ON_LBN_SELCHANGE(IDC_LIST3, &CMFCClientDlg::OnLbnSelchangeList3)
END_MESSAGE_MAP()


// CMFCClientDlg ��Ϣ�������

BOOL CMFCClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	FreshCtrl();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	LRESULT hr = S_OK;
	USER_ACCOUNT_T stUserInfo;
	USER_COORDINATE_INFO_T stUserCoordInfo;

	char strSerIP[16] = { "127.0.0.1" };
	hr = m_Client.Init(strSerIP, strSerIP, strSerIP);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMFCClientDlg::FreshCtrl()
{
	CString str;

	str.Format(_T("%d"), m_stUserProperty.unBagType);
	m_staticBagType.SetWindowText(str);
	str.Format(_T("%d"), m_stUserProperty.unUserLevel);
	m_staticLevel.SetWindowText(str);
	str.Format(_T("%.2f"), m_stUserProperty.fUserMoney);
	m_staticMoney.SetWindowText(str);

	str.Format(_T("%d"), m_stUserCoord.nCoordX);
	m_staticCoordx.SetWindowText(str);
	str.Format(_T("%d"), m_stUserCoord.nCoordY);
	m_staticCoordY.SetWindowText(str);
	str.Format(_T("%d"), (int)m_stUserCoord.enumGameMap);
	m_staticMap.SetWindowText(str);

	str.Format(_T("%s"), m_stUserAccount.szUserName);
	m_editName.SetWindowText(str);
	str.Format(_T("%s"), m_stUserAccount.szUserAlias);
	m_editAlias.SetWindowText(str);

	m_ipServer.SetAddress(ntohl(inet_addr("127.0.0.1")));
}

void CMFCClientDlg::GetCtrlVal()
{
	CString str;
	int nVal = 0;
	float fVal = 0;
	m_staticBagType.GetWindowText(str);
	_stscanf_s(str.GetString(), _T("%d"), &nVal);
	m_stUserProperty.unBagType = nVal;

	m_staticLevel.GetWindowText(str);
	_stscanf_s(str.GetString(), _T("%d"), &nVal);
	m_stUserProperty.unUserLevel = nVal;

	m_staticMoney.GetWindowText(str);
	_stscanf_s(str.GetString(), _T("%f"), &fVal);
	m_stUserProperty.fUserMoney = fVal;

	m_staticCoordx.GetWindowText(str);
	_stscanf_s(str.GetString(), _T("%d"), &nVal);
	m_stUserCoord.nCoordX = nVal;

	m_staticCoordY.GetWindowText(str);
	_stscanf_s(str.GetString(), _T("%d"), &nVal);
	m_stUserCoord.nCoordY = nVal;

	m_staticMap.GetWindowText(str);
	_stscanf_s(str.GetString(), _T("%d"), &nVal);
	m_stUserCoord.enumGameMap= (GAME_MAP_ENUM)nVal;

}

void CMFCClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFCClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCClientDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	// TODO:  �ڴ˴������Ϣ����������
}

void CMFCClientDlg::OnBnClickedBtnBuy()
{
	if (m_stUserAccount.guidUser == GUID_NULL)
	{
		MessageBox(_T("���ȵ�¼����ע��"));
		return;
	}

	int nCurSel = m_listShop.GetCurSel();
	if (nCurSel < 0)
	{
		MessageBox(_T("δѡ����Ʒ"));
		return;
	}
	GOODS_COMMON_INFO_T stGoodsInfo = { 0 };

	m_ShopMgr.GetGoodsInfoByIdx(nCurSel, &stGoodsInfo);

	LRESULT hr = m_Client.BuyGoods(&stGoodsInfo.guidGoods, 1);
	if (S_OK != hr)
	{
		MessageBox(_T("����ʧ��"));
		return;
	}
	MessageBox(_T("����ɹ�����ˢ�±���"));

}


void CMFCClientDlg::OnBnClickedBtnSell()
{
	if (m_stUserAccount.guidUser == GUID_NULL)
	{
		MessageBox(_T("���ȵ�¼����ע��"));
		return;
	}

	int nCurSel = m_listBag.GetCurSel();
	if (nCurSel < 0)
	{
		MessageBox(_T("δѡ����Ʒ"));
		return;
	}
	GOODS_COMMON_INFO_T stGoodsInfo = { 0 };

	m_UserBagMgr.GetGoodsInfoByIdx(nCurSel, &stGoodsInfo);

	LRESULT hr = m_Client.SellGoods(&stGoodsInfo.guidGoods, 1);
	if (S_OK != hr)
	{
		MessageBox(_T("����ʧ��"));
		return;
	}
	MessageBox(_T("���۳ɹ�����ˢ�±���"));

}


void CMFCClientDlg::OnBnClickedBtnDrop()
{
	if (m_stUserAccount.guidUser == GUID_NULL)
	{
		MessageBox(_T("���ȵ�¼����ע��"));
		return;
	}

	int nCurSel = m_listBag.GetCurSel();
	if (nCurSel < 0)
	{
		MessageBox(_T("δѡ����Ʒ"));
		return;
	}
	GOODS_COMMON_INFO_T stGoodsInfo = { 0 };

	m_UserBagMgr.GetGoodsInfoByIdx(nCurSel, &stGoodsInfo);

	LRESULT hr = m_Client.DropGoods(&stGoodsInfo.guidGoods, 1);
	if (S_OK != hr)
	{
		MessageBox(_T("����ʧ��"));
		return;
	}
	MessageBox(_T("�����ɹ�����ˢ�±���"));

}


void CMFCClientDlg::OnBnClickedBtnLogout()
{
	if (m_stUserAccount.guidUser == GUID_NULL)
	{
		MessageBox(_T("���ȵ�¼����ע��"));
		return;
	}

	m_Client.LogOut();

	ZeroMemory(&m_stUserAccount, sizeof(m_stUserAccount));
}


void CMFCClientDlg::OnBnClickedBtnLogin()
{
	CString strAcc;
	CString strPswd;

	m_editAccount.GetWindowText(strAcc);
	m_editPswd.GetWindowText(strPswd);

	strAcc.Trim();
	strPswd.Trim();
	LRESULT hr = m_Client.Login(strAcc.GetString(), strPswd.GetString());

	if (S_OK != hr)
	{
		MessageBox(_T("��¼ʧ��"));
		return;
	}
	m_Client.GetUserAccountInfo(&m_stUserAccount);
	FreshCtrl();
	MessageBox(_T("��¼�ɹ�"));
}


void CMFCClientDlg::OnBnClickedBtnSignup()
{
	CString strAcc;
	CString strPswd;
	CString strName;
	CString strAlias;

	m_editAccount.GetWindowText(strAcc);
	m_editPswd.GetWindowText(strPswd);
	m_editName.GetWindowText(strName);
	m_editAlias.GetWindowText(strAlias);

	LRESULT hr = m_Client.SignUp(strAcc.GetString(), strPswd.GetString(), strName.GetString(), strAlias.GetString());
	if (S_OK != hr)
	{
		MessageBox(_T("ע��ʧ��"));
		return;
	}
	MessageBox(_T("ע��ɹ�"));

}


void CMFCClientDlg::OnBnClickedBtnLevelUp()
{
	if (m_stUserProperty.unUserLevel < MAX_USER_LEVEL)
	{
		m_stUserProperty.unUserLevel++;
	}
	else
	{
		MessageBox(_T("�ѵ�����"));
		return;
	}
	FreshCtrl();
}


void CMFCClientDlg::OnBnClickedBtnBagBup()
{
	if (m_stUserProperty.unBagType < E_BAG_TYPE_END - 1)
	{
		m_stUserProperty.unBagType++;
	}
	else
	{
		MessageBox(_T("�ѵ����"));
		return;
	}
	FreshCtrl();
}

void CMFCClientDlg::OnBnClickedBtnMoneyUp()
{
	if (m_stUserProperty.fUserMoney < MAX_USER_MONEY)
	{
		m_stUserProperty.fUserMoney += 100;
	}
	else
	{
		MessageBox(_T("����"));
		return;
	}
	FreshCtrl();
}


void CMFCClientDlg::OnBnClickedBtnGetbase()
{
	if (m_stUserAccount.guidUser == GUID_NULL)
	{
		MessageBox(_T("���ȵ�¼����ע��"));
		return;
	}

	LRESULT hr = m_Client.GetUserProperty(&m_stUserProperty);
	if (S_OK != hr)
	{
		MessageBox(_T("��ȡ�������ʧ��"));
		return;
	}
	FreshCtrl();
	MessageBox(_T("��ȡ������Գɹ�"));

}


void CMFCClientDlg::OnBnClickedBtnSetbase()
{
	if (m_stUserAccount.guidUser == GUID_NULL)
	{
		MessageBox(_T("���ȵ�¼����ע��"));
		return;
	}
	GetCtrlVal();
	LRESULT hr = m_Client.SetUserProperty(&m_stUserProperty);
	if (S_OK != hr)
	{
		MessageBox(_T("�����������ʧ��"));
		return;
	}
	MessageBox(_T("����������Գɹ�"));

}


void CMFCClientDlg::OnBnClickedBtnGetcoord()
{
	if (m_stUserAccount.guidUser == GUID_NULL)
	{
		MessageBox(_T("���ȵ�¼����ע��"));
		return;
	}
	LRESULT hr = m_Client.GetCoordInfo(&m_stUserCoord);
	if (S_OK != hr)
	{
		MessageBox(_T("��ȡ����ʧ��"));
		return;
	}
	FreshCtrl();
	MessageBox(_T("��ȡ����ɹ�"));

}


void CMFCClientDlg::OnBnClickedBtnMoveup()
{
	if (m_stUserCoord.nCoordY > 0)
	{
		m_stUserCoord.nCoordY --;
	}
	else
	{
		MessageBox(_T("�ѵ���ͼ�ϱ�Ե"));
		return;
	}
	FreshCtrl();
}


void CMFCClientDlg::OnBnClickedBtnMoveleft()
{
	if (m_stUserCoord.nCoordX > 0)
	{
		m_stUserCoord.nCoordX--;
	}
	else
	{
		MessageBox(_T("�ѵ���ͼ���Ե"));
		return;
	}
	FreshCtrl();
}


void CMFCClientDlg::OnBnClickedBtnMoveright()
{
	if (m_stUserCoord.nCoordX  < MAX_COORD_X)
	{
		m_stUserCoord.nCoordX++;
	}
	else
	{
		MessageBox(_T("�ѵ���ͼ�ұ�Ե"));
		return;
	}
	FreshCtrl();
}


void CMFCClientDlg::OnBnClickedBtnMovebotoom()
{
	if (m_stUserCoord.nCoordY < MAX_COORD_Y)
	{
		m_stUserCoord.nCoordY ++;
	}
	else
	{
		MessageBox(_T("�ѵ���ͼ�±�Ե"));
		return;
	}
	FreshCtrl();
}


void CMFCClientDlg::OnBnClickedBtnMapUp()
{
	if (m_stUserCoord.enumGameMap > GAME_MAP_START + 1)
	{
		m_stUserCoord.enumGameMap = GAME_MAP_ENUM((int)m_stUserCoord.enumGameMap - 1);
	}
	else
	{
		MessageBox(_T("�ѵ���һ����ͼ"));
		return;
	}
	FreshCtrl();
}


void CMFCClientDlg::OnBnClickedBtnMapDown()
{
	if (m_stUserCoord.enumGameMap < GAME_MAP_END - 1)
	{
		m_stUserCoord.enumGameMap = GAME_MAP_ENUM((int)m_stUserCoord.enumGameMap + 1);
	}
	else
	{
		MessageBox(_T("�ѵ����һ����ͼ"));
		return;
	}
	FreshCtrl();
}


void CMFCClientDlg::OnBnClickedBtnSetcoord()
{
	if (m_stUserAccount.guidUser == GUID_NULL)
	{
		MessageBox(_T("���ȵ�¼����ע��"));
		return;
	}

	GetCtrlVal();
	LRESULT hr = m_Client.SetCoordInfo(&m_stUserCoord);
	if (S_OK != hr)
	{
		MessageBox(_T("��������ʧ��"));
		return;
	}
	MessageBox(_T("��������ɹ�"));

}


void CMFCClientDlg::OnBnClickedBtnUpdatebag()
{
	if (m_stUserAccount.guidUser == GUID_NULL)
	{
		MessageBox(_T("���ȵ�¼����ע��"));
		return;
	}
	m_UserBagMgr.ResetMgr();
	
	GOODS_COMMON_INFO_T stGoodsInfo = { 0 };
	int nCnt = m_Client.GetUserBagGoodsCnt();
	for (int i = 0; i < nCnt; i++)
	{
		m_Client.GetUserBagGoodsInfoByIdx(i, &stGoodsInfo);
		m_UserBagMgr.AddGoods(&stGoodsInfo);
	}

	FreshList();
}


void CMFCClientDlg::OnBnClickedBtnUpdateshop()
{
	m_ShopMgr.ResetMgr();

	GOODS_COMMON_INFO_T stGoodsInfo = { 0 };
	int nCnt = m_Client.GetShopGoodsCnt();
	for (int i = 0; i < nCnt; i++)
	{
		m_Client.GetShopGoodsInfoByIdx(i, &stGoodsInfo);
		m_ShopMgr.AddGoods(&stGoodsInfo);
	}

	FreshList();

}

void CMFCClientDlg::FreshList()
{
	// Shop
	m_listShop.ResetContent();
	m_listBag.ResetContent();

	int nCnt = m_ShopMgr.GetGoodsCnt();

	GOODS_COMMON_INFO_T stGoodsInfo = { 0 };
	for (int i = 0; i < nCnt; i++)
	{
		m_ShopMgr.GetGoodsInfoByIdx(i, &stGoodsInfo);
		m_listShop.InsertString(i, stGoodsInfo.szGoodsName);
	}
	m_listShop.UpdateWindow();

	nCnt = m_UserBagMgr.GetGoodsCnt();
	for (int i = 0; i < nCnt; i++)
	{
		m_UserBagMgr.GetGoodsInfoByIdx(i, &stGoodsInfo);
		m_listBag.InsertString(i, stGoodsInfo.szGoodsName);
	}
	m_listBag.UpdateWindow();

}

void CMFCClientDlg::OnLbnSelchangeList2()
{
	int nCurSel = m_listShop.GetCurSel();

	if (nCurSel < 0)
		return;

	GOODS_COMMON_INFO_T stGoodsInfo = { 0 };
	m_ShopMgr.GetGoodsInfoByIdx(nCurSel, &stGoodsInfo);

	CString strGoodsInfoDisplay;
	strGoodsInfoDisplay.Format(_T("��Ʒ���� %s   \n��Ʒ����: %s  \n��Ʒ�۸�: %.2f"),
		stGoodsInfo.szGoodsName, stGoodsInfo.szGoodsDesc, stGoodsInfo.fGoodsPrice);
	m_staticGoodsInfo.SetWindowText(strGoodsInfoDisplay);
}


void CMFCClientDlg::OnLbnSelchangeList3()
{
	int nCurSel = m_listBag.GetCurSel();

	if (nCurSel < 0)
		return;

	GOODS_COMMON_INFO_T stGoodsInfo = { 0 };
	m_UserBagMgr.GetGoodsInfoByIdx(nCurSel, &stGoodsInfo);

	CString strGoodsInfoDisplay;
	strGoodsInfoDisplay.Format(_T("��Ʒ���� %s   \n��Ʒ����: %s  \n��Ʒ�۸�: %.2f  \n��Ʒ���� %d"),
		stGoodsInfo.szGoodsName, stGoodsInfo.szGoodsDesc, stGoodsInfo.fGoodsPrice, stGoodsInfo.unGoodsCnt);
	m_staticGoodsInfo.SetWindowText(strGoodsInfoDisplay);
}
