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

//数据包包头
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

//客户生命值信息
#define MAX_CLIENT_LIFE		 	 	5
#define POLLING_TIME		 			1000
#define CONNECT_TIME_OUT 			5000

//服务器端口号定义
#define SRV_PORT_LOGIN						30010				//	login server
#define SRV_PORT_LOGIC						30020				//	logic server
#define SRV_PORT_BILLING						30030				// billing server


#define MSG_UPDATE								0
#define MSG_REQUEST							100
#define MSG_REPLY									200

//==========================================================================
//TcpBase messages: client and server can send messages to each other
//==========================================================================
#define MSG_UPDATE_COORDINATE				(MSG_UPDATE)				//数据包结构:1.UPDATE_COORDINATE_PKG_T
#define MSG_UPDATE_SHOP							(MSG_UPDATE + 1)			//数据包结构:1.UPDATE_SHOP_HEADER_PKG_T +	 GOODS_INFO_PKG_T*nGoodsCnt	
#define MSG_UPDATE_USER_BAG					(MSG_UPDATE + 2)			//数据包结构:1.UPDATE_BAG_HEADER_PKG_T + GOODS_INFO_PKG_T*nGoodsCnt	
#define MSG_UPDATE_USER_PROPERTY			(MSG_UPDATE + 3)			//数据包结构:1.UPDATE_USER_PROPERTY_PKG_T

//==========================================================================
//TcpBase commands: only client can send commands to server.
//==========================================================================
#define CMD_USER_SIGNGUP_REQUEST					(MSG_REQUEST + 0)		//数据包机构REQUEST_SIGNUP_PKG_T; 有回应，回应消息为：REPLY_SIGNUP_PKG_T
#define CMD_USER_LOGIN_REQUEST						(MSG_REQUEST + 1)		//数据包机构REQUEST_LOGIN_PKG_T; 有回应，回应消息为：REPLY_LOGIN_PKG_T
#define CMD_USER_LOGOUT_REQUEST					(MSG_REQUEST + 2)		//数据包机构REQUEST_LOGOUT_PKG_T; 有回应，回应消息为：REPLY_LOGOUT_PKG_T
#define CMD_SELL_GOODS_REQUEST						(MSG_REQUEST + 3)		//数据包机构REQUEST_SELL_GOODS_PKG_T; 有回应，回应消息为：REPLY_SELL_GOODS_PKG_T
#define CMD_BUY_GOODS_REQUEST						(MSG_REQUEST + 4)		//数据包机构REQUEST_BUY_GOODS_PKG_T; 有回应，回应消息为：REPLY_BUY_GOODS_PKG_T
#define CMD_DROP_GOODS_REQUEST					(MSG_REQUEST + 5)		//数据包机构REQUEST_SELL_GOODS_PKG_T; 有回应，回应消息为：REPLY_DROP_GOODS_PKG_T


//==========================================================================
//TcpBase replies: only server can send reply to client.
//==========================================================================
#define CMD_USER_SIGNUP_REPLY						(MSG_REPLY + 0)	//数据包机构1.REPLY_SIGNUP_PKG_T;
#define CMD_USER_LOGIN_REPLY						(MSG_REPLY + 1)	//数据包机构1.REPLY_LOGIN_PKG_T;
#define CMD_USER_LOGOUT_REPLY					(MSG_REPLY + 2)	//数据包机构1.REPLY_LOGOUT_PKG_T;
#define CMD_SELL_GOODS_REPLY						(MSG_REPLY + 3)	//数据包机构1.REPLY_SELL_GOODS_PKG_T;
#define CMD_BUY_GOODS_REPLY						(MSG_REPLY + 4)	//数据包机构1.REPLY_BUY_GOODS_PKG_T;
#define CMD_DROP_GOODS_REPLY					(MSG_REPLY + 5)	//数据包机构1.REPLY_DROP_GOODS_PKG_T;


//==========================================================================
//TcpBase传输协议数据包结构
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
	unsigned short	uNum;						//每个种类商品的数量，用于购买商品时使用
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
	*@nType: 0 上传用户属性， otherwise 下载用户属性
	*/
	COS_GUID guidUser;
	int nType;
	int nLevel;			//玩家等级
	int nBayType;	//玩家背包类型
	float fMoney;
}UPDATE_USER_PROPERTY_PKG_T;

typedef struct _update_coordinate_pkg_t
{
	/*
	*@nType: 0 上传用户坐标， otherwise 下载用户坐标
	*/

	COS_GUID guidUser;
	int nType;
	int nMapIndex;
	int nCoordX;
	int nCoordY;
}UPDATE_COORDINATE_PKG_T;


// 由于背包/商店内 商品数量未知，采用先读取头，再根据头信息读取商品
typedef struct  _update_bag_header_pkg_t
{
	COS_GUID guidUser;
	unsigned short nGoodsCount;					//背包内总共有多少种类的商品
	//GOODS_INFO_PKG_T * nGoodsCount
}UPDATE_BAG_HEADER_PKG_T;

typedef struct  _update_shop_header_pkg_t
{
	unsigned short nGoodsCount;					//商店内总共有多少种类的商品
	//GOODS_INFO_PKG_T * nGoodsCount
}UPDATE_SHOP_HEADER_PKG_T;





#endif