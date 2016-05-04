#include "ShopScene.h"
#include "DataSingleTon.h"
#include "MonsterInfoSingleTon.h"
#include "PlayerInfoSingleTon.h"

USING_NS_CC;

Scene* ShopScene::createScene()
{
	auto scene = Scene::create();

	auto layer = ShopScene::create();

	scene->addChild(layer);

	return scene;
}

bool ShopScene::init()
{

	if (!LayerColor::initWithColor(Color4B(255, 255, 255,255)))
	{
		return false;
	}
	////////////////////////////////////

	winSize = Director::getInstance()->getWinSize();

	auto exit = MenuItemFont::create(
		"���� ������",
		CC_CALLBACK_1(ShopScene::shopClose, this));
	exit->setColor(Color3B(0, 0, 0));
	//�޴�����
	auto exitMenu = Menu::create(exit, nullptr);
	//�޴� ��ġ
	exitMenu->setPosition(Vec2(240, 50));
	// ���̾ �޴� ��ü �߰�
	this->addChild(exitMenu);

	return true;
}


void ShopScene::shopClose(Ref * pSender)
{
	Director::getInstance()->popScene();
}

/*
void ShopScene::onEnter()
{
	Layer::onEnter();

	//�̱� ��ġ ���� ��ġ ������ ���
	auto listener = EventListenerTouchOneByOne::create();
	//Swallow touches only available in OneByOne mode.
	//�ڵ鸵�� ��ġ �̺�Ʈ�� ��ġ �̺�Ʈ array���� ����ڴٴ� �ǹ̴�.
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(ShopScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(ShopScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(ShopScene::onTouchEnded, this);

	// The prioriry of the touch listener is based on the draw order of sprite
	// ��ġ �������� �켱������ (��尡) ȭ�鿡 �׷��� ������� �Ѵ�
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}
void ShopScene::onExit()
{
	_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);

	Layer::onExit();
}



bool ShopScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	auto touchPoint = touch->getLocation();

	return true;
}


void ShopScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	auto touchPoint = touch->getLocation();

}

// �հ����� ȭ�鿡�� ���� ���� ȣ��ȴ�
void ShopScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	
}



*/