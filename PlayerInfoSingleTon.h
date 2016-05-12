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

	// �������� ���õ� ��
	int weaponSeleted = 0;
	int trapSeleted = -1;
	int helperSeleted = -1;

	//�Ѿ� ����
	int pistol_Damage = 50;
	float plstol_Rate = 0.15;

	int ak_Damage = 100;
	float ak_Rate = 0.5;

};

#endif