#include "BillingClient.h"

CBillingClient::CBillingClient()
{

}

CBillingClient::~CBillingClient()
{

}

LRESULT CBillingClient::BuyGoods(const COS_GUID*pUserGuid, const COS_GUID* pGoodsGuid, int nGoodsCnt)
{
	LRESULT hr = S_OK;
	REQUEST_BUY_GOODS_PKG_T stPkg;
	ZeroMemory(&stPkg, sizeof(stPkg));
	REPLY_BUY_GOODS_PKG_T stPkgReply = { 0 };
	MSGHEAD stReplyHead = { 0 };

	int nReplyBufLen = 0;
	char* pReplyBuf = NULL;
	int nOffset = 0;

	stPkg.guidUser = *pUserGuid;
	stPkg.guidGoods = *pGoodsGuid;
	stPkg.uNum = nGoodsCnt;
	stPkg.request_type = REQUEST_GOODS_TYPE_E::e_goods_buy;

	hr = SendCmd(CMD_BUY_GOODS_REQUEST, (char*)&stPkg, sizeof(stPkg));
	CHECK_SOK(hr);

	// wait for server reply
	nReplyBufLen = sizeof(MSGHEAD)+sizeof(REPLY_BUY_GOODS_PKG_T);
	pReplyBuf = (char*)malloc(nReplyBufLen);

	if (!SockRecv(pReplyBuf, nReplyBufLen))
	{
		hr = ERR_CLIENT_SER_REPLY_NOREPLY;
	}
	CHECK_SOK(hr);

	nOffset = MemOP_Read(pReplyBuf, &stReplyHead, sizeof(stReplyHead), nOffset);
	nOffset = MemOP_Read(pReplyBuf, &stPkgReply, sizeof(stPkgReply), nOffset);

	if (stReplyHead.dwMsgType != CMD_BUY_GOODS_REPLY ||
		stReplyHead.dwDataLen != sizeof(stPkgReply))
	{
		hr = ERR_CLIENT_SER_REPLY_NOREPLY;
	}
	CHECK_SOK(hr);

	if (stPkgReply.nResult != 0)
		hr = ERR_CLIENT_SER_REPLY_FALSE;
	else
		hr = ERR_CLIENT_SER_REPLY_TRUE;
	CHECK_SOK(hr);

done:
	SAFE_FREE(pReplyBuf);
	return hr;

}

LRESULT CBillingClient::SellGoods(const COS_GUID*pUserGuid, const COS_GUID* pGoodsGuid, int nGoodsCnt)
{
	LRESULT hr = S_OK;
	REQUEST_SELL_GOODS_PKG_T stPkg;
	ZeroMemory(&stPkg, sizeof(stPkg));
	REPLY_SELL_GOODS_PKG_T stPkgReply = { 0 };
	MSGHEAD stReplyHead = { 0 };

	int nReplyBufLen = 0;
	char* pReplyBuf = NULL;
	int nOffset = 0;

	stPkg.guidUser = *pUserGuid;
	stPkg.guidGoods = *pGoodsGuid;
	stPkg.uNum = nGoodsCnt;
	stPkg.request_type = REQUEST_GOODS_TYPE_E::e_goods_sell;

	hr = SendCmd(CMD_SELL_GOODS_REQUEST, (char*)&stPkg, sizeof(stPkg));
	CHECK_SOK(hr);

	// wait for server reply
	nReplyBufLen = sizeof(MSGHEAD)+sizeof(REPLY_SELL_GOODS_PKG_T);
	pReplyBuf = (char*)malloc(nReplyBufLen);

	if (!SockRecv(pReplyBuf, nReplyBufLen))
	{
		hr = ERR_CLIENT_SER_REPLY_NOREPLY;
	}
	CHECK_SOK(hr);

	nOffset = MemOP_Read(pReplyBuf, &stReplyHead, sizeof(stReplyHead), nOffset);
	nOffset = MemOP_Read(pReplyBuf, &stPkgReply, sizeof(stPkgReply), nOffset);

	if (stReplyHead.dwMsgType != CMD_SELL_GOODS_REPLY ||
		stReplyHead.dwDataLen != sizeof(stPkgReply))
	{
		hr = ERR_CLIENT_SER_REPLY_NOREPLY;
	}
	CHECK_SOK(hr);

	if (stPkgReply.nResult != 0)
		hr = ERR_CLIENT_SER_REPLY_FALSE;
	else
		hr = ERR_CLIENT_SER_REPLY_TRUE;
	CHECK_SOK(hr);

done:
	SAFE_FREE(pReplyBuf);
	return hr;
}

LRESULT CBillingClient::DropGoods(const COS_GUID*pUserGuid, const COS_GUID* pGoodsGuid, int nGoodsCnt)
{
	LRESULT hr = S_OK;
	REQUEST_DROP_GOODS_PKG_T stPkg;
	ZeroMemory(&stPkg, sizeof(stPkg));
	REPLY_DROP_GOODS_PKG_T stPkgReply = { 0 };
	MSGHEAD stReplyHead = { 0 };

	int nReplyBufLen = 0;
	char* pReplyBuf = NULL;
	int nOffset = 0;

	stPkg.guidUser = *pUserGuid;
	stPkg.guidGoods = *pGoodsGuid;
	stPkg.uNum = nGoodsCnt;
	stPkg.request_type = REQUEST_GOODS_TYPE_E::e_goods_drop;

	hr = SendCmd(CMD_DROP_GOODS_REQUEST, (char*)&stPkg, sizeof(stPkg));
	CHECK_SOK(hr);

	// wait for server reply
	nReplyBufLen = sizeof(MSGHEAD)+sizeof(REPLY_DROP_GOODS_PKG_T);
	pReplyBuf = (char*)malloc(nReplyBufLen);

	if (!SockRecv(pReplyBuf, nReplyBufLen))
	{
		hr = ERR_CLIENT_SER_REPLY_NOREPLY;
	}
	CHECK_SOK(hr);

	nOffset = MemOP_Read(pReplyBuf, &stReplyHead, sizeof(stReplyHead), nOffset);
	nOffset = MemOP_Read(pReplyBuf, &stPkgReply, sizeof(stPkgReply), nOffset);

	if (stReplyHead.dwMsgType != CMD_DROP_GOODS_REQUEST ||
		stReplyHead.dwDataLen != sizeof(stPkgReply))
	{
		hr = ERR_CLIENT_SER_REPLY_NOREPLY;
	}
	CHECK_SOK(hr);

	if (stPkgReply.nResult != 0)
		hr = ERR_CLIENT_SER_REPLY_FALSE;
	else
		hr = ERR_CLIENT_SER_REPLY_TRUE;
	CHECK_SOK(hr);

done:
	SAFE_FREE(pReplyBuf);
	return hr;
}