#ifndef __Bullet__H__
#define __Bullet__H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include "Monster.h"
USING_NS_CC;

//using namespace cocos2d;

class Bullet : public cocos2d::Node
{
public:

	Bullet(Vec2 position, int bulletType,float angle);
	~Bullet();
	b2Body* addNewSprite(Vec2 point, Size size, b2BodyType bodytype, int type);
	b2Body* body;
	Sprite * sprite;
	int damage=0;
	bool isRemove = false;
	float angle;
	int bulletType;

	std::vector<Monster*>* monsters;
private:
	cocos2d::LayerColor* gameLayer;
	b2World * _world;

	void tick(float dt);
};

#endif /* defined(__SpriteExtendEx_Monster__)*/