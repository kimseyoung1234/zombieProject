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

	auto weaponLabel = LabelBMFont::create("Weapon", "fonts/futura-48.fnt");
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

	auto trapLabel = LabelBMFont::create("Trap", "fonts/futura-48.fnt");
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
	
	auto helperLabel = LabelBMFont::create("Helper", "fonts/futura-48.fnt");
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

	auto buy = MenuItemImage::create(
		"ui/buy.png",
		"ui/buy_push.png",
		CC_CALLBACK_1(ShopScene::buy, this));
	buy->setScale(0.7f);
	auto upgrade = MenuItemImage::create(
		"ui/upgrade.png",
		"ui/upgrade_push.png",
		CC_CALLBACK_1(ShopScene::upgrade, this));
	upgrade->setScale(0.7f);

	auto exit = MenuItemImage::create(
		"ui/back.png",
		"ui/back_push.png",
		CC_CALLBACK_1(ShopScene::shopClose, this));
	exit->setScale(0.7f);

	//메뉴생성
	auto menu = Menu::create(buy,upgrade,exit, nullptr);
	
	//메뉴 위치
	menu->alignItemsHorizontally();
	menu->alignItemsHorizontallyWithPadding(30.0f);
	menu->setPosition(Vec2(winSize.width/2, 50));
	// 레이어에 메뉴 객체 추가
	this->addChild(menu);

	auto gold = Sprite::create("item/gold.png");
	gold->setPosition(Vec2(150, winSize.height - 100));
	this->addChild(gold);

	// 소지금 라벨
	cocos2d::String *money_In_Hand;
	money_In_Hand = String::createWithFormat("%d", PlayerInfoSingleTon::getInstance()->money_In_Hand);

	money_label = LabelBMFont::create(money_In_Hand->getCString(), "fonts/futura-48.fnt");
	money_label->setPosition(Vec2(240, winSize.height - 100));
	money_label->setScale(0.8f);
	//money_label->setColor(Color3B::YELLOW);;
	money_label->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(money_label, 1);

	////////// 무기 업그레이드 수치
	auto _weapon_table = weapon_table->getTableView();
	auto cell_1 = _weapon_table->cellAtIndex(0);
	auto cell_2 = _weapon_table->cellAtIndex(1);
	auto cell_3 = _weapon_table->cellAtIndex(2);

	auto gage1 = Sprite::create("ui/ui_gage.png");
	gage1->setPosition(Vec2(160, 60));
	gage1->setTag(500);
	cell_1->addChild(gage1);

	auto gage2 = Sprite::create("ui/ui_gage.png");
	gage2->setPosition(Vec2(160, 60));
	gage2->setTag(500);
	cell_2->addChild(gage2);

	auto gage3 = Sprite::create("ui/ui_gage.png");
	gage3->setPosition(Vec2(160, 60));
	gage3->setTag(500);
	cell_3->addChild(gage3);

	// 아이템 보유수

	item = Sprite::create("ui/ui_item.png");
	item->setPosition(Vec2(75, 60));
	this->addChild(item, 1000);

	Size parentSize;
	parentSize = item->getContentSize();

	auto trap1 = Sprite::create("item/trap01.png");
	trap1->setPosition(Vec2(parentSize.width / 2.0, parentSize.height / 2.0));
	trap1->setScale(2.0f);
	trap1->setOpacity(180.0f);
	item->addChild(trap1);

	item_Label = LabelBMFont::create("0", "fonts/futura-48.fnt");

	item_Label->setPosition(parentSize.width / 2.0, 15);
	item_Label->setColor(Color3B::RED);
	item_Label->setScale(0.8f);
	item->addChild(item_Label);

	// 보유 아이템
	item2 = Sprite::create("ui/ui_item.png");
	item2->setPosition(Vec2(225, 60));
	//item2->setOpacity(180.0f);
	this->addChild(item2, 1000);

	auto trap2 = Sprite::create("item/trap02.png");
	trap2->setPosition(Vec2(parentSize.width / 2.0, parentSize.height / 2.0));
	trap2->setScale(2.0f);
	trap2->setOpacity(180.0f);
	item2->addChild(trap2);

	item2_Label = LabelBMFont::create("0", "fonts/futura-48.fnt");
	item2_Label->setPosition(parentSize.width / 2.0, 15);
	item2_Label->setColor(Color3B::RED);
	item2_Label->setScale(0.8f);
	item2->addChild(item2_Label);

	// 보유 도우미
	have_helper1= MenuItemImage::create(
		"ui/ui_item.png",
		"ui/ui_item.png",
		CC_CALLBACK_1(ShopScene::sellHelper1, this));
	have_helper1->setPosition(Vec2(winSize.width - 225, 60));

	have_helper2 = MenuItemImage::create(
		"ui/ui_item.png",
		"ui/ui_item.png",
		CC_CALLBACK_1(ShopScene::sellHelper2, this));
	have_helper2->setPosition(Vec2(winSize.width - 75, 60));
		
	auto helper_Menu = Menu::create(have_helper1,have_helper2, nullptr);
	helper_Menu->setPosition(Vec2::ZERO);
	this->addChild(helper_Menu);
	
	helperUpdate(0);
	this->schedule(schedule_selector(ShopScene::tick));
	return true;
}

void ShopScene::sellHelper1(Ref * pSender)
{
	log("1번쨰칸");
}
void ShopScene::sellHelper2(Ref * pSender)
{
	log("2번쨰칸");
}
void ShopScene::shopClose(Ref * pSender)
{
	PlayerInfoSingleTon::getInstance()->trapSeleted = -1;
	PlayerInfoSingleTon::getInstance()->helperSeleted = -1;

	Director::getInstance()->popScene();
}

void ShopScene::upgrade(Ref * pSender)
{
	cocos2d::String *price;
	int selectedWeapon = PlayerInfoSingleTon::getInstance()->weaponSeleted; //선택된 무기
	int money_In_Hand = PlayerInfoSingleTon::getInstance()->money_In_Hand; // 플레이어 소지금
	bool isBuy = false; // 구입 했는지 아닌지
	int item_price = 0;


	// 선택된 무기에 따라 업그레이드, 가격 올리기
	if (selectedWeapon == 0 && money_In_Hand >= PlayerInfoSingleTon::getInstance()->machine_price && PlayerInfoSingleTon::getInstance()->machine_level < 10)
	{
		item_price = PlayerInfoSingleTon::getInstance()->machine_price;
		// 소지금에서 구입한 물품에 가격 빼기
		PlayerInfoSingleTon::getInstance()->money_In_Hand = money_In_Hand - item_price;
		// 무기 업그레이드 비용 증가
		PlayerInfoSingleTon::getInstance()->machine_price = item_price + 500;
		price = String::createWithFormat("%d", PlayerInfoSingleTon::getInstance()->machine_price);
		PlayerInfoSingleTon::getInstance()->machine_level++;

		isBuy = true;
	}
	else if (selectedWeapon == 1 && money_In_Hand >= PlayerInfoSingleTon::getInstance()->ak_price && PlayerInfoSingleTon::getInstance()->ak_level < 10)
	{
		item_price = PlayerInfoSingleTon::getInstance()->ak_price;
		PlayerInfoSingleTon::getInstance()->money_In_Hand = money_In_Hand - item_price;
		PlayerInfoSingleTon::getInstance()->ak_price = item_price + 500;
		price = String::createWithFormat("%d", PlayerInfoSingleTon::getInstance()->ak_price);
		PlayerInfoSingleTon::getInstance()->ak_level++;

		isBuy = true;
	}
	else if (selectedWeapon == 2 && money_In_Hand >= PlayerInfoSingleTon::getInstance()->sniper_price && PlayerInfoSingleTon::getInstance()->sniper_level < 10)
	{
		item_price = PlayerInfoSingleTon::getInstance()->sniper_price;
		PlayerInfoSingleTon::getInstance()->money_In_Hand = money_In_Hand - item_price;
		PlayerInfoSingleTon::getInstance()->sniper_price = item_price + 500;
		price = String::createWithFormat("%d", PlayerInfoSingleTon::getInstance()->sniper_price);
		PlayerInfoSingleTon::getInstance()->sniper_level++;

		isBuy = true;
	}

	// 새로운 가격으로 업데이트
	if (isBuy) {
		auto _weapon_table = weapon_table->getTableView();
		auto selectedCell = _weapon_table->cellAtIndex(selectedWeapon);

		buyAni(item_price);

		if (selectedCell) {
			auto label = (LabelBMFont*)selectedCell->getChildByTag(101);
			label->setString(price->getCString());
		}
	}
	else
		log("돈 부족해서 못삼");
}

void ShopScene::buyAni(int price)
{
	auto buy_gold = String::createWithFormat(" - %d", price);

	auto buy_gold_label = LabelBMFont::create(buy_gold->getCString(), "fonts/futura-48.fnt");
	buy_gold_label->setAnchorPoint(Vec2(0, 0.5));
	buy_gold_label->setPosition(Vec2(220, 650));
	buy_gold_label->setScale(0.7f);
	this->addChild(buy_gold_label, 1000);

	auto move = MoveBy::create(1.5, Vec2(0, 30));
	auto fade_seq = Sequence::create(DelayTime::create(0.7),
		FadeOut::create(0.8), nullptr);
	auto spawn = Spawn::create(move, fade_seq, nullptr);
	auto gold_seq = Sequence::create(spawn,
		CallFunc::create(CC_CALLBACK_0(ShopScene::remove_label, this, buy_gold_label)), nullptr);
	buy_gold_label->runAction(gold_seq);

}
void ShopScene::remove_label(Node* sender)
{
	auto label = (LabelBMFont*)sender;
	this->removeChild(label);
}
void ShopScene::remove_sprite(Node* sender)
{
	auto sprite = (Sprite*)sender;
	this->removeChild(sprite);
}
void ShopScene::buy(Ref * pSender)
{
	int money_In_Hand = PlayerInfoSingleTon::getInstance()->money_In_Hand; // 플레이어 소지금
	bool isBuy = false;
	int item_price = 0;
	//트랩에서 선택된거 있으면
	if (PlayerInfoSingleTon::getInstance()->trapSeleted > -1)
	{
		int selectedTrap = PlayerInfoSingleTon::getInstance()->trapSeleted;
		if (selectedTrap == 0 && money_In_Hand >= PlayerInfoSingleTon::getInstance()->trap1_price)
		{
			item_price = PlayerInfoSingleTon::getInstance()->trap1_price;
			PlayerInfoSingleTon::getInstance()->money_In_Hand = money_In_Hand - item_price;
			PlayerInfoSingleTon::getInstance()->have_trap1++;
			isBuy = true;

			// bezier 실험
			auto trap01 = Sprite::create("item/trap01.png");
			trap01->setPosition(Vec2(400, 400));
			trap01->setScale(1.5f);
			this->addChild(trap01, 2000);

			ccBezierConfig bezier;
			bezier.controlPoint_1 = Vec2(100, winSize.height - 200);
			bezier.controlPoint_2 = Vec2(75, winSize.height / 2 / 2);
			bezier.endPosition = Vec2(item->getPosition());

			auto bezierForward = BezierTo::create(0.7, bezier);
			auto seq = Sequence::create(bezierForward,
				CallFunc::create(CC_CALLBACK_0(ShopScene::remove_sprite, this, trap01)), nullptr);
			trap01->runAction(seq);
		}
		else if (selectedTrap == 1 && money_In_Hand >= PlayerInfoSingleTon::getInstance()->trap2_price)
		{
			item_price = PlayerInfoSingleTon::getInstance()->trap2_price;
			PlayerInfoSingleTon::getInstance()->money_In_Hand = money_In_Hand - item_price;
			PlayerInfoSingleTon::getInstance()->have_trap2++;
			isBuy = true;

			// bezier 실험
			auto trap02 = Sprite::create("item/trap02.png");
			trap02->setPosition(Vec2(600, 400));
			trap02->setScale(1.5f);
			this->addChild(trap02, 2000);

			ccBezierConfig bezier;
			bezier.controlPoint_1 = Vec2(325, winSize.height - 200);
			bezier.controlPoint_2 = Vec2(225, winSize.height / 2 / 2);
			bezier.endPosition = Vec2(item2->getPosition());

			auto bezierForward = BezierTo::create(0.7, bezier);

			auto seq = Sequence::create(bezierForward,
				CallFunc::create(CC_CALLBACK_0(ShopScene::remove_sprite, this, trap02)), nullptr);
			trap02->runAction(seq);
		}

		if (isBuy) 
		{
			buyAni(item_price);
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
		int item_price = 0;

		if (selectedHelper == 0 && money_In_Hand >= PlayerInfoSingleTon::getInstance()->helper1_price && PlayerInfoSingleTon::getInstance()->have_helper < 2)
		{
			item_price = PlayerInfoSingleTon::getInstance()->helper1_price;
			PlayerInfoSingleTon::getInstance()->money_In_Hand = money_In_Hand - item_price;
			isBuy = true;
		}
		else if (selectedHelper == 1 && money_In_Hand >= PlayerInfoSingleTon::getInstance()->helper2_price && PlayerInfoSingleTon::getInstance()->have_helper < 2)
		{
			item_price = PlayerInfoSingleTon::getInstance()->helper2_price;
			PlayerInfoSingleTon::getInstance()->money_In_Hand = money_In_Hand - item_price;
			isBuy = true;
		}

		if (isBuy) {
			if (PlayerInfoSingleTon::getInstance()->have_helper == 0) {
				auto helper = new Helper(Vec2(30, 160), selectedHelper);
				gameLayer->addChild(helper);
				helpers->push_back(helper);

				PlayerInfoSingleTon::getInstance()->have_helper++;
			}
			else if (PlayerInfoSingleTon::getInstance()->have_helper == 1)
			{
				auto helper = new Helper(Vec2(30, 400), selectedHelper);
				gameLayer->addChild(helper);
				helpers->push_back(helper);

				PlayerInfoSingleTon::getInstance()->have_helper++;
				
			}
			helperUpdate(1);
			buyAni(item_price);
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

void ShopScene::helperUpdate(int type)
{
	if (type == 0) {
		for (int i = 0; i < helpers->size(); i++)
		{
			auto helper = (Helper *)helpers->at(i);
			log("몇번째 %d", i);
			if (!isHelper1)
			{
				if (helper->sprite->getTag() == 100)
				{
					auto helper = Sprite::create("item/helper01.png");
					helper->setPosition(Vec2(have_helper1->getContentSize().width / 2, have_helper1->getContentSize().height / 2));
					have_helper1->addChild(helper);
					log("1");
				}
				else if (helper->sprite->getTag() == 200)
				{
					auto helper = Sprite::create("item/helper02.png");
					helper->setPosition(Vec2(have_helper1->getContentSize().width / 2, have_helper1->getContentSize().height / 2));
					have_helper1->addChild(helper);
					log("2");
				}
				isHelper1 = true;
			}
			else if (!isHelper2)
			{
				if (helper->sprite->getTag() == 100)
				{
					auto helper = Sprite::create("item/helper01.png");
					helper->setPosition(Vec2(have_helper1->getContentSize().width / 2, have_helper1->getContentSize().height / 2));
					have_helper2->addChild(helper);
					log("3");
				}
				else if (helper->sprite->getTag() == 200)
				{
					auto helper = Sprite::create("item/helper02.png");
					helper->setPosition(Vec2(have_helper1->getContentSize().width / 2, have_helper1->getContentSize().height / 2));
					have_helper2->addChild(helper);
					log("4");
				}
				isHelper2 = true;
			}
		}
	}
	else
	{
		if (!isHelper1)
		{
			if (PlayerInfoSingleTon::getInstance()->helperSeleted == 0)
			{
				auto helper = Sprite::create("item/helper01.png");
				helper->setPosition(Vec2(have_helper1->getContentSize().width / 2, have_helper1->getContentSize().height / 2));
				have_helper1->addChild(helper);
				log("1");
			}
			else if (PlayerInfoSingleTon::getInstance()->helperSeleted == 1)
			{
				auto helper = Sprite::create("item/helper02.png");
				helper->setPosition(Vec2(have_helper1->getContentSize().width / 2, have_helper1->getContentSize().height / 2));
				have_helper1->addChild(helper);
				log("2");
			}
			isHelper1 = true;
		}
		else if (!isHelper2)
		{
			if (PlayerInfoSingleTon::getInstance()->helperSeleted == 0)
			{
				auto helper = Sprite::create("item/helper01.png");
				helper->setPosition(Vec2(have_helper1->getContentSize().width / 2, have_helper1->getContentSize().height / 2));
				have_helper2->addChild(helper);
				log("3");
			}
			else if (PlayerInfoSingleTon::getInstance()->helperSeleted == 1)
			{
				auto helper = Sprite::create("item/helper02.png");
				helper->setPosition(Vec2(have_helper1->getContentSize().width / 2, have_helper1->getContentSize().height / 2));
				have_helper2->addChild(helper);
				log("4");
			}
			isHelper2 = true;
		}
	}
}

void ShopScene::tick(float dt)
{

	// 보유 도우미 갱신
	if (isUpdate = false) {
		isUpdate = true;
	}


	// 아이템 수 갱신
	item_Label->setString((String::createWithFormat("%d", PlayerInfoSingleTon::getInstance()->have_trap1)->getCString()));
	item2_Label->setString((String::createWithFormat("%d", PlayerInfoSingleTon::getInstance()->have_trap2)->getCString()));

	int trap_tableCellCount = trap_table->cellCount;
	int helper_tableCellCount = helper_table->cellCount;

	// 실험
	auto _weapon_table = weapon_table->getTableView();
	auto cell_1 = _weapon_table->cellAtIndex(0);
	if (cell_1 != nullptr) {
		auto gage1 = (Sprite*)cell_1->getChildByTag(500);
		gage1->removeAllChildren();
		for (int i = 0; i < PlayerInfoSingleTon::getInstance()->machine_level; i++)
		{
			auto upgrade = Sprite::create("ui/ui_upgrade.png");
			upgrade->setPosition(Vec2(13, 18 + (9 * i)));
			gage1->addChild(upgrade);
		}
	}

	auto cell_2 = _weapon_table->cellAtIndex(1);
	if (cell_2 != nullptr) {
		auto gage2 = (Sprite*)cell_2->getChildByTag(500);
		gage2->removeAllChildren();
		for (int i = 0; i < PlayerInfoSingleTon::getInstance()->ak_level; i++)
		{
			auto upgrade = Sprite::create("ui/ui_upgrade.png");
			upgrade->setPosition(Vec2(13, 18 + (9 * i)));
			gage2->addChild(upgrade);
		}
	}

	auto cell_3 = _weapon_table->cellAtIndex(2);
	if (cell_3 != nullptr) {
		auto gage3 = (Sprite*)cell_3->getChildByTag(500);
		gage3->removeAllChildren();
		for (int i = 0; i < PlayerInfoSingleTon::getInstance()->sniper_level; i++)
		{
			auto upgrade = Sprite::create("ui/ui_upgrade.png");
			upgrade->setPosition(Vec2(13, 18 + (9 * i)));
			gage3->addChild(upgrade);
		}
	}


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