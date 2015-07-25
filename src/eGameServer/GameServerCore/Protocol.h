// Protocol.h
// by gaozhe
// 20150706

#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

using namespace std;

#include <windows.h>
#include "guid.h"
#include <vector>

#define MAX_ACCOUNT_LEN				256
#define MAX_PASSWORD_LEN			256
#define MAX_GOODS_NAME_LEN		256
#define MAX_GOODS_DESC_LEN			1024
#define MAX_USER_NAME_LEN			256
#define MAX_USER_ALIAS_LEN			256

//���ݰ���ͷ
typedef struct _msghead
{
	DWORD dwSyncWord;			//=0xaaaabbbb
	DWORD dwMsgType;
	DWORD dwDataLen;
	int nPacketNum;
}MSGHEAD;

__inline int Protocol_SetMsgHead(void * pBuf, DWORD dwMsgType, int dwDataLen)
{
	((MSGHEAD *)pBuf)->dwSyncWord = 0xaaaabbbb;
	((MSGHEAD *)pBuf)->dwMsgType = dwMsgType;
	((MSGHEAD *)pBuf)->dwDataLen = dwDataLen;
	((MSGHEAD *)pBuf)->nPacketNum = 1;
	return sizeof(MSGHEAD);
}

//�ͻ�����ֵ��Ϣ
#define MAX_CLIENT_LIFE		 	 	5
#define POLLING_TIME		 			1000
#define CONNECT_TIME_OUT 			5000

//�������˿ںŶ���
#define SRV_PORT_LOGIN						30010				//	login server
#define SRV_PORT_LOGIC						30020				//	logic server
#define SRV_PORT_BILLING						30030				// billing server


#define MSG_UPDATE								0
#define MSG_REQUEST							100
#define MSG_REPLY									200

//==========================================================================
//TcpBase messages: client and server can send messages to each other
//==========================================================================
#define MSG_UPDATE_COORDINATE				(MSG_UPDATE)				//���ݰ��ṹ:1.UPDATE_COORDINATE_PKG_T
#define MSG_UPDATE_SHOP							(MSG_UPDATE + 1)			//���ݰ��ṹ:1.UPDATE_SHOP_HEADER_PKG_T +	 GOODS_INFO_PKG_T*nGoodsCnt	
#define MSG_UPDATE_USER_BAG					(MSG_UPDATE + 2)			//���ݰ��ṹ:1.UPDATE_BAG_HEADER_PKG_T + GOODS_INFO_PKG_T*nGoodsCnt	
#define MSG_UPDATE_USER_PROPERTY			(MSG_UPDATE + 3)			//���ݰ��ṹ:1.UPDATE_USER_PROPERTY_PKG_T

//==========================================================================
//TcpBase commands: only client can send commands to server.
//==========================================================================
#define CMD_USER_SIGNGUP_REQUEST					(MSG_REQUEST + 0)		//���ݰ�����REQUEST_SIGNUP_PKG_T; �л�Ӧ����Ӧ��ϢΪ��REPLY_SIGNUP_PKG_T
#define CMD_USER_LOGIN_REQUEST						(MSG_REQUEST + 1)		//���ݰ�����REQUEST_LOGIN_PKG_T; �л�Ӧ����Ӧ��ϢΪ��REPLY_LOGIN_PKG_T
#define CMD_USER_LOGOUT_REQUEST					(MSG_REQUEST + 2)		//���ݰ�����REQUEST_LOGOUT_PKG_T; �л�Ӧ����Ӧ��ϢΪ��REPLY_LOGOUT_PKG_T
#define CMD_SELL_GOODS_REQUEST						(MSG_REQUEST + 3)		//���ݰ�����REQUEST_SELL_GOODS_PKG_T; �л�Ӧ����Ӧ��ϢΪ��REPLY_SELL_GOODS_PKG_T
#define CMD_BUY_GOODS_REQUEST						(MSG_REQUEST + 4)		//���ݰ�����REQUEST_BUY_GOODS_PKG_T; �л�Ӧ����Ӧ��ϢΪ��REPLY_BUY_GOODS_PKG_T
#define CMD_DROP_GOODS_REQUEST					(MSG_REQUEST + 5)		//���ݰ�����REQUEST_SELL_GOODS_PKG_T; �л�Ӧ����Ӧ��ϢΪ��REPLY_DROP_GOODS_PKG_T


//==========================================================================
//TcpBase replies: only server can send reply to client.
//==========================================================================
#define CMD_USER_SIGNUP_REPLY						(MSG_REPLY + 0)	//���ݰ�����1.REPLY_SIGNUP_PKG_T;
#define CMD_USER_LOGIN_REPLY						(MSG_REPLY + 1)	//���ݰ�����1.REPLY_LOGIN_PKG_T;
#define CMD_USER_LOGOUT_REPLY					(MSG_REPLY + 2)	//���ݰ�����1.REPLY_LOGOUT_PKG_T;
#define CMD_SELL_GOODS_REPLY						(MSG_REPLY + 3)	//���ݰ�����1.REPLY_SELL_GOODS_PKG_T;
#define CMD_BUY_GOODS_REPLY						(MSG_REPLY + 4)	//���ݰ�����1.REPLY_BUY_GOODS_PKG_T;
#define CMD_DROP_GOODS_REPLY					(MSG_REPLY + 5)	//���ݰ�����1.REPLY_DROP_GOODS_PKG_T;


//==========================================================================
//TcpBase����Э�����ݰ��ṹ
//==========================================================================

typedef struct _request_signup_pkg_t
{
	TCHAR szName[MAX_USER_NAME_LEN];
	TCHAR szAlias[MAX_USER_ALIAS_LEN];
	TCHAR szAccount[MAX_ACCOUNT_LEN];
	TCHAR szPswd[MAX_PASSWORD_LEN];
}REQUEST_SIGNUP_PKG_T;

typedef struct _reply_signup_pkg_t
{
	/*
	*@nResult: 0 success, otherwise failed
	*/
	int nResult;
}REPLY_SIGNUP_PKG_T;

typedef struct _request_login_pkg_t
{
	/*
	*@szAccount: user account
	*@szPswd: user password
	*/
	TCHAR szAccount[256];
	TCHAR szPswd[256];
}REQUEST_LOGIN_PKG_T;

typedef struct _reply_login_pkg_t
{
	/*
	*@guidUser: the guid of the user
	*@nResult: 0 success, otherwise failed
	*/
	COS_GUID guidUser;
	TCHAR szUserName[MAX_USER_NAME_LEN];
	TCHAR szUserAlias[MAX_USER_ALIAS_LEN];
	time_t	tmLastLogin;
	int nResult;
}REPLY_LOGIN_PKG_T;

typedef struct _request_logout_pkg_t
{
	/*
	*@guidUser: the guid of the user
	*/
	COS_GUID guidUser;
}REQUEST_LOGOUT_PKG_T;

typedef struct _reply_logout_pkg_t
{
	/*
	*@nResult: 0 success, otherwise failed
	*/
	int nResult;
}REPLY_LOGOUT_PKG_T;

typedef enum _request_goods_type_e
{
	e_goods_start = 0,
	e_goods_buy,
	e_goods_sell,
	e_goods_drop,
	e_goods_end
}REQUEST_GOODS_TYPE_E;

typedef struct _goods_info_pkg_t
{
	COS_GUID guidGoods;
	float fGoodsPrice;
	TCHAR szGoodsName[MAX_GOODS_NAME_LEN];
	TCHAR szGoodsDesc[MAX_GOODS_DESC_LEN];
	unsigned short	uNum;						//ÿ��������Ʒ�����������ڹ�����Ʒʱʹ��
}GOODS_INFO_PKG_T;

typedef struct _request_goods_pkg_t
{
	/*
	*@request_type: request type
	*@pkg: the goods info pkg
	*/
	REQUEST_GOODS_TYPE_E request_type;
	COS_GUID guidUser;
	COS_GUID guidGoods;
	unsigned short uNum;
}REQUEST_SELL_GOODS_PKG_T, REQUEST_BUY_GOODS_PKG_T, REQUEST_DROP_GOODS_PKG_T;

typedef struct _reply_goods_pkg_t
{
	/*
	*@nResult: 0 success, otherwise failed
	*/
	int  nResult;
}REPLY_SELL_GOODS_PKG_T, REPLY_BUY_GOODS_PKG_T, REPLY_DROP_GOODS_PKG_T;

typedef struct _update_user_property_pkg_t
{
	/*
	*@nType: 0 �ϴ��û����ԣ� otherwise �����û�����
	*/
	COS_GUID guidUser;
	int nType;
	int nLevel;			//��ҵȼ�
	int nBayType;	//��ұ�������
	float fMoney;
}UPDATE_USER_PROPERTY_PKG_T;

typedef struct _update_coordinate_pkg_t
{
	/*
	*@nType: 0 �ϴ��û����꣬ otherwise �����û�����
	*/

	COS_GUID guidUser;
	int nType;
	int nMapIndex;
	int nCoordX;
	int nCoordY;
}UPDATE_COORDINATE_PKG_T;


// ���ڱ���/�̵��� ��Ʒ����δ֪�������ȶ�ȡͷ���ٸ���ͷ��Ϣ��ȡ��Ʒ
typedef struct  _update_bag_header_pkg_t
{
	COS_GUID guidUser;
	unsigned short nGoodsCount;					//�������ܹ��ж����������Ʒ
	//GOODS_INFO_PKG_T * nGoodsCount
}UPDATE_BAG_HEADER_PKG_T;

typedef struct  _update_shop_header_pkg_t
{
	unsigned short nGoodsCount;					//�̵����ܹ��ж����������Ʒ
	//GOODS_INFO_PKG_T * nGoodsCount
}UPDATE_SHOP_HEADER_PKG_T;





#endif