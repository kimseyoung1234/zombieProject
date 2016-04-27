#ifndef __ContactListener__
#define __ContactListener__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "Monster.h"
using namespace cocos2d;

class ContactListener : public b2ContactListener
{
public:
	ContactListener();
	~ContactListener();

	virtual void BeginContact(b2Contact *contact);
	virtual void EndContact(b2Contact *contact);
	virtual void PreSolve(b2Contact *contact,const b2Manifold *oldManifold);
	virtual void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse);


	b2World* _world;
	cocos2d::LayerColor* gameLayer;
	std::vector<Monster*>* monsters;
	std::vector<b2Body *>* removeBodys;
protected:
	
};

#endif // __HELLOWORLD_SCENE_H__
