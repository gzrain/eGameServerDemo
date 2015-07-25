#include "BillingLink.h"
#include "BillingSrv.h"

CBillingSrv::CBillingSrv()
{
}

CBillingSrv::~CBillingSrv()
{
}

void CBillingSrv::Destroy()
{
	CBaseSrv::Destroy();
}

LRESULT CBillingSrv::LinkMgrProc(UINT unAccSocket)
{
	LRESULT hr = S_OK;

	//¼ÓËø
	CAutoLock lock(&m_csLinkArray);
	CBillingLink *pLink = new CBillingLink();
	hr = pLink->Init(m_pParent, unAccSocket, ErrProc_CB);
	CHECK_HR(hr);
	pLink->StartRecv();
	m_vecLinkArray.push_back(pLink);

done:
	if (FAILED(hr))
		SAFE_DELETE(pLink);
	return hr;
}
