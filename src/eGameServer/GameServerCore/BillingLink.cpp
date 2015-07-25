#include "BillingLink.h"
#include "LoginSrv.h"


CBillingLink::CBillingLink()
{
}

CBillingLink::~CBillingLink()
{
}

LRESULT CBillingLink::ProcessRawData(const MSGHEAD* pstHead, const char* pDataBuf, int nDataLen)
{
	LRESULT hr = S_OK;

	switch (pstHead->dwMsgType)
	{
	case CMD_SELL_GOODS_REQUEST:
		hr = ProcSellGoods(pDataBuf, nDataLen);
		break;
	case CMD_BUY_GOODS_REQUEST:
		hr = ProcBuyGoods(pDataBuf, nDataLen);
		break;
	case CMD_DROP_GOODS_REQUEST:
		hr = ProcDropGoods(pDataBuf, nDataLen);
		break;
	default:
		break;
	}

	return hr;
}

LRESULT CBillingLink::ProcBuyGoods(const char* pDataBuf, int nDatalen)
{
	LRESULT hr = S_OK;
	assert(pDataBuf);
	assert(sizeof(REQUEST_BUY_GOODS_PKG_T) == nDatalen);

	REQUEST_BUY_GOODS_PKG_T stPkg;
	memcpy(&stPkg, pDataBuf, nDatalen);

	hr = ProcessGoods(&stPkg.guidUser, &stPkg.guidGoods, stPkg.uNum, e_goods_buy);

	//服务端处理完毕，根据返回结果，发送回复至客户端
	REPLY_BUY_GOODS_PKG_T stReply = { 0 };

	if (FAILED(hr))
		stReply.nResult = -1;
	else
		stReply.nResult = 0;

	// 发送回应
	SendReply(CMD_BUY_GOODS_REPLY, &stReply, sizeof(stReply));

	return hr;
}

LRESULT CBillingLink::ProcSellGoods(const char* pDataBuf, int nDatalen)
{
	LRESULT hr = S_OK;
	assert(pDataBuf);
	assert(sizeof(REQUEST_SELL_GOODS_PKG_T) == nDatalen);

	REQUEST_SELL_GOODS_PKG_T stPkg;
	memcpy(&stPkg, pDataBuf, nDatalen);

	hr = ProcessGoods(&stPkg.guidUser, &stPkg.guidGoods, stPkg.uNum, e_goods_sell);

	//服务端处理完毕，根据返回结果，发送回复至客户端
	REPLY_SELL_GOODS_PKG_T stReply = { 0 };

	if (FAILED(hr))
		stReply.nResult = -1;
	else
		stReply.nResult = 0;

	// 发送回应
	SendReply(CMD_SELL_GOODS_REPLY, &stReply, sizeof(stReply));

	return hr;
}

LRESULT CBillingLink::ProcDropGoods(const char* pDataBuf, int nDatalen)
{
	LRESULT hr = S_OK;
	assert(pDataBuf);
	assert(sizeof(REQUEST_DROP_GOODS_PKG_T) == nDatalen);

	REQUEST_DROP_GOODS_PKG_T stPkg;
	memcpy(&stPkg, pDataBuf, nDatalen);

	hr = ProcessGoods(&stPkg.guidUser, &stPkg.guidGoods, stPkg.uNum, e_goods_buy);

	//服务端处理完毕，根据返回结果，发送回复至客户端
	REPLY_DROP_GOODS_PKG_T stReply = { 0 };

	if (FAILED(hr))
		stReply.nResult = -1;
	else
		stReply.nResult = 0;

	// 发送回应
	SendReply(CMD_DROP_GOODS_REPLY, &stReply, sizeof(stReply));

	return hr;
}

LRESULT CBillingLink::ProcessGoods(const COS_GUID* pGuidUser, const COS_GUID * pGuidGoods, int nGoodsCnt, int nProcessType)
{
	LRESULT hr = S_OK;
	TCHAR szUserGuid[MIN_GUID_STR_LEN] = { 0 };
	CrossPlatform_GUID2String(*pGuidUser, szUserGuid, MIN_GUID_STR_LEN);

	cout << "user";
	switch ( nProcessType)
	{
	case (int)e_goods_buy:
		hr = m_DBMgr.UserBuyGoods(pGuidUser, pGuidGoods, nGoodsCnt);
		cout << " buy goods：";
		break;
	case (int)e_goods_sell:
		hr = m_DBMgr.UserSellGoods(pGuidUser, pGuidGoods, nGoodsCnt);
		cout <<(" sell goods：");
		break;
	case (int)e_goods_drop:
		hr = m_DBMgr.UserDropGoods(pGuidUser, pGuidGoods, nGoodsCnt);
		cout <<(" drop goods：");
		break;

	default:
		break;
	}

	cout << "  return： "<< (int)hr << endl;

	return hr;
}
