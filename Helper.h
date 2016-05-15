#ifndef __HELPER__H__
#define __HELPER__H__

#include "cocos2d.h"
#include "Monster.h"
#include "Bullet.h"
class Helper : public cocos2d::Sprite {
public:

	Helper(cocos2d::Vec2 position, int type);

	cocos2d::Sprite * sprite;
	int type;
	float attackDelayTime = 0;

	cocos2d::LayerColor* gameLayer;
	std::vector<Monster *>* monsters;
	std::vector<Bullet *>* bullets;
	float attackRate;

	void re_Idle();
	
	void autoAttack(float dt);
	virtual void onEnter();
	virtual void onExit();

};

#endif /* defined(__SpriteExtendEx_Monster__)*/