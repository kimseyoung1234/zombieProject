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

	this->type = type;

	sprite = Sprite::create("bomb.png");
	sprite->setScale(2.0f);
	sprite->setPosition(position);
	//sprite->setAnchorPoint(Vec2(0.5, 0.5));
	gameLayer->addChild(sprite);

	auto draw_node = DrawNode::create();
	
	
	Size parentSize;
	parentSize = sprite->getContentSize();
	sprite->addChild(draw_node);
	draw_node->drawCircle(Vec2(parentSize.width / 2.0, parentSize.height / 2.0), 80, CC_DEGREES_TO_RADIANS(0), 50, false, Color4F(1, 1, 1, 1));
}

void Trap::onEnter()
{
	Sprite::onEnter();
}

void Trap::onExit()
{
	Sprite::onExit();
}
