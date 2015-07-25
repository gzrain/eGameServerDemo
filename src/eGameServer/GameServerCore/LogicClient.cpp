#include "LogicClient.h"


CLogicClient::CLogicClient()
{
}

CLogicClient::~CLogicClient()
{
}

LRESULT CLogicClient::UnInit()
{
	m_ShopMgr.ResetMgr();
	m_UserBagMgr.ResetMgr();

	return CBaseClient::UnInit();
}

LRESULT CLogicClient::GetUserCoordInfo(const COS_GUID* pUserGuid, LPUSER_COORDINATE_INFO_T lpstUserCoordInfo)
{
	assert(pUserGuid);
	assert(lpstUserCoordInfo);

	LRESULT hr = S_OK;
	UPDATE_COORDINATE_PKG_T stPkgReply = { 0 };
	MSGHEAD stReplyHead = { 0 };

	int nReplyBufLen = 0;
	char* pReplyBuf = NULL;
	int nOffset = 0;

	stPkgReply.guidUser = *pUserGuid;
	stPkgReply.nType = 0;

	hr = SendCmd(MSG_UPDATE_COORDINATE, (char*)&stPkgReply, sizeof(stPkgReply));
	CHECK_SOK(hr);

	// wait for server reply
	nReplyBufLen = sizeof(MSGHEAD)+sizeof(UPDATE_COORDINATE_PKG_T);
	pReplyBuf = (char*)malloc(nReplyBufLen);

	if (!SockRecv(pReplyBuf, nReplyBufLen))
	{
		hr = ERR_CLIENT_SER_REPLY_NOREPLY;
	}
	CHECK_SOK(hr);

	nOffset = MemOP_Read(pReplyBuf, &stReplyHead, sizeof(stReplyHead), nOffset);
	nOffset = MemOP_Read(pReplyBuf, &stPkgReply, sizeof(stPkgReply), nOffset);

	if (stReplyHead.dwMsgType != MSG_UPDATE_COORDINATE ||
		stReplyHead.dwDataLen != sizeof(stPkgReply))
	{
		hr = ERR_CLIENT_SER_REPLY_NOREPLY;
	}
	CHECK_SOK(hr);

	lpstUserCoordInfo->enumGameMap = (GAME_MAP_ENUM)stPkgReply.nMapIndex;
	lpstUserCoordInfo->nCoordX = stPkgReply.nCoordX;
	lpstUserCoordInfo->nCoordY = stPkgReply.nCoordY;

done:
	SAFE_FREE(pReplyBuf);
	return hr;

}

LRESULT CLogicClient::SetUserCoordInfo(const COS_GUID* pUserGuid, LPUSER_COORDINATE_INFO_T lpstUserCoordInfo)
{
	assert(pUserGuid);
	assert(lpstUserCoordInfo);

	LRESULT hr = S_OK;
	UPDATE_COORDINATE_PKG_T stPkgSend = { 0 };
	MSGHEAD stReplyHead = { 0 };

	int nReplyBufLen = 0;
	char* pReplyBuf = NULL;
	int nOffset = 0;

	stPkgSend.guidUser = *pUserGuid;
	stPkgSend.nType = 1;
	stPkgSend.nMapIndex = lpstUserCoordInfo->enumGameMap;
	stPkgSend.nCoordX = lpstUserCoordInfo->nCoordX;
	stPkgSend.nCoordY = lpstUserCoordInfo->nCoordY;

	hr = SendCmd(MSG_UPDATE_COORDINATE, (char*)&stPkgSend, sizeof(stPkgSend));

	return hr;
}

LRESULT CLogicClient::GetUserProperty(const COS_GUID* pUserGuid, LPUSER_GAME_PROPERTY_T lpstUserProperty)
{
	assert(pUserGuid);
	assert(lpstUserProperty);

	LRESULT hr = S_OK;
	UPDATE_USER_PROPERTY_PKG_T stPkgReply = { 0 };
	MSGHEAD stReplyHead = { 0 };

	int nReplyBufLen = 0;
	char* pReplyBuf = NULL;
	int nOffset = 0;

	stPkgReply.guidUser = *pUserGuid;
	stPkgReply.nType = 0;

	hr = SendCmd(MSG_UPDATE_USER_PROPERTY, (char*)&stPkgReply, sizeof(stPkgReply));
	CHECK_SOK(hr);

	// wait for server reply
	nReplyBufLen = sizeof(MSGHEAD)+sizeof(UPDATE_USER_PROPERTY_PKG_T);
	pReplyBuf = (char*)malloc(nReplyBufLen);

	if (!SockRecv(pReplyBuf, nReplyBufLen))
	{
		hr = ERR_CLIENT_SER_REPLY_NOREPLY;
	}
	CHECK_SOK(hr);

	nOffset = MemOP_Read(pReplyBuf, &stReplyHead, sizeof(stReplyHead), nOffset);
	nOffset = MemOP_Read(pReplyBuf, &stPkgReply, sizeof(stPkgReply), nOffset);

	if (stReplyHead.dwMsgType != MSG_UPDATE_USER_PROPERTY ||
		stReplyHead.dwDataLen != sizeof(stPkgReply))
	{
		hr = ERR_CLIENT_SER_REPLY_NOREPLY;
	}
	CHECK_SOK(hr);

	lpstUserProperty->fUserMoney = stPkgReply.fMoney;
	lpstUserProperty->guidUser = stPkgReply.guidUser;
	lpstUserProperty->unBagType = stPkgReply.nBayType;
	lpstUserProperty->unUserLevel = stPkgReply.nLevel;

done:
	SAFE_FREE(pReplyBuf);
	return hr;

}

LRESULT CLogicClient::SetUserProperty(const COS_GUID* pUserGuid, LPUSER_GAME_PROPERTY_T lpstUserProperty)
{
	assert(pUserGuid);
	assert(lpstUserProperty);

	LRESULT hr = S_OK;
	UPDATE_USER_PROPERTY_PKG_T stPkgSend = { 0 };
	MSGHEAD stReplyHead = { 0 };

	int nReplyBufLen = 0;
	char* pReplyBuf = NULL;
	int nOffset = 0;

	stPkgSend.nType = 1;
	stPkgSend.fMoney = lpstUserProperty->fUserMoney;
	stPkgSend.guidUser = lpstUserProperty->guidUser;
	stPkgSend.nBayType = lpstUserProperty->unBagType;
	stPkgSend.nLevel = lpstUserProperty->unUserLevel;

	hr = SendCmd(MSG_UPDATE_USER_PROPERTY, (char*)&stPkgSend, sizeof(stPkgSend));

	return hr;

}


int CLogicClient::GetShopGoodsCnt()
{
	m_ShopMgr.ResetMgr();

	LRESULT hr = S_OK;
	UPDATE_SHOP_HEADER_PKG_T stPkg = { 0 };
	MSGHEAD stReplyHead = { 0 };
	UPDATE_SHOP_HEADER_PKG_T stPkgReplyHead = { 0 };

	int nReplyHeadLen = 0;
	int nReplyDataLen = 0;
	char* pReplyHeadBuf = NULL;
	char* pReplyData = NULL;
	int nGoodsCnt = 0;
	int nOffset = 0;

	hr = SendCmd(MSG_UPDATE_SHOP, (char*)&stPkg, sizeof(stPkg));
	CHECK_SOK(hr);

	// wait for server reply
	// 必须先接收 数据头，根据头中数量信息，再接收余下数据

	nReplyHeadLen = sizeof(MSGHEAD)+sizeof(UPDATE_SHOP_HEADER_PKG_T);
	pReplyHeadBuf = (char*)malloc(nReplyHeadLen);

	//接受头数据
	if (!SockRecv(pReplyHeadBuf, nReplyHeadLen))
	{
		hr = ERR_CLIENT_SER_REPLY_NOREPLY;
	}
	CHECK_SOK(hr);

	nOffset = MemOP_Read(pReplyHeadBuf, &stReplyHead, sizeof(stReplyHead), nOffset);
	nOffset = MemOP_Read(pReplyHeadBuf, &stPkgReplyHead, sizeof(stPkgReplyHead), nOffset);

	if (stReplyHead.dwMsgType != MSG_UPDATE_SHOP )
	{
		hr = ERR_CLIENT_SER_REPLY_NOREPLY;
	}
	CHECK_SOK(hr);
	nGoodsCnt = stPkgReplyHead.nGoodsCount;
	//recv goods info
	nReplyDataLen = sizeof(GOODS_INFO_PKG_T)*nGoodsCnt;
	pReplyData = (char*)malloc(nReplyDataLen);

	//接收余下数据
	if (!SockRecv(pReplyData, nReplyDataLen))
	{
		hr = ERR_CLIENT_SER_REPLY_NOREPLY;
	}
	CHECK_SOK(hr);

	nOffset = 0;
	for (int i = 0; i < nGoodsCnt; i++)
	{
		GOODS_INFO_PKG_T stGoodsInfo = { 0 };
		nOffset = MemOP_Read(pReplyData, &stGoodsInfo, sizeof(stGoodsInfo), nOffset);

		GOODS_COMMON_INFO_T stCommonGoodsInfo = { 0 };
		stCommonGoodsInfo.fGoodsPrice = stGoodsInfo.fGoodsPrice;
		stCommonGoodsInfo.guidGoods = stGoodsInfo.guidGoods;
		memcpy(stCommonGoodsInfo.szGoodsDesc, stGoodsInfo.szGoodsDesc, sizeof(stCommonGoodsInfo.szGoodsDesc));
		memcpy(stCommonGoodsInfo.szGoodsName, stGoodsInfo.szGoodsName, sizeof(stCommonGoodsInfo.szGoodsName));

		//将商品信息保存到shop mgr 中
		m_ShopMgr.AddGoods(&stCommonGoodsInfo);
	}

done:
	SAFE_FREE(pReplyData);
	SAFE_FREE(pReplyHeadBuf);
	return nGoodsCnt;
}

LRESULT CLogicClient::GetShopGoodsInfoByIdx(int nGoodsIdx, LPGOODS_COMMON_INFO_T lpstGoodsInfoOut)
{
	LRESULT hr = S_OK;
	assert(nGoodsIdx >= 0);
	assert(lpstGoodsInfoOut);

	int nGoodCnt = m_ShopMgr.GetGoodsCnt();

	if (nGoodsIdx >= nGoodCnt)
		return E_FAIL;

	hr = m_ShopMgr.GetGoodsInfoByIdx(nGoodsIdx, lpstGoodsInfoOut);

	return hr;
}

int CLogicClient::GetUserBagGoodsCnt(const COS_GUID* pUserGuid)
{
	m_UserBagMgr.ResetMgr();

	LRESULT hr = S_OK;
	UPDATE_BAG_HEADER_PKG_T stPkg = { 0 };
	MSGHEAD stReplyHead = { 0 };
	UPDATE_BAG_HEADER_PKG_T stPkgReplyHead = { 0 };

	int nReplyHeadLen = 0;
	int nReplyDataLen = 0;
	char* pReplyHeadBuf = NULL;
	char* pReplyData = NULL;
	int nGoodsCnt = 0;
	int nOffset = 0;
	
	stPkg.guidUser = *pUserGuid;
	hr = SendCmd(MSG_UPDATE_USER_BAG, (char*)&stPkg, sizeof(stPkg));
	CHECK_SOK(hr);

	// wait for server reply
	// 必须先接收 数据头，根据头中数量信息，再接收余下数据
	nReplyHeadLen = sizeof(MSGHEAD)+sizeof(UPDATE_BAG_HEADER_PKG_T);
	pReplyHeadBuf = (char*)malloc(nReplyHeadLen);

	//接受头数据
	if (!SockRecv(pReplyHeadBuf, nReplyHeadLen))
	{
		hr = ERR_CLIENT_SER_REPLY_NOREPLY;
	}
	CHECK_SOK(hr);

	nOffset = MemOP_Read(pReplyHeadBuf, &stReplyHead, sizeof(stReplyHead), nOffset);
	nOffset = MemOP_Read(pReplyHeadBuf, &stPkgReplyHead, sizeof(stPkgReplyHead), nOffset);

	if (stReplyHead.dwMsgType != MSG_UPDATE_USER_BAG)
	{
		hr = ERR_CLIENT_SER_REPLY_NOREPLY;
	}
	CHECK_SOK(hr);
	nGoodsCnt = stPkgReplyHead.nGoodsCount;

	assert(stPkgReplyHead.guidUser == *pUserGuid);

	//recv goods info
	nReplyDataLen = sizeof(GOODS_INFO_PKG_T)*nGoodsCnt;
	pReplyData = (char*)malloc(nReplyDataLen);

	//接收余下数据
	if (!SockRecv(pReplyData, nReplyDataLen))
	{
		hr = ERR_CLIENT_SER_REPLY_NOREPLY;
	}
	CHECK_SOK(hr);

	nOffset = 0;
	for (int i = 0; i < nGoodsCnt; i++)
	{
		GOODS_INFO_PKG_T stGoodsInfo = { 0 };
		nOffset = MemOP_Read(pReplyData, &stGoodsInfo, sizeof(stGoodsInfo), nOffset);

		GOODS_COMMON_INFO_T stCommonGoodsInfo = { 0 };
		stCommonGoodsInfo.fGoodsPrice = stGoodsInfo.fGoodsPrice;
		stCommonGoodsInfo.guidGoods = stGoodsInfo.guidGoods;
		stCommonGoodsInfo.unGoodsCnt = stGoodsInfo.uNum;
		memcpy(stCommonGoodsInfo.szGoodsDesc, stGoodsInfo.szGoodsDesc, sizeof(stCommonGoodsInfo.szGoodsDesc));
		memcpy(stCommonGoodsInfo.szGoodsName, stGoodsInfo.szGoodsName, sizeof(stCommonGoodsInfo.szGoodsName));

		//将商品信息保存到shop mgr 中
		m_UserBagMgr.AddGoods(&stCommonGoodsInfo);
	}

done:
	SAFE_FREE(pReplyData);
	SAFE_FREE(pReplyHeadBuf);
	return nGoodsCnt;

}

LRESULT CLogicClient::GetUserBagGoodsInfoByIdx(int nGoodsIdx, LPGOODS_COMMON_INFO_T lpstGoodsInfoOut)
{
	LRESULT hr = S_OK;
	assert(nGoodsIdx >= 0);
	assert(lpstGoodsInfoOut);

	int nGoodCnt = m_UserBagMgr.GetGoodsCnt();

	if (nGoodsIdx >= nGoodCnt)
		return E_FAIL;

	hr = m_UserBagMgr.GetGoodsInfoByIdx(nGoodsIdx, lpstGoodsInfoOut);

	return hr;
}