#include "Trap.h"
#include "DataSingleTon.h"
USING_NS_CC;

Trap::Trap(Vec2 position,int type)
{
	bool bOk = initWithTexture(nullptr, Rect::ZERO);
	if (bOk)
	{
		this->autorelease();
	}

	gameLayer = DataSingleTon::getInstance()->getGameLayer();

	this->position = position;
	this->type = type;

	sprite = Sprite::create("bomb.png");
	sprite->setScale(2.0f);
	sprite->setPosition(position);
	gameLayer->addChild(sprite);
}

void Trap::onEnter()
{
	Sprite::onEnter();
}

void Trap::onExit()
{
	Sprite::onExit();
}
