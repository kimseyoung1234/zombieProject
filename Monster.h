#ifndef __Monster__H__
#define __Monster__H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"

USING_NS_CC;
#define FatZombie 1
#define BrainZombie 2
#define SuperZombie 3

//using namespace cocos2d;

class Monster : public cocos2d::Node
{
public:

	Monster(Vec2 position,int monsterType);
	~Monster();
	b2Body* addNewSprite(Vec2 point, Size size, b2BodyType bodytype, int type);
	void moving(float dt);
	b2Body* body;
	int hp;
	cocos2d::Sprite* hpBar;
	virtual void onEnter();
	virtual void onExit();
private:
	cocos2d::LayerColor* gameLayer;
	b2World * _world;
	cocos2d::Vec2 position;
	float xSpeed;
	float ySpeed;
	int monsterType;
	float yTurnTime = 0;
	
};

#endif /* defined(__SpriteExtendEx_Monster__)*/