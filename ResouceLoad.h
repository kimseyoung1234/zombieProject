#ifndef __RESOUCELOAD_SINGLETON__H__
#define __RESOUCELOAD_SINGLETON__H__
#include "cocos2d.h"

USING_NS_CC;
// 리소스 로딩
class ResouceLoad {
private:
	static ResouceLoad *m_pInstance;
	ResouceLoad();

public:
	static ResouceLoad *getInstance();
	void Loading();

	//플레이어
	Animate * player_idleAnimate;
	Animate * player_attackAnimate;
	Animate * player_deadAnimate;
	Animate * blood;

	//몬스터
	Animate * hit;
	Animate * brain_attackAnimate;
	Animate * brain_attack2Animate;
	Animate * brain_moveAnimate;

	Animate * fat_attackAnimate;
	Animate * fat_attack2Animate;
	Animate * fat_moveAnimate;

	Animate * super_attackAnimate;
	Animate * super_attack2Animate;
	Animate * super_moveAnimate;

	Animate * boss_moveAnimate;
	Animate * boss_attackAnimate;
	Animate * boss_attack2Animate;
	Animate * boss_bombAnimate;

	Animate * dieAnimate;
	// 폭파
	Animate * explosion1;
	Animate * explosion2;
	Animate * slow_effect;
	Animate * barricade_bomb;
	//도우미
	Animate * helper1_idle;
	Animate * helper1_attack;
	Animate * helper2_idle;
	Animate * helper2_attack;

};

#endif