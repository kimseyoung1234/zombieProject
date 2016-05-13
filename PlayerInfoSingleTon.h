#ifndef __PLAYER_INFO_SINGLETON__H__
#define __PLAYER_INFO_SINGLETON__H__
#include "cocos2d.h"


// Player 정보와 무기들의 정보를 가지고 있는 싱글톤 클래스
class PlayerInfoSingleTon {
private:
	static PlayerInfoSingleTon *m_pInstance;
	PlayerInfoSingleTon();

public:
	static PlayerInfoSingleTon *getInstance();

	// 플레이어 hp
	int hp = 100;

	// 상점에서 선택된 것
	int weaponSeleted = 0;
	int trapSeleted = -1;
	int helperSeleted = -1;

	//총알 정보
	int pistol_Damage = 50;
	float plstol_Rate = 0.1;

	int ak_Damage = 100;
	float ak_Rate = 0.5;

};

#endif