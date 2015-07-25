#include "LoginLink.h"
#include "LoginSrv.h"

CLoginSrv::CLoginSrv()
{
	InitializeCriticalSection(&m_csUserArray);
}

CLoginSrv::~CLoginSrv()
{
	DeleteCriticalSection(&m_csUserArray);
}

void CLoginSrv::Destroy()
{
	CBaseSrv::Destroy();

	for (ONLINE_USERARRAY_ITER it = m_vecOnlineUserArray.begin(); it != m_vecOnlineUserArray.end(); it++)
	{
		SAFE_DELETE(*it);
	}

	m_vecOnlineUserArray.clear();
}

LRESULT CLoginSrv::LinkMgrProc(UINT unAccSocket)
{
	LRESULT hr = S_OK;

	//����
	CAutoLock lock(&m_csLinkArray);
	CLoginLink *pLink = new CLoginLink();
	hr = pLink->Init(m_pParent, unAccSocket, ErrProc_CB);
	CHECK_HR(hr);
	pLink->StartRecv();
	m_vecLinkArray.push_back(pLink);

done:
	if (FAILED(hr))
		SAFE_DELETE(pLink);
	return hr;
}

LRESULT CLoginSrv::AddToOnlineList(const COS_GUID *guidUser)
{
	LRESULT hr = S_OK;

	//����
	CAutoLock lock(&m_csLinkArray);
	ONLINE_USERARRAY_ITER it = m_vecOnlineUserArray.begin();
	USER_ACCOUNT_T stUser;

	for (it; it != m_vecOnlineUserArray.end(); it++)
	{
		CUserData *pUserData = (CUserData*)*it;
		
		USER_ACCOUNT_T stUserTemp;
		pUserData->GetUserAccount(&stUserTemp);

		if (stUserTemp.guidUser == *guidUser)
			break;
	}

	if (it != m_vecOnlineUserArray.end())
		hr = S_FALSE;		//�Ѿ��������б��У����ѵ�¼����ֱ�ӷ���
	CHECK_SOK(hr);

	stUser.guidUser = *guidUser;
	CUserData *pUser = new CUserData();
	pUser->SetUserAccount(&stUser);

	cout << "user online��"<< stUser.szUserAlias << endl;

	m_vecOnlineUserArray.push_back(pUser);
done:
	return hr;
}

LRESULT CLoginSrv::DeleteFromOnlineList(const COS_GUID *guidUser)
{
	LRESULT hr = E_FAIL;

	//����
	CAutoLock lock(&m_csLinkArray);
	ONLINE_USERARRAY_ITER it = m_vecOnlineUserArray.begin();

	for (it; it != m_vecOnlineUserArray.end(); it++)
	{
		CUserData *pUserData = (CUserData*)*it;

		USER_ACCOUNT_T stUserTemp;
		pUserData->GetUserAccount(&stUserTemp);

		// ���б����ҵ�
		if (stUserTemp.guidUser == *guidUser)
		{
			// ���ͷ���Դ������б���ɾ��
			SAFE_DELETE(pUserData);
			m_vecOnlineUserArray.erase(it);

			cout << "user offline��" << stUserTemp.szUserAlias << endl;

			hr = S_OK;
			break;
		}
	}

	return hr;
}

BOOL CLoginSrv::IsUserOnLine(const COS_GUID *guidUser)
{
	LRESULT hr = E_FAIL;
	CAutoLock lock(&m_csLinkArray);
	ONLINE_USERARRAY_ITER it = m_vecOnlineUserArray.begin();

	for (it; it != m_vecOnlineUserArray.end(); it++)
	{
		CUserData *pUserData = (CUserData*)*it;

		USER_ACCOUNT_T stUserTemp;
		pUserData->GetUserAccount(&stUserTemp);

		// ���б����ҵ�
		if (stUserTemp.guidUser == *guidUser)
		{
			hr = S_OK;
			break;
		}
	}

	return hr;

}