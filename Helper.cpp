#include "Helper.h"
#include "DataSingleTon.h"
#include "ResouceLoad.h"
#include "PlayerInfoSingleTon.h"
#include "MyQueryCallback.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
USING_NS_CC;

Helper::Helper(Vec2 position, int type)
{
	bool bOk = initWithTexture(nullptr, Rect::ZERO);
	if (bOk)
	{
		this->autorelease();
	}

	gameLayer = DataSingleTon::getInstance()->getGameLayer();
	monsters = DataSingleTon::getInstance()->getMonsters();
	bullets = DataSingleTon::getInstance()->getBullets();
	_world = DataSingleTon::getInstance()->get_world();
	this->type = type;

	if (type == 0) {
		auto sprite = Sprite::create("helper/helper1_idle.png", Rect(0, 0, 88, 64));
		sprite->setScale(1.5f);
		sprite->setAnchorPoint(Vec2(0, 0));
		sprite->setPosition(Vec2(position));
		sprite->setTag(100);
		gameLayer->addChild(sprite);

		auto helper1_idle = ResouceLoad::getInstance()->helper1_idle->clone();
		auto rep = RepeatForever::create(helper1_idle);
		sprite->runAction(rep);

		attackRate = PlayerInfoSingleTon::getInstance()->bazooka_Rate * 3.0;
		this->sprite = sprite;
	}
	else if(type == 1)
	{
		auto sprite = Sprite::create("helper/helper2_idle.png", Rect(0, 0, 72, 56));
		sprite->setScale(1.5f);
		sprite->setAnchorPoint(Vec2(0, 0));
		sprite->setPosition(Vec2(position));
		sprite->setTag(200);
		gameLayer->addChild(sprite);

		auto helper2_idle = ResouceLoad::getInstance()->helper2_idle->clone();
		auto rep = RepeatForever::create(helper2_idle);
		sprite->runAction(rep);

		attackRate = PlayerInfoSingleTon::getInstance()->sniper_Rate * 4.0;
		this->sprite = sprite;
	}

}

void Helper::autoAttack(float dt)
{

	attackDelayTime = attackDelayTime + dt;
	if (attackDelayTime >= attackRate)
	{

		MyQueryCallback queryCallback; //see "World querying topic"
		b2AABB aabb;
		b2Vec2 center = b2Vec2(sprite->getPosition().x / PTM_RATIO, sprite->getPosition().y / PTM_RATIO);
		// center : 폭탄 중심 위치
		// 폭발 범위
		// 폭발 바운딩박스 위치와 크기 
		aabb.lowerBound = b2Vec2(0, 0);
		//aabb.upperBound = b2Vec2(1200 / PTM_RATIO, 720 / PTM_RATIO);
		aabb.upperBound = center + b2Vec2(1000 / PTM_RATIO, 720 / PTM_RATIO);


		_world->QueryAABB(&queryCallback, aabb);

		Sprite * monster;
		bool isAttack = false;

		for (int i = 0; i < queryCallback.foundBodies.size(); i++) {
			b2Body* body = queryCallback.foundBodies[i];
			auto q_sprite = (Sprite*)body->GetUserData();
			if (q_sprite != nullptr)
			{
				if (q_sprite->getTag() == MONSTER)
				{
					monster = q_sprite;
					isAttack = true;
					log("대상찾음");
					break;
				}
			}
		}
		if (isAttack) {
			// 대포
			if (type == 0)
			{
				Vec2 nPos1 = Vec2(sprite->getContentSize().width, sprite->getContentSize().height / 2);
				Vec2 nPos2 = sprite->convertToWorldSpace(nPos1);
				Vec2 shootVector = monster->getPosition() - nPos2;

				float shootAngle = shootVector.getAngle();
				float cocosAngle = CC_RADIANS_TO_DEGREES(-1 * shootAngle);

				shootVector.normalize();

				SimpleAudioEngine::getInstance()->playEffect("sounds/bazooka.ogg");

				Bullet * bullet = new Bullet(nPos2, 3, cocosAngle);
				bullets->push_back(bullet);
				bullet->body->SetLinearVelocity(b2Vec2(shootVector.x * 25, shootVector.y * 25));

				sprite->stopAllActions();

				auto helper1_attack = ResouceLoad::getInstance()->helper1_attack->clone();
				auto seq = Sequence::create(helper1_attack,
					CallFunc::create(CC_CALLBACK_0(Helper::re_Idle, this)), nullptr);
				sprite->runAction(seq);
			}
			// 스나이퍼
			else if (type == 1)
			{
				Vec2 nPos1 = Vec2(sprite->getContentSize().width - 20, sprite->getContentSize().height / 2 + 5);
				Vec2 nPos2 = sprite->convertToWorldSpace(nPos1);
				Vec2 shootVector = monster->getPosition() - nPos2;

				shootVector.normalize();

				float shootAngle = shootVector.getAngle();
				float cocosAngle = CC_RADIANS_TO_DEGREES(-1 * shootAngle);

				SimpleAudioEngine::getInstance()->playEffect("sounds/sniper.ogg");

				Bullet * bullet = new Bullet(nPos2, 2, cocosAngle);
				bullets->push_back(bullet);
				bullet->body->SetLinearVelocity(b2Vec2(shootVector.x * 70, shootVector.y * 70));
				gameLayer->addChild(bullet);

				sprite->stopAllActions();

				auto helper2_attack = ResouceLoad::getInstance()->helper2_attack->clone();
				auto seq = Sequence::create(helper2_attack,
					CallFunc::create(CC_CALLBACK_0(Helper::re_Idle, this)), nullptr);
				sprite->runAction(seq);
			}
			attackDelayTime = 0;

		}
	
	}
}

// 공격이 끝나면 Idle 애니메이션으로 돌아옴
void Helper::re_Idle()
{
	sprite->stopAllActions();
	if (type == 0) 
	{
		auto helper1_idle = ResouceLoad::getInstance()->helper1_idle->clone();
		auto rep = RepeatForever::create(helper1_idle);
		sprite->runAction(rep);
	}
	else if (type == 1)
	{
		auto helper2_idle = ResouceLoad::getInstance()->helper2_idle->clone();
		auto rep = RepeatForever::create(helper2_idle);
		sprite->runAction(rep);
	}
}

void Helper::onEnter()
{
	Sprite::onEnter();
	this->schedule(schedule_selector(Helper::autoAttack));
}

void Helper::onExit()
{
	this->unschedule(schedule_selector(Helper::autoAttack));

	Sprite::onExit();
}
