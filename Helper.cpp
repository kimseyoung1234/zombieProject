#include "Helper.h"
#include "DataSingleTon.h"
#include "ResouceLoad.h"
#include "PlayerInfoSingleTon.h"
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

	this->type = type;

	if (type == 0) {
		auto sprite = Sprite::create("helper1_idle.png", Rect(0, 0, 56, 64));
		sprite->setScale(1.5f);
		sprite->setAnchorPoint(Vec2(0, 0));
		sprite->setPosition(Vec2(position));
		gameLayer->addChild(sprite);

		auto helper1_idle = ResouceLoad::getInstance()->helper1_idle->clone();
		auto rep = RepeatForever::create(helper1_idle);
		sprite->runAction(rep);

		attackRate = PlayerInfoSingleTon::getInstance()->bazooka_Rate;
		this->sprite = sprite;
	}
	else if(type == 1)
	{
	
		auto sprite = Sprite::create("helper1_idle.png", Rect(0, 0, 56, 64));
		sprite->setScale(1.5f);
		sprite->setAnchorPoint(Vec2(0, 0));
		sprite->setPosition(Vec2(position));
		gameLayer->addChild(sprite);

		auto helper1_idle = ResouceLoad::getInstance()->helper1_idle->clone();
		auto rep = RepeatForever::create(helper1_idle);
		sprite->runAction(rep);

		attackRate = PlayerInfoSingleTon::getInstance()->sniper_Rate;
		this->sprite = sprite;
	}

}

void Helper::autoAttack(float dt)
{

	attackDelayTime = attackDelayTime + dt;
	if (attackDelayTime >= attackRate)
	{
		if (monsters->size() > 0) {
			int monsterSize = monsters->size() - 1;
			int selected_monster = random(0, monsterSize);

			Vec2 nPos1 = Vec2(sprite->getContentSize().width, sprite->getContentSize().height / 2);
			Vec2 nPos2 = sprite->convertToWorldSpace(nPos1);
			Vec2 shootVector = monsters->at(selected_monster)->sprite->getPosition() - nPos2;
			
			float shootAngle = shootVector.getAngle();
			float cocosAngle = CC_RADIANS_TO_DEGREES(-1 * shootAngle);

			shootVector.normalize();
			// 대포
			if (type == 0) 
			{
				Bullet * bullet = new Bullet(nPos2, 3, cocosAngle);
				bullets->push_back(bullet);
				bullet->body->SetLinearVelocity(b2Vec2(shootVector.x * 30, shootVector.y * 30));
			}
			// 스나이퍼
			else if (type == 1)
			{
				
				Bullet * bullet = new Bullet(nPos2, 2, cocosAngle);
				bullets->push_back(bullet);
				bullet->body->SetLinearVelocity(b2Vec2(shootVector.x * 30, shootVector.y * 30));
				gameLayer->addChild(bullet);
			}
			attackDelayTime = 0;

			// 공격 애니메이션
			sprite->stopAllActions();
	
			auto helper1_attack = ResouceLoad::getInstance()->helper1_attack->clone();
			auto seq = Sequence::create(helper1_attack,
				CallFunc::create(CC_CALLBACK_0(Helper::re_Idle,this)),nullptr);
			sprite->runAction(seq);

		}
	}
}

// 공격이 끝나면 Idle 애니메이션으로 돌아옴
void Helper::re_Idle()
{
	sprite->stopAllActions();
	
	auto helper1_idle = ResouceLoad::getInstance()->helper1_idle->clone();
	auto rep = RepeatForever::create(helper1_idle);
	sprite->runAction(rep);
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
