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

	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255)))
	{
		return false;
	}
	////////////////////////////////////

	winSize = Director::getInstance()->getWinSize();
	gameLayer = DataSingleTon::getInstance()->getGameLayer();
	traps = DataSingleTon::getInstance()->getTraps();
	helpers = DataSingleTon::getInstance()->getHelpers();
	
	auto shopLayer = LayerColor::create(Color4B(0, 0, 0, 0), winSize.width, winSize.height);

	this->addChild(shopLayer);

	weapon_tableLayer = new TableViewLayer(Vec2(300,550),2,WEAPON);
	auto weapon_view = weapon_tableLayer->getTableView();
	shopLayer->addChild(weapon_view);

	trap_tableLayer = new TableViewLayer(Vec2(300, 340), 3,TRAP);
	auto trap_view = trap_tableLayer->getTableView();
	shopLayer->addChild(trap_view);

	helper_tableLayer = new TableViewLayer(Vec2(300, 130), 3,HELPER);
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

	this->schedule(schedule_selector(ShopScene::tick));

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
		auto trap = new Trap(Vec2(winSize.width/2, winSize.height/2), 1);
		gameLayer->addChild(trap);
		traps->push_back(trap);
	}
	// 도우미에서 선택된거 있으면
	else if (PlayerInfoSingleTon::getInstance()->helperSeleted > -1)
	{
		auto helper = new Helper(Vec2(50, 200), 1);
		gameLayer->addChild(helper);
		helpers->push_back(helper);
	}
	// 선택된 것이 아무 것도 없으면
	else
	{
		log("살 거 선택해");
	}
}

// 트랩과 헬퍼 중에 하나만 체크하도록 확인
void ShopScene::tick(float dt)
{
	int trap_tableCellCount = trap_tableLayer->cellCount;
	int helper_tableCellCount = helper_tableLayer->cellCount;

	for (int i = 0; i < trap_tableCellCount; i++)
	{
		if (PlayerInfoSingleTon::getInstance()->trapSeleted == i)
		{
			auto trap_table = trap_tableLayer->getTableView();
			auto selectedCell = trap_table->cellAtIndex(i);
			if (selectedCell) {
				auto check = selectedCell->getChildByTag(123);
				check->setVisible(true);
			}
		}
		else
		{
			auto trap_table = trap_tableLayer->getTableView();
			auto selectedCell = trap_table->cellAtIndex(i);
			if (selectedCell) {
				auto check = selectedCell->getChildByTag(123);
				check->setVisible(false);
			}
		}
	}

	for (int i = 0; i < helper_tableCellCount; i++)
	{
		if (PlayerInfoSingleTon::getInstance()->helperSeleted == i)
		{
			auto helper_table = helper_tableLayer->getTableView();
			auto selectedCell = helper_table->cellAtIndex(i);
			if (selectedCell) {
				auto check = selectedCell->getChildByTag(123);
				check->setVisible(true);
			}
		}
		else
		{
			auto helper_table = helper_tableLayer->getTableView();
			auto selectedCell = helper_table->cellAtIndex(i);
			if (selectedCell) {
				auto check = selectedCell->getChildByTag(123);
				check->setVisible(false);
			}
		}
	}
}


void ShopScene::onEnter()
{
	Layer::onEnter();

}
void ShopScene::onExit()
{
	this->unschedule(schedule_selector(ShopScene::tick));
	delete weapon_tableLayer;
	delete trap_tableLayer;
	delete helper_tableLayer;
	Layer::onExit();
}