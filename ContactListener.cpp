#include "ContactListener.h"
#include "HelloWorldScene.h"
#include "DataSingleTon.h"
ContactListener::ContactListener() {
	_world = DataSingleTon::getInstance()->get_world();
	gameLayer = DataSingleTon::getInstance()->getGameLayer();
	monsters = DataSingleTon::getInstance()->getMonsters();
	removeBullets = DataSingleTon::getInstance()->getRemoveBullets();
}

ContactListener::~ContactListener() {}

void ContactListener::BeginContact(b2Contact *contact)
{
//	log("Contact:Begin");

	b2Fixture *fixA = contact->GetFixtureA();
	b2Fixture *fixB = contact->GetFixtureB();

	b2Body *bodyA = fixA->GetBody();
	b2Body *bodyB = fixB->GetBody();

	auto spriteA = (Sprite*)bodyA->GetUserData();
	auto spriteB = (Sprite*)bodyB->GetUserData();

	if (spriteA != nullptr && spriteB != nullptr) {
		if (spriteA->getTag() == MONSTER && spriteB->getTag() == BULLET) {

			removeBullets->push_back(bodyB);
			//	log("둘이 충돌");
			for (int i = 0; i < monsters->size(); i++)
			{
				b2Body * m_body = (b2Body* )monsters->at(i)->body;
				if (m_body == bodyA)
				{
					log("몬스터 HP : %d", monsters->at(i)->hp);
					monsters->at(i)->hp = monsters->at(i)->hp - 50;
					log("몬스터 HP : %d", monsters->at(i)->hp);
				}
			}
		}

	}
	else if (spriteA == nullptr && spriteB->getTag() == BULLET)
	{
		//log("벽이랑");
		removeBullets->push_back(bodyB);
	}

}



void ContactListener::PreSolve(b2Contact *contact, const b2Manifold *oldManifold)
{
//	log("Contact:PreSolve");
}
void ContactListener::PostSolve(b2Contact *contact, const b2ContactImpulse *impulse)
{
//	log("Contact:PostSolve .. 1");

	b2Fixture *fixA = contact->GetFixtureA();
	b2Fixture *fixB = contact->GetFixtureB();

	b2Body *bodyA = fixA->GetBody();
	b2Body *bodyB = fixB->GetBody();


	if (bodyA->GetType() == b2_dynamicBody || bodyB->GetType() == b2_dynamicBody)
	{
		//log("Contact:impulse .. %f", impulse->normalImpulses[0]);
	}
}

void ContactListener::EndContact(b2Contact *contact)
{

}