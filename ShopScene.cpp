#include "ShopScene.h"
#include "DataSingleTon.h"
#include "PlayerInfoSingleTon.h"
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
	gameLayer = DataSingleTon::getInstance()->getGameLayer();
	traps = DataSingleTon::getInstance()->getTraps();
	helpers = DataSingleTon::getInstance()->getHelpers();
	
	auto shopLayer = LayerColor::create(Color4B(0, 0, 0, 0), winSize.width, winSize.height);

	this->addChild(shopLayer);

	auto weapon_tableLayer = new TableViewLayer(Vec2(300,480),2,WEAPON);
	auto weapon_view = weapon_tableLayer->getTableView();
	shopLayer->addChild(weapon_view);

	auto trap_tableLayer = new TableViewLayer(Vec2(300, 300), 1,TRAP);
	auto trap_view = trap_tableLayer->getTableView();
	shopLayer->addChild(trap_view);

	auto helper_tableLayer = new TableViewLayer(Vec2(300, 120), 1,HELPER);
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
	//Ʈ������ ���õȰ� ������
	if (PlayerInfoSingleTon::getInstance()->trapSeleted > -1)
	{
		auto trap = new Trap(Vec2(70, 70), 1);
		gameLayer->addChild(trap);
		traps->push_back(trap);
	}
	// ����̿��� ���õȰ� ������
	else if (PlayerInfoSingleTon::getInstance()->helperSeleted > -1)
	{
		auto helper = new Helper(Vec2(50, 200), 1);
		gameLayer->addChild(helper);
		helpers->push_back(helper);
	}
	// ���õ� ���� �ƹ� �͵� ������
	else
	{
		log("�� �� ������");
	}
}
