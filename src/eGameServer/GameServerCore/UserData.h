// UserData.h
// by gaozhe
// 20150705
#ifndef _USER_DATA_H_
#define _USER_DATA_H_

#ifdef GAMESERVERCORE_EXPORTS
#define USER_DATA_API __declspec(dllexport)
#else
#define USER_DATA_API __declspec(dllimport)
#endif


#include "BagData.h"
#include "UserCoordinate.h"
#define MAX_USER_LEVEL			100
#define MAX_USER_MONEY			1000000


//////////////////////////////////////////////////////////////////////////
// ����˻���Ϣ
typedef struct _user_account_t
{
	COS_GUID guidUser;
	TCHAR szUserName[MAX_USER_NAME_LEN];
	TCHAR szUserAlias[MAX_USER_ALIAS_LEN];
	time_t	tmLastLogin;

	_user_account_t()
	{
		guidUser = GUID_NULL;
		memset(szUserName, 0, sizeof(szUserName));
		memset(szUserAlias, 0, sizeof(szUserAlias));
		tmLastLogin = 0;
	}

}USER_ACCOUNT_T, *LPUSER_ACCOUNT_T;


//////////////////////////////////////////////////////////////////////////
// �����Ϸ����
typedef struct _user_game_property_t
{
	COS_GUID	guidUser;							//	���Ψһ��ʶ
	unsigned short unUserLevel;			//	��ҵȼ�
	unsigned short unBagType;			//	��ұ�������
	float fUserMoney;
	//����
	//����
}USER_GAME_PROPERTY_T, *LPUSER_GAME_PROPERTY_T;


class USER_DATA_API CUserData
{
public:
	CUserData();
	virtual ~CUserData();

public:
	/*
	*����˻�����
	*/
	void SetUserAccount(LPUSER_ACCOUNT_T pstUserData);
	HRESULT GetUserAccount(LPUSER_ACCOUNT_T pstUserDataOut);

	/*
	*�����Ϸ����
	*/
	unsigned short GetUserLevel();
	LRESULT SetUserLevel(unsigned short nLevel);

// 	CBagMgr* GetUserBag();
// 	LRESULT SetUserBag(const CBagMgr* pBagMgr);
// 
// 	CUserCoordinate *GetUserCoordInfo();
// 	LRESULT SetUserCoordInfo(const CUserCoordinate* pUserCoordInfo);


private:
	USER_ACCOUNT_T	m_stUserData;
	USER_GAME_PROPERTY_T m_stUserGameProperty;
};

#endif