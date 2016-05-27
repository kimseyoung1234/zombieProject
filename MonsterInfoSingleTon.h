#ifndef __MONSTER_INFO_SINGLETON__H__
#define __MONSTER_INFO_SINGLETON__H__
#include "cocos2d.h"


// 현재 레벨과 몬스터들의 정보를 가지고 있는 싱글톤 클래스
class MonsterInfoSingleTon {
private:
	static MonsterInfoSingleTon *m_pInstance;
	MonsterInfoSingleTon();
	~MonsterInfoSingleTon();
public:
	static MonsterInfoSingleTon *getInstance();
	static void FreeInstance();
	int level = 1;
	int maxMonster = 50;

	int fatZombie_HP = 150;
	int fatZombie_damage = 5;
	float fatZombie_xSpeed = 0.15;
	float fatZomie_ySpeed = 0.1;
	float fatZombie_reward = 10;

	int brainZombie_HP = 100;
	int brainZombie_damage = 2;
	float brainZombie_xSpeed = 0.17;
	float brainZomie_ySpeed = 0.1;
	float brainZombie_reward = 10;

	int superZombie_HP = 120;
	int superZombie_damage = 3;
	float superZombie_xSpeed = 0.2;
	float superZomie_ySpeed = 0.1;
	float superZombie_reward = 10;

	int bossZombie_HP = 1000;
	int bossZombie_damage = 10;
	float bossZombie_xSpeed = 20.0;
	float bossZomie_ySpeed = 20.0;
	float bossZombie_reward = 10;

	void level_up();
};

#endif