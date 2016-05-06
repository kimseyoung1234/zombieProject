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

	//auto ta = new TableViewLayer();
	auto tl = TableViewLayer::create();
	this->addChild(tl);


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
	
	// 무기 테이블뷰
	/*TableView* tableView1 = TableView::create(this, Size(500, 60));
	tableView1->setDirection(ScrollView::Direction::HORIZONTAL);
	tableView1->setPosition(Vec2(600, 480));
	
	tableView1->setDelegate(this);
	tableView1->setTag(100);
	this->addChild(tableView1);
	tableView1->reloadData();
	count++;
	*/
	// 트랩 테이블 뷰
/*	TableView* tableView2 = TableView::create(this, Size(500, 60));
	tableView2->setDirection(ScrollView::Direction::HORIZONTAL);
	tableView2->setPosition(Vec2(600, 360));

	tableView2->setDelegate(this);
	tableView2->setTag(200);
	this->addChild(tableView2);
	tableView2->reloadData();
	count++;

	// 도우미 테이블 뷰
	TableView* tableView3 = TableView::create(this, Size(500, 60));
	tableView3->setDirection(ScrollView::Direction::HORIZONTAL);
	tableView3->setPosition(Vec2(600, 240));

	tableView3->setDelegate(this);
	tableView3->setTag(300);
	this->addChild(tableView3);
	tableView3->reloadData();
	*/

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
	log("구입!");
}

void ShopScene::scrollViewDidScroll(ScrollView *view)
{

}
void ShopScene::scrollViewDidZoom(ScrollView *view)
{

}
void ShopScene::tableCellTouched(TableView* table, TableViewCell* cell)
{
	log("Tag : %d\nCell touched at index : %ld", table->getTag(), cell->getIdx());
}
Size ShopScene::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
/*	if (idx == 2)
	{
		return Size(100, 100);
	}*/
	return Size(100, 100);
}

TableViewCell* ShopScene::tableCellAtIndex(TableView *table, ssize_t idx)
{
	auto str = String::createWithFormat("gun%02d.png", idx + 1);
	auto sprite = Sprite::create(str->getCString());

	auto string = String::createWithFormat("%ld", idx);

	TableViewCell *cell = table->dequeueCell();
	if (!cell)
	{
		cell = new CustomTableViewCell();
		cell->autorelease();
		auto sprite = Sprite::create(str->getCString());
		sprite->setAnchorPoint(Vec2::ZERO);
		sprite->setPosition(Vec2(0, 0));
		sprite->setTag(150);
		cell->addChild(sprite);

		auto label = LabelTTF::create(string->getCString(), "Helvetica", 20.0);
		label->setPosition(Vec2::ZERO);
		label->setAnchorPoint(Vec2::ZERO);
		label->setTag(123);
		cell->addChild(label);
	}
	else
	{
		auto sprite1 = (Sprite*)(cell->getChildByTag(150));
		sprite1->setTexture(str->getCString());
		sprite1->setPosition(Vec2(0, 0));

		log("index2 %d", idx);
		auto label = (LabelTTF*)cell->getChildByTag(123);
		label->setString(string->getCString());
	}
	return cell;
}
ssize_t ShopScene::numberOfCellsInTableView(TableView *table)
{
	return 2;
}