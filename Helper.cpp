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
	this->schedule(schedule_selector(Helper::autoAttack));
}

void Helper::autoAttack(float dt)
{
	log("°ø°ÝÁß");
}

void Helper::onEnter()
{
	Sprite::onEnter();

}

void Helper::onExit()
{
	this->unschedule(schedule_selector(Helper::autoAttack));

	Sprite::onExit();
}
