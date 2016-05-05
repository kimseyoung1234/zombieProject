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
	//�ڵ鸵�� ��ġ �̺�Ʈ�� ��ġ �̺�Ʈ array���� ����ڴٴ� �ǹ̴�.
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

	Vec2 spritePosition = this->getPosition();	// ��ġ�� ����� ��ǥ
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

// �հ����� ȭ�鿡�� ���� ���� ȣ��ȴ�
void Trap::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	
}
