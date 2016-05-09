#include "Helper.h"
#include "DataSingleTon.h"
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

	sprite = Sprite::create("turret.png");
	sprite->setPosition(position);
	gameLayer->addChild(sprite);

	this->sprite = sprite;

}

void Helper::autoAttack(float dt)
{
	attackDelayTime = attackDelayTime + dt;
	if (attackDelayTime >= 0.3)
	{
		if (monsters->size() > 0) {
			int monsterSize = monsters->size() - 1;
			int selected_monster = random(0, monsterSize);

			Vec2 nPos1 = Vec2(sprite->getContentSize().width, sprite->getContentSize().height / 2);
			Vec2 nPos2 = sprite->convertToWorldSpace(nPos1);
			Vec2 shootVector = monsters->at(selected_monster)->sprite->getPosition() - nPos2;
			shootVector.normalize();

			Bullet * bullet = new Bullet(nPos2, 1);
			bullets->push_back(bullet);
			bullet->body->SetLinearVelocity(b2Vec2(shootVector.x * 30, shootVector.y * 30));
			attackDelayTime = 0;
		}
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
