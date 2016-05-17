#include "ContactListener.h"
#include "HelloWorldScene.h"
#include "DataSingleTon.h"
#include "PlayerInfoSingleTon.h"
#include "MyQueryCallback.h"
#include "ResouceLoad.h"
ContactListener::ContactListener() {
	_world = DataSingleTon::getInstance()->get_world();
	gameLayer = DataSingleTon::getInstance()->getGameLayer();
	monsters = DataSingleTon::getInstance()->getMonsters();
	bullets = DataSingleTon::getInstance()->getBullets();
	barricade = DataSingleTon::getInstance()->getBarricade();
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
								// ����� ����ī �Ѿ�
								if (bullets->at(k)->bulletType == 3) {
									Vec2 position = Vec2(b_body->GetPosition().x * PTM_RATIO, b_body->GetPosition().y * PTM_RATIO);
									trigger(position, 3.0f, 0, 20);
									bullets->at(k)->isRemove = true;

									// ���� �ִϸ��̼�
									auto cache = SpriteFrameCache::getInstance();
									cache->addSpriteFramesWithFile("explosion/ExplosionPlist.plist");

									auto exp = Sprite::createWithSpriteFrameName("explosion_10002.png");
									exp->setPosition(bullets->at(k)->sprite->getPosition());
									exp->setScale(1.7f);
									gameLayer->addChild(exp, 200);

									auto explosion1 = ResouceLoad::getInstance()->explosion1->clone();
									auto rep = Sequence::create(explosion1,
										CallFunc::create(CC_CALLBACK_0(ContactListener::remove_anim, this, exp)), nullptr);
									exp->runAction(rep);
								}
								else {

									monsters->at(i)->hp = monsters->at(i)->hp - bullets->at(k)->damage;							
									monsters->at(i)->hpBar->setVisible(true);
									monsters->at(i)->hpBarShowTime = 0;
									bullets->at(k)->isRemove = true;

									monsters->at(i)->isHit = true;
									monsters->at(i)->stiffenTime = 0;
									break;
								}
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
		//�浹������ ������ ������ ���θ� true�� �ٲ�
		if (spriteA->getTag() == BARRICADE && spriteB->getTag() == MONSTER)
		{
			for (int i = 0; i < monsters->size(); i++)
			{
				b2Body * m_body = (b2Body*)monsters->at(i)->body;
				if (m_body == bodyB)
				{
					monsters->at(i)->isAttack = true;
					break;
				}
			}
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
	//	log("Contact:impulse .. %f", impulse->normalImpulses[0]);
	}
}

void ContactListener::EndContact(b2Contact *contact)
{
	b2Fixture *fixA = contact->GetFixtureA();
	b2Fixture *fixB = contact->GetFixtureB();

	b2Body *bodyA = fixA->GetBody();
	b2Body *bodyB = fixB->GetBody();
	
	auto spriteA = (Sprite*)bodyA->GetUserData();
	auto spriteB = (Sprite*)bodyB->GetUserData();

	//���Ϳ� �ٸ�����Ʈ �浹 �������� ��
	
	auto _barricade = (b2Body*)barricade->at(0);
	if (_barricade == bodyA){
		for (int i = 0; i < monsters->size(); i++)
		{
			b2Body * m_body = (b2Body*)monsters->at(i)->body;
			if (m_body == bodyB)
			{
				monsters->at(i)->isAttack = false;
				break;
			}
		}
	}
}

// type 0: ����ȿ�� 1: ���ο�ȿ��
void ContactListener::trigger(Vec2 position, float blastRadius, int type, float blastPower)
{
	log("x : %f  y : %f", position.x, position.y);
	MyQueryCallback queryCallback; //see "World querying topic"
	b2AABB aabb;
	// center : ��ź �߽� ��ġ
	b2Vec2 center = b2Vec2(position.x / PTM_RATIO, position.y / PTM_RATIO);
	// ���� ����
	//float blastRadius = 5.0;
	// ���� �ٿ���ڽ� ��ġ�� ũ�� 
	aabb.lowerBound = center - b2Vec2(blastRadius, blastRadius);
	aabb.upperBound = center + b2Vec2(blastRadius, blastRadius);
	_world->QueryAABB(&queryCallback, aabb);

	//check which of these bodies have their center of mass within the blast radius
	for (int i = 0; i < queryCallback.foundBodies.size(); i++) {
		b2Body* body = queryCallback.foundBodies[i];
		b2Vec2 bodyCom = body->GetWorldCenter();

		//ignore bodies outside the blast range
		if ((bodyCom - center).Length() >= blastRadius)
		{
			continue;
		}
		for (int k = 0; k < monsters->size(); k++)
		{
			b2Body * m_body = (b2Body*)monsters->at(k)->body;
			if (body == m_body)
			{
				// ����ȿ��
				if (type == 0) {
					monsters->at(k)->hp = monsters->at(k)->hp - 90;
					monsters->at(k)->hpBar->setVisible(true);
					monsters->at(k)->hpBarShowTime = 0;
					monsters->at(k)->isHit = true;
					monsters->at(k)->stiffenTime = 0;
					applyBlastImpulse(body, center, bodyCom, blastPower);
					break;
				}
				else if (type == 1)
				{
					monsters->at(k)->isSlow = true;
					monsters->at(k)->slowTime = 0.0f;
					break;
				}
				else if (type == 2)
				{
					monsters->at(k)->isPipe = true;
					monsters->at(k)->pipe_positon = position;
					monsters->at(k)->pipeTime = 0.0f;
					break;
				}
			}
		}
	}
}

void ContactListener::applyBlastImpulse(b2Body* body, b2Vec2 blastCenter, b2Vec2 applyPoint, float blastPower)
{
	b2Vec2 blastDir = applyPoint - blastCenter;
	float distance = blastDir.Normalize();
	//ignore bodies exactly at the blast point - blast direction is undefined
	if (distance == 0)
		return;
	float invDistance = 1 / distance;
	float impulseMag = blastPower * invDistance * invDistance;
	body->ApplyLinearImpulse(impulseMag * blastDir, applyPoint, true);
}

void ContactListener::remove_anim(Node* sender)
{
	auto sprite = (Sprite*)sender;
	gameLayer->removeChild(sprite);
}