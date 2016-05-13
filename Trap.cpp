#include "Trap.h"
#include "DataSingleTon.h"
#include "PlayerInfoSingleTon.h"
USING_NS_CC;

Trap::Trap(Vec2 position,int type)
{
	bool bOk = initWithTexture(nullptr, Rect::ZERO);
	if (bOk)
	{
		this->autorelease();
	}
	float radius;
	gameLayer = DataSingleTon::getInstance()->getGameLayer();

	this->type = type;
	if (type == 0) {
		sprite = Sprite::create("item/trap01.png");
		radius = PlayerInfoSingleTon::getInstance()->trap1_blastRadius;
	}
	else if (type == 1)
	{
		sprite = Sprite::create("item/trap02.png");
		radius = PlayerInfoSingleTon::getInstance()->trap2_blastRadius;
	}
	sprite->setPosition(position);

	gameLayer->addChild(sprite);

	auto draw_node = DrawNode::create();
	
	
	Size parentSize;
	parentSize = sprite->getContentSize();
	sprite->addChild(draw_node);
	draw_node->drawCircle(Vec2(parentSize.width / 2.0, parentSize.height / 2.0), radius * PTM_RATIO, CC_DEGREES_TO_RADIANS(0), 50, false, Color4F(1, 1, 1, 1));
}

void Trap::onEnter()
{
	Sprite::onEnter();
}

void Trap::onExit()
{
	Sprite::onExit();
}
