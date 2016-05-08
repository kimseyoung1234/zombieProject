#include "ShopScene.h"
#include "DataSingleTon.h"
#include "PlayerInfoSingleTon.h"
USING_NS_CC;

Scene* ShopScene::createScene()
{
	auto scene = Scene::create();

	auto layer = ShopScene::create();

	scene->addChild(layer);
	//log("새로운씬");
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
	gameLayer = DataSingleTon::getInstance()->getGameLayer();
	traps = DataSingleTon::getInstance()->getTraps();
	
	auto shopLayer = LayerColor::create(Color4B(0, 0, 0, 0), winSize.width, winSize.height);

	this->addChild(shopLayer);

	auto weapon_tableLayer = new TableViewLayer(Vec2(300,480),4,WEAPON);
	auto weapon_view = weapon_tableLayer->getTableView();
	shopLayer->addChild(weapon_view);

	auto trap_tableLayer = new TableViewLayer(Vec2(300, 300), 2,TRAP);
	auto trap_view = trap_tableLayer->getTableView();
	shopLayer->addChild(trap_view);

	auto helper_tableLayer = new TableViewLayer(Vec2(300, 120), 3,HELPER);
	auto helper_view = helper_tableLayer->getTableView();
	shopLayer->addChild(helper_view);


	auto buy = MenuItemFont::create(
		"구입",
		CC_CALLBACK_1(ShopScene::buy, this));
	buy->setColor(Color3B(0, 0, 0));

	auto upgrade = MenuItemFont::create(
		"업그레이드",
		CC_CALLBACK_1(ShopScene::upgrade, this));
	upgrade->setColor(Color3B(0, 0, 0));

	auto exit = MenuItemFont::create(
		"상점 나가기",
		CC_CALLBACK_1(ShopScene::shopClose, this));
	exit->setColor(Color3B(0, 0, 0));

	//메뉴생성
	auto menu = Menu::create(buy,upgrade,exit, nullptr);
	
	//메뉴 위치
	menu->alignItemsHorizontally();
	menu->alignItemsHorizontallyWithPadding(50.0f);
	menu->setPosition(Vec2(winSize.width/2, 50));
	// 레이어에 메뉴 객체 추가
	shopLayer->addChild(menu);

	return true;
}

void ShopScene::shopClose(Ref * pSender)
{
	Director::getInstance()->popScene();
}

void ShopScene::upgrade(Ref * pSender)
{
	log("업그레이드");
}
void ShopScene::buy(Ref * pSender)
{
	//트랩에서 선택된거 있으면
	if (PlayerInfoSingleTon::getInstance()->trapSeleted > -1)
	{
		log("트랩 구입햇다");
		auto trap = new Trap(Vec2(winSize.width / 2, winSize.height / 2), 1);
		gameLayer->addChild(trap);
		traps->push_back(trap);
	}
	// 도우미에서 선택된거 있으면
	else if (PlayerInfoSingleTon::getInstance()->helperSeleted > -1)
	{
		log("도우미 구입햇다");
	}
	// 선택된 것이 아무 것도 없으면
	else
	{
		log("살 거 선택해");
	}
}
