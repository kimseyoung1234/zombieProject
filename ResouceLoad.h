#ifndef __RESOUCELOAD_SINGLETON__H__
#define __RESOUCELOAD_SINGLETON__H__
#include "cocos2d.h"

USING_NS_CC;
// ���ҽ� �ε�
class ResouceLoad {
private:
	static ResouceLoad *m_pInstance;
	ResouceLoad();

public:
	static ResouceLoad *getInstance();
	void Loading();

	//�÷��̾�
	Animate * player_idleAnimate;
	Animate * player_attackAnimate;

	//����
	Animate * brain_attackAnimate;
	Animate * brain_moveAnimate;

	Animate * fat_attackAnimate;
	Animate * fat_moveAnimate;

	Animate * super_attackAnimate;
	Animate * super_moveAnimate;

	Animate * boss_moveAnimate;
	Animate * boss_attackAnimate;
	Animate * boss_attack2Animate;

	Animate * dieAnimate;
	// ����
	Animate * explosion1;
	Animate * pipe_move;
	//�����
	Animate * helper1_idle;
	Animate * helper1_attack;
	Animate * helper2_idle;
	Animate * helper2_attack;

};

#endif