// UserData.h
// by gaozhe
// 20150703
#ifndef _USER_COORDINATE_H_
#define _USER_COORDINATE_H_

#ifdef GAMESERVERCORE_EXPORTS
#define USER_COORDINATE_API __declspec(dllexport)
#else
#define USER_COORDINATE_API __declspec(dllimport)
#endif
#pragma warning( disable: 4251 )

#include <windows.h>
// ��ͼ��Ϣ

#define MAX_COORD_X		100
#define MAX_COORD_Y		100

typedef enum _game_map_enum
{
	GAME_MAP_START = 0,
	GAME_MAP_TEST_1,					//���Ե�ͼ1
	GAME_MAP_TEST_2,					//���Ե�ͼ2
	GAME_MAP_TEST_3,					//���Ե�ͼ3
	GAME_MAP_TEST_4,					//���Ե�ͼ4
	GAME_MAP_TEST_5,					//���Ե�ͼ5
	GAME_MAP_END
}GAME_MAP_ENUM;

typedef struct _user_coordinate_info_t
{
	GAME_MAP_ENUM enumGameMap;
	int nCoordX;
	int nCoordY;

	_user_coordinate_info_t()
	{
		enumGameMap = GAME_MAP_START;
		nCoordY = 0;
		nCoordY = 0;
	}

	//���ز����������㸳ֵ����
	_user_coordinate_info_t & operator = (_user_coordinate_info_t *puser_coord_info)
	{
		this->enumGameMap = puser_coord_info->enumGameMap;
		this->nCoordX = puser_coord_info->nCoordX;
		this->nCoordY = puser_coord_info->nCoordY;
		return *this;
	}
}USER_COORDINATE_INFO_T, *LPUSER_COORDINATE_INFO_T;

class USER_COORDINATE_API CUserCoordinate
{
public:
	CUserCoordinate();
	~CUserCoordinate();

public:
	LRESULT SetUserCoordInfo(LPUSER_COORDINATE_INFO_T lpCoordInfo);
	LRESULT GetUserCoodinfo(USER_COORDINATE_INFO_T &stCoordInfo);

	//���ͻ���ʹ��
	//

private:
	USER_COORDINATE_INFO_T m_stCoordInfo;
};

#endif
