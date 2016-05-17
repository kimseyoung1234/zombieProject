#ifndef __PLAYER_INFO_SINGLETON__H__
#define __PLAYER_INFO_SINGLETON__H__
#include "cocos2d.h"


// Player ������ ������� ������ ������ �ִ� �̱��� Ŭ����
class PlayerInfoSingleTon {
private:
	static PlayerInfoSingleTon *m_pInstance;
	PlayerInfoSingleTon();

public:
	static PlayerInfoSingleTon *getInstance();

	// �÷��̾� hp
	int hp = 100;
	int money_In_Hand = 500;

	// �������� ���õ� ��
	int weaponSeleted = 0;
	int trapSeleted = -1;
	int helperSeleted = -1;

	//�Ѿ� ����
	int machine_Damage = 10;
	float machine_Rate = 0.1;
	int machine_price = 100;

	int ak_Damage = 40;
	float ak_Rate = 0.4;
	int ak_price = 200;

	int sniper_Damage = 100;
	float sniper_Rate = 0.6;
	int sniper_price = 150;

	int bazooka_Damage = 70;
	float bazooka_Rate = 2.0f;
	// Ʈ�� ����
	int trap1_Damage = 50;
	float trap1_blastRadius = 5.0f;
	int trap1_price = 100;

	int trap2_Damge = 0;
	float trap2_blastRadius = 8.0f;
	int trap2_price = 150;

	// ����� ����
	int helper1_price = 90;

	int helper2_price = 120;

};

#endif