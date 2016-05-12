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

	// 배경
	auto background = Sprite::create("shopbackground.png");
	background->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	this->addChild(background);

	// 테이블레이어
	auto tableLayer = LayerColor::create(Color4B(0, 0, 0, 0), 1120 , 450);
	tableLayer->setPosition(Vec2(80, 120));
	this->addChild(tableLayer);
	// 테이블레이어 배경
	auto table_background = Sprite::create("tableBackground.png");
	table_background->setPosition(tableLayer->getContentSize().width / 2, tableLayer->getContentSize().height / 2);
	tableLayer->addChild(table_background);

	// 테이블뷰 각각의 레이어
	auto weapon_tableLayer = LayerColor::create(Color4B(255, 255, 255, 0), 1009, 115);
	weapon_tableLayer->setPosition(Vec2(55, 291));
	tableLayer->addChild(weapon_tableLayer,100);

	// 무기 테이블뷰 배경
	auto weapon_background = Sprite::create("weapon_layer.png");
	weapon_background->setPosition(Vec2(weapon_tableLayer->getContentSize().width / 2, weapon_tableLayer->getContentSize().height / 2));
	weapon_tableLayer->addChild(weapon_background);


	auto trap_tableLayer = LayerColor::create(Color4B(255, 255, 255, 255), 1009, 115);
	trap_tableLayer->setPosition(Vec2(55, 166));
	tableLayer->addChild(trap_tableLayer, 100);

	auto helper_tableLayer = LayerColor::create(Color4B(255, 255, 255, 255), 1009, 115);
	helper_tableLayer->setPosition(Vec2(55, 42));
	tableLayer->addChild(helper_tableLayer, 100);


	// 테이블뷰 객체들
	weapon_table = new TableViewLayer(Vec2(200,0),2,WEAPON);
	auto weapon_view = weapon_table->getTableView();
	weapon_tableLayer->addChild(weapon_view);


	trap_table = new TableViewLayer(Vec2(200, 0), 3,TRAP);
	auto trap_view = trap_table->getTableView();
	trap_tableLayer->addChild(trap_view);

	helper_table = new TableViewLayer(Vec2(200, 0), 3,HELPER);
	auto helper_view = helper_table->getTableView();
	helper_tableLayer->addChild(helper_view);


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
	this->addChild(menu);

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
	int trap_tableCellCount = trap_table->cellCount;
	int helper_tableCellCount = helper_table->cellCount;

	for (int i = 0; i < trap_tableCellCount; i++)
	{
		if (PlayerInfoSingleTon::getInstance()->trapSeleted == i)
		{
			auto _trap_table = trap_table->getTableView();
			auto selectedCell = _trap_table->cellAtIndex(i);
			if (selectedCell) {
				auto check = selectedCell->getChildByTag(123);
				check->setVisible(true);
			}
		}
		else
		{
			auto _trap_table = trap_table->getTableView();
			auto selectedCell = _trap_table->cellAtIndex(i);
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
			auto _helper_table = helper_table->getTableView();
			auto selectedCell = _helper_table->cellAtIndex(i);
			if (selectedCell) {
				auto check = selectedCell->getChildByTag(123);
				check->setVisible(true);
			}
		}
		else
		{
			auto _helper_table = helper_table->getTableView();
			auto selectedCell = _helper_table->cellAtIndex(i);
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
	delete weapon_table;
	delete trap_table;
	delete helper_table;
	Layer::onExit();
}