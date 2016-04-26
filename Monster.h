#ifndef __Monster__H__
#define __Monster__H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"
USING_NS_CC;

using namespace cocos2d;

class Monster : public cocos2d::Sprite
{
public:
	//static Node* create1();
	
	Monster(Vec2 position);
	b2Body* addNewSprite(Vec2 point, Size size, b2BodyType bodytype, int type);
	void moving(float dt);
	b2Body* body;
	//virtual bool init();
	virtual void onEnter();
	virtual void onExit();

private:
	cocos2d::LayerColor* gameLayer;
	int hp;
	Vec2 position;
	float xSpeed;
	b2World * _world;
};

#endif /* defined(__SpriteExtendEx_Monster__)*/