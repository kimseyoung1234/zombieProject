#ifndef __MONSTER_INFO_SINGLETON__H__
#define __MONSTER_INFO_SINGLETON__H__
#include "cocos2d.h"


// 모든 클래스에서 사용하기 위한 싱글톤 클래스
class MonsterInfoSingleTon {
private:
	static MonsterInfoSingleTon *m_pInstance;
	MonsterInfoSingleTon();

public:
	static MonsterInfoSingleTon *getInstance();

	int level = 1;
	int maxMonster = 1;

	int fatZombie_HP = 150;
	float fatZombie_xSpeed = 0.05;
	float fatZomie_ySpeed=0;

	int brainZombie_HP = 100;
	float brainZombie_xSpeed = 0.06;
	float brainZomie_ySpeed = 0.03;

	int superZombie_HP = 120;
	float superZombie_xSpeed = 0.07;
	float superZomie_ySpeed = 0;

	void level_up();

};

#endif