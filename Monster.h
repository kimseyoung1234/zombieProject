#ifndef __Monster__H__
#define __Monster__H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"

USING_NS_CC;
#define FatZombie 1
#define BrainZombie 2
#define SuperZombie 3

#define MOVE 10
#define ATTACK 20
#define DIE 30
//using namespace cocos2d;

class Monster : public cocos2d::Node
{
public:

	Monster(Vec2 position,int monsterType);
	~Monster();
	b2Body* addNewSprite(Vec2 point, Size size, b2BodyType bodytype, int type);
	b2Body* body;
	cocos2d::Sprite* sprite;
	cocos2d::Sprite* hpBar;

	int hp;
	int damage;
	bool isAttack = false;
	float attackDelay = 1.0;
	float hpBarShowTime = 0.0;
	float slowTime = 0.0;
	int present_ani = MOVE;
	Animate * moveAnimate;
	Animate * attackAnimate;

	bool isSlow = false;
private:
	cocos2d::LayerColor* gameLayer;
	b2World * _world;
	cocos2d::Vec2 position;
	float xSpeed;
	float ySpeed;
	int monsterType;
	float yTurnTime = 0;
	
	void moving(float dt);
	
};

#endif /* defined(__SpriteExtendEx_Monster__)*/