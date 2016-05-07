#include "ShopScene.h"
#include "DataSingleTon.h"

USING_NS_CC;

Scene* ShopScene::createScene()
{
	auto scene = Scene::create();

	auto layer = ShopScene::create();

	scene->addChild(layer);
	//log("���ο��");
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
		"����",
		CC_CALLBACK_1(ShopScene::buy, this));
	buy->setColor(Color3B(0, 0, 0));

	auto upgrade = MenuItemFont::create(
		"���׷��̵�",
		CC_CALLBACK_1(ShopScene::upgrade, this));
	upgrade->setColor(Color3B(0, 0, 0));

	auto exit = MenuItemFont::create(
		"���� ������",
		CC_CALLBACK_1(ShopScene::shopClose, this));
	exit->setColor(Color3B(0, 0, 0));

	//�޴�����
	auto menu = Menu::create(buy,upgrade,exit, nullptr);
	
	//�޴� ��ġ
	menu->alignItemsHorizontally();
	menu->alignItemsHorizontallyWithPadding(50.0f);
	menu->setPosition(Vec2(winSize.width/2, 50));
	// ���̾ �޴� ��ü �߰�
	shopLayer->addChild(menu);
	
	// ���� ���̺��
	/*TableView* tableView1 = TableView::create(this, Size(500, 60));
	tableView1->setDirection(ScrollView::Direction::HORIZONTAL);
	tableView1->setPosition(Vec2(600, 480));
	
	tableView1->setDelegate(this);
	tableView1->setTag(100);
	this->addChild(tableView1);
	tableView1->reloadData();
	count++;
	*/
	// Ʈ�� ���̺� ��
/*	TableView* tableView2 = TableView::create(this, Size(500, 60));
	tableView2->setDirection(ScrollView::Direction::HORIZONTAL);
	tableView2->setPosition(Vec2(600, 360));

	tableView2->setDelegate(this);
	tableView2->setTag(200);
	this->addChild(tableView2);
	tableView2->reloadData();
	count++;

	// ����� ���̺� ��
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
	log("���׷��̵�");
}
void ShopScene::buy(Ref * pSender)
{
	log("����!");
}
