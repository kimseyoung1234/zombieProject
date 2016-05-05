#include "Trap.h"

USING_NS_CC;

Trap::Trap(Vec2 position)
{
	this->position = position;
	bool bOk = initWithTexture(nullptr, Rect::ZERO);
	if (bOk)
	{
		this->autorelease();
	}
}

void Trap::onEnter()
{
	Sprite::onEnter();

	auto listener = EventListenerTouchOneByOne::create();
	//Swallow touches only available in OneByOne mode.
	//핸들링된 터치 이벤트를 터치 이벤트 array에서 지우겠다는 의미다.
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(Trap::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Trap::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Trap::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void Trap::onExit()
{
	_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);

	Sprite::onExit();
}

bool Trap::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	auto touchPoint = touch->getLocation();

	Vec2 spritePosition = this->getPosition();	// 터치한 블록의 좌표
	Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());

	Size s = this->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);
	if (rect.containsPoint(locationInNode))
	{
		this->setColor(Color3B::RED);
		return true;
	}
	return false;
}


void Trap::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	auto touchPoint = touch->getLocation();

}

// 손가락을 화면에서 떼는 순간 호출된다
void Trap::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	
}
