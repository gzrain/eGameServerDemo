
// MFCClientDlg.h : 头文件
//

#pragma once

#include "ClientCore.h"
#include "afxwin.h"
#include "afxcmn.h"

// CMFCClientDlg 对话框
class CMFCClientDlg : public CDialogEx
{
// 构造
public:
	CMFCClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFCCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	void FreshCtrl();//初始化控件
	void GetCtrlVal();

	void FreshList();
private:
	CClientCore m_Client;

	USER_GAME_PROPERTY_T m_stUserProperty;
	USER_COORDINATE_INFO_T m_stUserCoord;
	USER_ACCOUNT_T m_stUserAccount;

	CGoodsMgr m_ShopMgr;
	CGoodsMgr m_UserBagMgr;
private:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedBtnBuy();
	afx_msg void OnBnClickedBtnSell();
	afx_msg void OnBnClickedBtnDrop();
	afx_msg void OnBnClickedBtnLogout();
	afx_msg void OnBnClickedBtnLogin();
	afx_msg void OnBnClickedBtnSignup();
	afx_msg void OnBnClickedBtnLevelUp();
	afx_msg void OnBnClickedBtnBagBup();
	afx_msg void OnBnClickedBtnMoneyUp();
	afx_msg void OnBnClickedBtnGetbase();
	afx_msg void OnBnClickedBtnSetbase();
	afx_msg void OnBnClickedBtnGetcoord();
	afx_msg void OnBnClickedBtnMoveup();
	afx_msg void OnBnClickedBtnMoveleft();
	afx_msg void OnBnClickedBtnMoveright();
	afx_msg void OnBnClickedBtnMovebotoom();
	afx_msg void OnBnClickedBtnMapUp();
	afx_msg void OnBnClickedBtnMapDown();
	afx_msg void OnBnClickedBtnSetcoord();
private:
	CEdit m_editAccount;
	CEdit m_editPswd;
	CEdit m_editName;
	CEdit m_editAlias;
	CStatic m_staticLevel;
	CStatic m_staticBagType;
	CStatic m_staticMoney;
	CStatic m_staticCoordx;
	CStatic m_staticCoordY;
	CStatic m_staticMap;
	
public:
	afx_msg void OnBnClickedBtnUpdatebag();
	afx_msg void OnBnClickedBtnUpdateshop();
	CListBox m_listShop;
	CListBox m_listBag;
	CStatic m_staticGoodsInfo;
	afx_msg void OnLbnSelchangeList2();
	afx_msg void OnLbnSelchangeList3();
	CIPAddressCtrl m_ipServer;
};
