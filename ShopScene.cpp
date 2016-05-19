#include "ShopScene.h"
#include "DataSingleTon.h"
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

	// 무기 테이블 뷰 레이어
	auto weapon_tableLayer = LayerColor::create(Color4B(255, 255, 255, 0), 1009, 115);
	weapon_tableLayer->setPosition(Vec2(55, 291));
	tableLayer->addChild(weapon_tableLayer,100);

	auto weapon_background = Sprite::create("layer_weapon.png");
	weapon_background->setPosition(Vec2(weapon_tableLayer->getContentSize().width / 2, weapon_tableLayer->getContentSize().height / 2));
	weapon_tableLayer->addChild(weapon_background);

	auto weaponLabel = Label::create("Weapon", "Arial", 34);
	weaponLabel->setPosition(Vec2(100, 60));
	weaponLabel->setColor(Color3B::RED);
	weapon_tableLayer->addChild(weaponLabel, 101);

	// 트랩 테이블 뷰 레이어
	auto trap_tableLayer = LayerColor::create(Color4B(255, 255, 255, 0), 1009, 115);
	trap_tableLayer->setPosition(Vec2(55, 166));
	tableLayer->addChild(trap_tableLayer, 100);

	auto trap_background = Sprite::create("layer_trap.png");
	trap_background->setPosition(Vec2(trap_tableLayer->getContentSize().width / 2, trap_tableLayer->getContentSize().height / 2));
	trap_tableLayer->addChild(trap_background);

	auto trapLabel = Label::create("Trap", "Arial", 34);
	trapLabel->setPosition(Vec2(100, 60));
	trapLabel->setColor(Color3B::RED);
	trap_tableLayer->addChild(trapLabel, 101);

	// 도우미 테이블뷰 레이어
	auto helper_tableLayer = LayerColor::create(Color4B(255, 255, 255, 0), 1009, 115);
	helper_tableLayer->setPosition(Vec2(55, 42));
	tableLayer->addChild(helper_tableLayer, 100);

	auto helper_background = Sprite::create("layer_helper.png");
	helper_background->setPosition(Vec2(helper_tableLayer->getContentSize().width / 2, helper_tableLayer->getContentSize().height / 2));
	helper_tableLayer->addChild(helper_background);
	
	auto helperLabel = Label::create("Helper", "Arial", 34);
	helperLabel->setPosition(Vec2(100, 60));
	helperLabel->setColor(Color3B::RED);
	helper_tableLayer->addChild(helperLabel, 101);

	// 테이블뷰 객체들
	weapon_table = new TableViewLayer(Vec2(200,0),3,WEAPON);
	auto weapon_view = weapon_table->getTableView();
	weapon_tableLayer->addChild(weapon_view);


	trap_table = new TableViewLayer(Vec2(200, 0), 2,TRAP);
	auto trap_view = trap_table->getTableView();
	trap_tableLayer->addChild(trap_view);

	helper_table = new TableViewLayer(Vec2(200, 0), 2,HELPER);
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

	auto gold = Sprite::create("item/gold.png");
	gold->setPosition(Vec2(150, winSize.height - 100));
	this->addChild(gold);

	// 소지금 라벨
	cocos2d::String *money_In_Hand;
	money_In_Hand = String::createWithFormat("%d", PlayerInfoSingleTon::getInstance()->money_In_Hand);

	money_label = LabelTTF::create(money_In_Hand->getCString(), "Helvetica", 20.0);
	money_label->setPosition(Vec2(220, winSize.height - 100));
	money_label->setScale(1.5f);
	money_label->setColor(Color3B::YELLOW);;
	money_label->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(money_label, 1);

	this->schedule(schedule_selector(ShopScene::tick));

	return true;
}

void ShopScene::shopClose(Ref * pSender)
{
	Director::getInstance()->popScene();
}

void ShopScene::upgrade(Ref * pSender)
{
	cocos2d::String *price;
	int selectedWeapon = PlayerInfoSingleTon::getInstance()->weaponSeleted; //선택된 무기
	int money_In_Hand = PlayerInfoSingleTon::getInstance()->money_In_Hand; // 플레이어 소지금
	bool isBuy = false; // 구입 했는지 아닌지

	// 선택된 무기에 따라 업그레이드, 가격 올리기
	if (selectedWeapon == 0 && money_In_Hand >= PlayerInfoSingleTon::getInstance()->machine_price)
	{
		// 소지금에서 구입한 물품에 가격 빼기
		PlayerInfoSingleTon::getInstance()->money_In_Hand = money_In_Hand - PlayerInfoSingleTon::getInstance()->machine_price;
		// 무기 업그레이드 비용 증가
		PlayerInfoSingleTon::getInstance()->machine_price = PlayerInfoSingleTon::getInstance()->machine_price + 500;
		price = String::createWithFormat("%d", PlayerInfoSingleTon::getInstance()->machine_price);

		isBuy = true;
	}
	else if (selectedWeapon == 1 && money_In_Hand >= PlayerInfoSingleTon::getInstance()->ak_price)
	{
		PlayerInfoSingleTon::getInstance()->money_In_Hand = money_In_Hand - PlayerInfoSingleTon::getInstance()->ak_price;
		PlayerInfoSingleTon::getInstance()->ak_price = PlayerInfoSingleTon::getInstance()->ak_price + 500;
		price = String::createWithFormat("%d", PlayerInfoSingleTon::getInstance()->ak_price);

		isBuy = true;
	}
	else if (selectedWeapon == 2 && money_In_Hand >= PlayerInfoSingleTon::getInstance()->sniper_price)
	{
		PlayerInfoSingleTon::getInstance()->money_In_Hand = money_In_Hand - PlayerInfoSingleTon::getInstance()->sniper_price;
		PlayerInfoSingleTon::getInstance()->sniper_price = PlayerInfoSingleTon::getInstance()->sniper_price + 500;
		price = String::createWithFormat("%d", PlayerInfoSingleTon::getInstance()->sniper_price);

		isBuy = true;
	}

	// 새로운 가격으로 업데이트
	if (isBuy) {
		auto _weapon_table = weapon_table->getTableView();
		auto selectedCell = _weapon_table->cellAtIndex(selectedWeapon);

		if (selectedCell) {
			auto label = (LabelTTF*)selectedCell->getChildByTag(101);
			label->setString(price->getCString());
		}
	}
	else
		log("돈 부족해서 못삼");
}
void ShopScene::buy(Ref * pSender)
{
	int money_In_Hand = PlayerInfoSingleTon::getInstance()->money_In_Hand; // 플레이어 소지금
	bool isBuy = false;
	//트랩에서 선택된거 있으면
	if (PlayerInfoSingleTon::getInstance()->trapSeleted > -1)
	{
		int selectedTrap = PlayerInfoSingleTon::getInstance()->trapSeleted;
		if (selectedTrap == 0 && money_In_Hand >= PlayerInfoSingleTon::getInstance()->trap1_price)
		{
			PlayerInfoSingleTon::getInstance()->money_In_Hand = money_In_Hand - PlayerInfoSingleTon::getInstance()->trap1_price;
			PlayerInfoSingleTon::getInstance()->have_trap1++;
			isBuy = true;
		}
		else if (selectedTrap == 1 && money_In_Hand >= PlayerInfoSingleTon::getInstance()->trap2_price)
		{
			PlayerInfoSingleTon::getInstance()->money_In_Hand = money_In_Hand - PlayerInfoSingleTon::getInstance()->trap2_price;
			PlayerInfoSingleTon::getInstance()->have_trap2++;
			isBuy = true;
		}

		if (isBuy) {
			/*auto trap = new Trap(Vec2(winSize.width / 2, winSize.height / 2), selectedTrap);
			gameLayer->addChild(trap);
			traps->push_back(trap);*/
		}
		else
		{
			log("살돈 없다");
		}
	}
	// 도우미에서 선택된거 있으면
	else if (PlayerInfoSingleTon::getInstance()->helperSeleted > -1)
	{
		
		int selectedHelper = PlayerInfoSingleTon::getInstance()->helperSeleted;

		if (selectedHelper == 0 && money_In_Hand >= PlayerInfoSingleTon::getInstance()->helper1_price && PlayerInfoSingleTon::getInstance()->have_helper < 2)
		{
			PlayerInfoSingleTon::getInstance()->money_In_Hand = money_In_Hand - PlayerInfoSingleTon::getInstance()->helper1_price;
			isBuy = true;
		}
		else if (selectedHelper == 1 && money_In_Hand >= PlayerInfoSingleTon::getInstance()->helper2_price && PlayerInfoSingleTon::getInstance()->have_helper < 2)
		{
			PlayerInfoSingleTon::getInstance()->money_In_Hand = money_In_Hand - PlayerInfoSingleTon::getInstance()->helper2_price;
			isBuy = true;
		}

		if (isBuy) {
			if (PlayerInfoSingleTon::getInstance()->have_helper == 0) {
				auto helper = new Helper(Vec2(50, 200), selectedHelper);
				gameLayer->addChild(helper);
				helpers->push_back(helper);

				PlayerInfoSingleTon::getInstance()->have_helper++;
			}
			else if (PlayerInfoSingleTon::getInstance()->have_helper == 1)
			{
				auto helper = new Helper(Vec2(50, 300), selectedHelper);
				gameLayer->addChild(helper);
				helpers->push_back(helper);

				PlayerInfoSingleTon::getInstance()->have_helper++;
			}
		}
		else
		{
			log("못삼");
		}
	}
	// 선택된 것이 아무 것도 없으면
	else
	{
		log("살 거 선택해");
	}
}


void ShopScene::tick(float dt)
{
	int trap_tableCellCount = trap_table->cellCount;
	int helper_tableCellCount = helper_table->cellCount;


	//

	//소지금 라벨 업데이트
	cocos2d::String *money_In_Hand;
	money_In_Hand = String::createWithFormat("%d", PlayerInfoSingleTon::getInstance()->money_In_Hand);
	money_label->setString(money_In_Hand->getCString());

	// 트랩과 헬퍼 중에 하나만 체크하도록 확인
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