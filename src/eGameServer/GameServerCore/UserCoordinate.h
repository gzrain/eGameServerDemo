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
// 地图信息

#define MAX_COORD_X		100
#define MAX_COORD_Y		100

typedef enum _game_map_enum
{
	GAME_MAP_START = 0,
	GAME_MAP_TEST_1,					//测试地图1
	GAME_MAP_TEST_2,					//测试地图2
	GAME_MAP_TEST_3,					//测试地图3
	GAME_MAP_TEST_4,					//测试地图4
	GAME_MAP_TEST_5,					//测试地图5
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

	//重载操作符，方便赋值操作
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

	//供客户端使用
	//

private:
	USER_COORDINATE_INFO_T m_stCoordInfo;
};

#endif
