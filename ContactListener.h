#ifndef __ContactListener__
#define __ContactListener__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "Monster.h"
#include "Bullet.h"
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

	void trigger(Vec2 position, float blastRadius, int type, float blastPower);
	void applyBlastImpulse(b2Body* body, b2Vec2 blastCenter, b2Vec2 applyPoint, float blastPower);
	void remove_anim(Node* sender);
	b2World* _world;
	//b2Body* barricade;
	cocos2d::LayerColor* gameLayer;
	std::vector<Monster*>* monsters;
	std::vector<Bullet *>* bullets;
	std::vector<b2Body*>* barricade;
protected:
	
};

#endif // __HELLOWORLD_SCENE_H__
