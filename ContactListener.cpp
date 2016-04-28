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

	bool isPushBullet = true;
	b2Fixture *fixA = contact->GetFixtureA();
	b2Fixture *fixB = contact->GetFixtureB();

	b2Body *bodyA = fixA->GetBody();
	b2Body *bodyB = fixB->GetBody();

	auto spriteA = (Sprite*)bodyA->GetUserData();
	auto spriteB = (Sprite*)bodyB->GetUserData();
	// ���ϰ� ���� �浹 �� 
	if (spriteA != nullptr && spriteB != nullptr) {
		if (spriteA->getTag() == MONSTER && spriteB->getTag() == BULLET) {

			// ���� �Ѿ��� ���Ϳ� �ߺ����� �ʰ� �ϱ� ����
			for (int i = 0; i<removeBullets->size(); i++)
			{
				if (bodyB == removeBullets->at(i))
				{
					isPushBullet = false;
				}
			}
			if (isPushBullet) {
				log("���ؽ� �ҷ� : %d", removeBullets->size());
				removeBullets->push_back(bodyB);
			}

			//  �Ѿ˰� ���� �浹 �� HP ���� ó��
			for (int i = 0; i < monsters->size(); i++)
			{
				b2Body * m_body = (b2Body* )monsters->at(i)->body;
				if (m_body == bodyA)
				{
					log("���� HP : %d", monsters->at(i)->hp);
					monsters->at(i)->hp = monsters->at(i)->hp - 50;
					log("���� HP : %d", monsters->at(i)->hp);
				}
			}
		}

	}

	// �Ѿ˰� ���� �� �浹 �� �Ѿ� ���� 
	if (spriteA->getTag() == WORLD && spriteB->getTag() == BULLET)
	{
		for (int i = 0; i <removeBullets->size(); i++)
		{
			if (bodyB == removeBullets->at(i))
			{
				isPushBullet = false;
			}
		}
		if (isPushBullet) {
			removeBullets->push_back(bodyB);
		}
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