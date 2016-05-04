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
		"상점 나가기",
		CC_CALLBACK_1(ShopScene::shopClose, this));
	exit->setColor(Color3B(0, 0, 0));
	//메뉴생성
	auto exitMenu = Menu::create(exit, nullptr);
	//메뉴 위치
	exitMenu->setPosition(Vec2(240, 50));
	// 레이어에 메뉴 객체 추가
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

	//싱글 터치 모드로 터치 리스너 등록
	auto listener = EventListenerTouchOneByOne::create();
	//Swallow touches only available in OneByOne mode.
	//핸들링된 터치 이벤트를 터치 이벤트 array에서 지우겠다는 의미다.
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(ShopScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(ShopScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(ShopScene::onTouchEnded, this);

	// The prioriry of the touch listener is based on the draw order of sprite
	// 터치 리스너의 우선순위를 (노드가) 화면에 그려진 순서대로 한다
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

// 손가락을 화면에서 떼는 순간 호출된다
void ShopScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	
}



*/