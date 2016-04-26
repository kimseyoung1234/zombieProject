#ifndef __Monster__H__
#define __Monster__H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"
USING_NS_CC;

using namespace cocos2d;

class Monster : public cocos2d::Node
{
public:
	Monster();

	b2Body* addNewSprite(Vec2 point, Size size, b2BodyType bodytype, int type);

	b2Body* body;
	b2World * _world;
private:
	int hp;
	int speed;
};

#endif /* defined(__SpriteExtendEx_Monster__)*/