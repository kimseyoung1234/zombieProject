#ifndef __Bullet__H__
#define __Bullet__H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"

USING_NS_CC;

//using namespace cocos2d;

class Bullet : public cocos2d::Node
{
public:

	Bullet(Vec2 position, int bulletType);
	~Bullet();
	b2Body* addNewSprite(Vec2 point, Size size, b2BodyType bodytype, int type);
	b2Body* body;
	int damage = 100;
	bool isRemove = false;
private:
	cocos2d::LayerColor* gameLayer;
	b2World * _world;
	int bulletType;

};

#endif /* defined(__SpriteExtendEx_Monster__)*/