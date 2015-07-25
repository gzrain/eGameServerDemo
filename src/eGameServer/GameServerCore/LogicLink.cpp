#include "LogicLink.h"
#include "LoginSrv.h"


CLogicLink::CLogicLink()
{
}

CLogicLink::~CLogicLink()
{
}

LRESULT CLogicLink::ProcessRawData(const MSGHEAD* pstHead, const char* pDataBuf, int nDataLen)
{
	LRESULT hr = S_OK;

	switch (pstHead->dwMsgType)
	{
	case MSG_UPDATE_COORDINATE:
		hr = ProcUpdateUserCoord(pDataBuf, nDataLen);
		break;
	case MSG_UPDATE_USER_BAG:
		hr = ProcUpdateUserBag(pDataBuf, nDataLen);
		break;
	case MSG_UPDATE_SHOP:
		hr = ProcUpdateShop(pDataBuf, nDataLen);
		break;
	case MSG_UPDATE_USER_PROPERTY:
		hr = ProcUpdateUserProperty(pDataBuf, nDataLen);
		break;
	default:
		break;
	}

	return hr;
}

LRESULT CLogicLink::ProcUpdateUserCoord(const char* pDataBuf, int nDatalen)
{
	LRESULT hr = S_OK;
	assert(pDataBuf);
	assert(sizeof(UPDATE_COORDINATE_PKG_T) == nDatalen);

	UPDATE_COORDINATE_PKG_T stPkg = { 0 };
	memcpy(&stPkg, pDataBuf, nDatalen);

	USER_COORDINATE_INFO_T stUserInfo;

	if (stPkg.nType != 0)
	{
		stUserInfo.enumGameMap = (GAME_MAP_ENUM)stPkg.nMapIndex;
		stUserInfo.nCoordX = stPkg.nCoordX;
		stUserInfo.nCoordY = stPkg.nCoordY;
		
		// 更新服务端信息，更新完毕后返回
		hr = SetCoordinate(&stPkg.guidUser, &stUserInfo);
		return hr;
	}

	//服务端处理完毕，根据返回结果，发送回复至客户端
	UPDATE_COORDINATE_PKG_T stReply = { 0 };
	
	stReply.guidUser = stPkg.guidUser;
	hr = GetCoordinate(&stPkg.guidUser, &stUserInfo);
	CHECK_HR(hr);

	stReply.nMapIndex = stUserInfo.enumGameMap;
	stReply.nCoordX = stUserInfo.nCoordX;
	stReply.nCoordY = stUserInfo.nCoordY;

	cout << "user update bag" << "  return： " << (int)hr << endl;

done:
	// 发送回应
	SendReply(MSG_UPDATE_COORDINATE, &stReply, sizeof(stReply));

	return hr;
}

LRESULT CLogicLink::ProcUpdateUserProperty(const char* pDataBuf, int nDatalen)
{
	LRESULT hr = S_OK;
	assert(pDataBuf);
// 	assert(sizeof(UPDATE_USER_PROPERTY_PKG_T) == nDatalen);

	UPDATE_USER_PROPERTY_PKG_T stPkg = { 0 };
	memcpy(&stPkg, pDataBuf, nDatalen);

	USER_GAME_PROPERTY_T stUseProperty;

	if (stPkg.nType != 0)
	{
		stUseProperty.fUserMoney = stPkg.fMoney;
		stUseProperty.guidUser = stPkg.guidUser;
		stUseProperty.unBagType = stPkg.nBayType;
		stUseProperty.unUserLevel = stPkg.nLevel;

		// 更新服务端信息，更新完毕后返回
		hr = m_DBMgr.SetUserProperty(&stPkg.guidUser, &stUseProperty);
		cout << "user update property" << "  return： " << (int)hr << endl;
		cout << "level:" << stUseProperty.unUserLevel << " bagtype: " << stUseProperty.unBagType
			<< " money:" << stUseProperty.fUserMoney << endl;
		return hr;
	}

	//服务端处理完毕，根据返回结果，发送回复至客户端
	UPDATE_USER_PROPERTY_PKG_T stReply = { 0 };

	stReply.guidUser = stPkg.guidUser;
	hr = m_DBMgr.GetUserProperty(&stPkg.guidUser, &stUseProperty);
	CHECK_HR(hr);

	stReply.fMoney = stUseProperty.fUserMoney;
	stReply.guidUser = stUseProperty.guidUser;
	stReply.nBayType = stUseProperty.unBagType;
	stReply.nLevel = stUseProperty.unUserLevel;
	
	cout << "user update property" << "  return： " << (int)hr << endl;

done:
	// 发送回应
	SendReply(MSG_UPDATE_USER_PROPERTY, &stReply, sizeof(stReply));

	return hr;
}


LRESULT CLogicLink::ProcUpdateShop(const char* pDataBuf, int nDatalen)
{
	LRESULT hr = S_OK;
	assert(pDataBuf);
	assert(sizeof(UPDATE_SHOP_HEADER_PKG_T) == nDatalen);

	// 收到刷新商店请求
	UPDATE_SHOP_HEADER_PKG_T stUpdateHead = { 0 };
	MemOP_Read((void*)pDataBuf, &stUpdateHead, sizeof(stUpdateHead), 0);

	//回复客户端
	//将商店所有商品信息发送至客户端
	int nOffset = 0;
	int nSendDataLen = 0;
	char* pSendDataBuf = NULL;
	int nGoodsCount = 0;
	int nPkgsLen = 0;

	// Get goods length
	CGoodsMgr goodsMgr;
	m_DBMgr.GetShop(&goodsMgr);
	nGoodsCount = goodsMgr.GetGoodsCnt();

	nPkgsLen = sizeof(GOODS_INFO_PKG_T)*nGoodsCount;
	nSendDataLen = sizeof(UPDATE_SHOP_HEADER_PKG_T)+nPkgsLen;
	
	//分配内存，组装回应包
	pSendDataBuf = (char*)malloc(nSendDataLen);

	UPDATE_SHOP_HEADER_PKG_T stReplyDataHead = { 0 };
	stReplyDataHead.nGoodsCount = nGoodsCount;

	nOffset = MemOP_Write(pSendDataBuf, &stReplyDataHead, sizeof(stReplyDataHead), nOffset);

	for (int i = 0; i < nGoodsCount; i++)
	{
		GOODS_INFO_PKG_T stGoodsPkg = { 0 };

		GOODS_COMMON_INFO_T stGoodsComnInfo = { 0 };
		if(S_OK  != goodsMgr.GetGoodsInfoByIdx(i, &stGoodsComnInfo))
			continue;

		stGoodsPkg.fGoodsPrice = stGoodsComnInfo.fGoodsPrice;
		stGoodsPkg.guidGoods = stGoodsComnInfo.guidGoods;
		stGoodsPkg.uNum = 0;		//商店物品数量，这里不做处理，认为是无数个
		_tcscpy_s(stGoodsPkg.szGoodsDesc, sizeof(stGoodsPkg.szGoodsDesc) / sizeof(stGoodsPkg.szGoodsDesc[0]), stGoodsComnInfo.szGoodsDesc);
		_tcscpy_s(stGoodsPkg.szGoodsName, sizeof(stGoodsPkg.szGoodsName) / sizeof(stGoodsPkg.szGoodsName[0]), stGoodsComnInfo.szGoodsName);

		nOffset = MemOP_Write(pSendDataBuf, &stGoodsPkg, sizeof(stGoodsPkg), nOffset);
	}

	SendReply(MSG_UPDATE_SHOP, pSendDataBuf, nSendDataLen);

	cout << "user update shop" << "  return： " << (int)hr << endl;

	SAFE_FREE(pSendDataBuf);
	return hr;
}

LRESULT CLogicLink::ProcUpdateUserBag(const char* pDataBuf, int nDatalen)
{
	LRESULT hr = S_OK;
	assert(pDataBuf);
	assert(sizeof(UPDATE_BAG_HEADER_PKG_T) == nDatalen);
	int nOffset = 0;
	int nSendDataLen = 0;
	char* pSendDataBuf = NULL;
	int nGoodsCount = 0;
	int nPkgsLen = 0;

	// 收到刷新背包请求
	UPDATE_BAG_HEADER_PKG_T stUpdateHead = { 0 };
	MemOP_Read((void*)pDataBuf, &stUpdateHead, sizeof(stUpdateHead), 0);

	//////////////////////////////////////////////////////////////////////////
	//数据库中找到数据
	// Get goods length
	CGoodsMgr goodsMgr;
	m_DBMgr.GetUserBag(&stUpdateHead.guidUser, &goodsMgr);
	nGoodsCount = goodsMgr.GetGoodsCnt();

	nPkgsLen = sizeof(GOODS_INFO_PKG_T)*nGoodsCount;
	nSendDataLen = sizeof(UPDATE_BAG_HEADER_PKG_T)+nPkgsLen;

	//回复客户端
	//将用户所有背包物品发送给客户端
	//分配内存，组装回应包
	pSendDataBuf = (char*)malloc(nSendDataLen);

	UPDATE_BAG_HEADER_PKG_T stReplyDataHead = { 0 };
	stReplyDataHead.nGoodsCount = nGoodsCount;
	stReplyDataHead.guidUser = stUpdateHead.guidUser;

	nOffset = MemOP_Write(pSendDataBuf, &stReplyDataHead, sizeof(stReplyDataHead), nOffset);
	for (int i = 0; i < nGoodsCount; i++)
	{
		GOODS_INFO_PKG_T stGoodsPkg = { 0 };

		GOODS_COMMON_INFO_T stGoodsComnInfo = { 0 };
		if (S_OK != goodsMgr.GetGoodsInfoByIdx(i, &stGoodsComnInfo))
			continue;

		stGoodsPkg.fGoodsPrice = stGoodsComnInfo.fGoodsPrice;
		stGoodsPkg.guidGoods = stGoodsComnInfo.guidGoods;
		stGoodsPkg.uNum = stGoodsComnInfo.unGoodsCnt;		//商店物品数量，这里不做处理，认为是无数个
		_tcscpy_s(stGoodsPkg.szGoodsDesc, sizeof(stGoodsPkg.szGoodsDesc) / sizeof(stGoodsPkg.szGoodsDesc[0]), stGoodsComnInfo.szGoodsDesc);
		_tcscpy_s(stGoodsPkg.szGoodsName, sizeof(stGoodsPkg.szGoodsName) / sizeof(stGoodsPkg.szGoodsName[0]), stGoodsComnInfo.szGoodsName);

		nOffset = MemOP_Write(pSendDataBuf, &stGoodsPkg, sizeof(stGoodsPkg), nOffset);
	}
	SendReply(MSG_UPDATE_USER_BAG, pSendDataBuf, nSendDataLen);

	TCHAR szUserGuid[MIN_GUID_STR_LEN] = { 0 };
	CrossPlatform_GUID2String(stUpdateHead.guidUser, szUserGuid, MIN_GUID_STR_LEN);
	cout << "user :( " << szUserGuid  <<" )update bag" << "  return： " << (int)hr << endl;
	SAFE_FREE(pSendDataBuf);
	return hr;
}

LRESULT CLogicLink::GetCoordinate(const COS_GUID *pUserGuid, LPUSER_COORDINATE_INFO_T lpUserCoordInfo)
{
	LRESULT hr = S_OK;

	hr = m_DBMgr.GetUserCoordinate(pUserGuid, lpUserCoordInfo);
		//////////////////////////////////////////////////////////////////////////
		//test
		// 	lpUserCoordInfo->enumGameMap = GAME_MAP_TEST_1;
		// 	lpUserCoordInfo->nCoordX = 23;
		// 	lpUserCoordInfo->nCoordY = 56;
		//////////////////////////////////////////////////////////////////////////
	cout << "user get coordinate" << "  return： " << (int)hr << endl;

	return hr;
}

LRESULT CLogicLink::SetCoordinate(const COS_GUID *pUserGuid, LPUSER_COORDINATE_INFO_T lpUserCoordInfo)
{
	LRESULT hr = S_OK;
	hr = m_DBMgr.SetUserCoordinate(pUserGuid, lpUserCoordInfo);

	cout << "user update coordinate" << "  return： " << (int)hr << endl;
	cout << "coordinate info：" << "x:" << lpUserCoordInfo->nCoordX << " y:" << lpUserCoordInfo->nCoordY << " map:" << (int)lpUserCoordInfo->enumGameMap << endl;
	return hr;
}