#include "ContactListener.h"
#include "HelloWorldScene.h"
#include "DataSingleTon.h"
ContactListener::ContactListener() {
	_world = DataSingleTon::getInstance()->get_world();
	gameLayer = DataSingleTon::getInstance()->getGameLayer();
	monsters = DataSingleTon::getInstance()->getMonsters();
	bullets = DataSingleTon::getInstance()->getBullets();
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

	//log("sprite A = %d , sprite B = %d", spriteA->getTag(), spriteB->getTag());
	// ���ϰ� ���� �浹 �� 
	if (spriteA != nullptr && spriteB != nullptr) {
		if (spriteA->getTag() == MONSTER && spriteB->getTag() == BULLET) {

			//  �Ѿ˰� ���� �浹 �� HP ���� ó��
			for (int i = 0; i < monsters->size(); i++)
			{
				b2Body * m_body = (b2Body* )monsters->at(i)->body;
				if (m_body == bodyA)
				{
					for (int k = 0; k < bullets->size(); k++)
					{
						b2Body * b_body = (b2Body*)bullets->at(k)->body;
							if (b_body == bodyB)
							{
								//log("���� HP : %d", monsters->at(i)->hp);
								monsters->at(i)->hp = monsters->at(i)->hp - bullets->at(k)->damage;
								bullets->at(k)->isRemove = true;
								//log("���� HP : %d", monsters->at(i)->hp);
								break;
							}
					}
				}
			}
		}
	}

	if (spriteA != nullptr && spriteB != nullptr) {
		// �Ѿ˰� ���� �� �浹 �� �Ѿ� ���� 
		if (spriteA->getTag() == WORLD && spriteB->getTag() == BULLET)
		{
			for (int k = 0; k < bullets->size(); k++)
			{
				b2Body * b_body = (b2Body*)bullets->at(k)->body;
				if (b_body == bodyB)
				{
					bullets->at(k)->isRemove = true;
					break;
				}
			}
		}
	}

	if (spriteA != nullptr && spriteB != nullptr) {
		//���Ϳ� �ٸ�����Ʈ �浹
		if (spriteA->getTag() == BARRICADE && spriteB->getTag() == MONSTER)
		{
			log("����");
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