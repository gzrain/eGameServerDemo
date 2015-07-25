#include "LogicLink.h"
#include "LogicSrv.h"

CLogicSrv::CLogicSrv()
{
}

CLogicSrv::~CLogicSrv()
{
}

void CLogicSrv::Destroy()
{
	CBaseSrv::Destroy();
}

LRESULT CLogicSrv::LinkMgrProc(UINT unAccSocket)
{
	LRESULT hr = S_OK;

	//¼ÓËø
	CAutoLock lock(&m_csLinkArray);
	CLogicLink *pLink = new CLogicLink();
	hr = pLink->Init(m_pParent, unAccSocket, ErrProc_CB);
	CHECK_HR(hr);
	pLink->StartRecv();
	m_vecLinkArray.push_back(pLink);

done:
	if (FAILED(hr))
		SAFE_DELETE(pLink);
	return hr;
}
