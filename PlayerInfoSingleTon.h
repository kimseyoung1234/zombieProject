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

	int hp = 100;

	// �������� ���õ� ��
	int weaponSeleted = -1;
	int trapSeleted = -1;
	int helperSeleted = -1;

};

#endif