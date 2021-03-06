#ifndef __Monster__H__
#define __Monster__H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"

USING_NS_CC;
#define FatZombie 1
#define BrainZombie 2
#define SuperZombie 3
#define BossZombie 4

#define MOVE 10
#define ATTACK 20
#define HIT 30
#define ATTACKFINISH 40

//using namespace cocos2d;

class Monster : public cocos2d::Node
{
public:

	Monster(Vec2 position,int monsterType);
	~Monster();
	b2Body* addNewSprite(Vec2 point, Size size, b2BodyType bodytype, int type);
	void attackFinish();
	void finish_hit();
	void remove_anim(Node* sender);
	
	b2Body* body;
	cocos2d::Sprite* sprite;
	cocos2d::Sprite* hpBar;
	Animate * moveAnimate;
	Animate * attackAnimate;
	Animate * attack2Animate;

	int hp;
	int damage;
	int monsterType;
	int reward;

	float stiffenTime = 0.0;
	float attackDelay = 1.0;
	float hpBarShowTime = 0.0;
	float slowTime = 0.0;
	float pipeTime = 0;
	int present_ani = MOVE;
	Vec2 pipe_positon;

	bool isAttack = false;
	bool isHit = false;
	bool isMotion = false;
	bool isSlow = false;
	bool isPipe = false;

	bool isRight = false;
	bool isLeft = true;
	b2Body * hitBullet = nullptr;
	float ySpeed;
private:
	cocos2d::LayerColor* gameLayer;
	b2World * _world;
	cocos2d::Vec2 position;
	float xSpeed;
	
	
	
	void moving(float dt);
	
};

#endif /* defined(__SpriteExtendEx_Monster__)*/