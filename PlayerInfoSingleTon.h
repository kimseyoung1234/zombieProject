#ifndef __PLAYER_INFO_SINGLETON__H__
#define __PLAYER_INFO_SINGLETON__H__
#include "cocos2d.h"


// Player 정보와 무기들의 정보를 가지고 있는 싱글톤 클래스
class PlayerInfoSingleTon {
private:
	static PlayerInfoSingleTon *m_pInstance;
	PlayerInfoSingleTon();
	~PlayerInfoSingleTon();

public:
	static PlayerInfoSingleTon *getInstance();
	static void FreeInstance();

	// 플레이어 hp
	int hp = 10;
	int money_In_Hand = 30000;

	// 가지고있는 트랩 수
	int have_trap1 = 0;
	int have_trap2 = 0;
	
	// 가지고있는 도우미 수
	int have_helper = 0;

	// 상점에서 선택된 것
	int weaponSeleted = 0;
	int trapSeleted = -1;
	int helperSeleted = -1;

	//총알 정보
	int machine_Damage = 10;
	float machine_Rate = 0.15;
	int machine_price = 100;
	int machine_level = 0;


	int ak_Damage = 40;
	float ak_Rate = 0.4;
	int ak_price = 200;
	int ak_level = 0;


	int sniper_Damage = 100;
	float sniper_Rate = 0.6;
	int sniper_price = 150;
	int sniper_level = 0;



	int bazooka_Damage = 70;
	float bazooka_Rate = 2.0f;
	// 트랩 정보
	int trap1_Damage = 50;
	float trap1_blastRadius = 5.0f;
	int trap1_price = 100;

	int trap2_Damge = 0;
	float trap2_blastRadius = 8.0f;
	int trap2_price = 150;

	// 도우미 정보
	int helper1_price = 90;
	int helper2_price = 120;

	//스킬정보
	float skill_cooltime = 3.0f;
	float skill_blastRadius = 5.0f;

	float skill2_cooltime = 5.0f;
	float skill2_blastRadius = 8.0f;

};

#endif