#ifndef __MONSTER_INFO_SINGLETON__H__
#define __MONSTER_INFO_SINGLETON__H__
#include "cocos2d.h"


// 현재 레벨과 몬스터들의 정보를 가지고 있는 싱글톤 클래스
class MonsterInfoSingleTon {
private:
	static MonsterInfoSingleTon *m_pInstance;
	MonsterInfoSingleTon();

public:
	static MonsterInfoSingleTon *getInstance();

	int level = 1;
	int maxMonster = 50;

	int fatZombie_HP = 150;
	int fatZombie_damage = 5;
	float fatZombie_xSpeed = 0.05;
	float fatZomie_ySpeed=0;

	int brainZombie_HP = 100;
	int brainZombie_damage = 2;
	float brainZombie_xSpeed = 0.06;
	float brainZomie_ySpeed = 0.03;

	int superZombie_HP = 120;
	int superZombie_damage = 3;
	float superZombie_xSpeed = 0.07;
	float superZomie_ySpeed = 0;

	void level_up();

};

#endif