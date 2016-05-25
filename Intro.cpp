#include "Intro.h"
#include "HelloWorldScene.h"
#include "ResouceLoad.h"
USING_NS_CC;

Scene* Intro::createScene()
{

	auto scene = Scene::create();

	auto layer = Intro::create();

	scene->addChild(layer);

	return scene;
}


bool Intro::init()
{

	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 0)))
	{
		return false;
	}
	ResouceLoad::getInstance();
	winSize = Director::getInstance()->getWinSize();

	auto background = Sprite::create("intro/intro_background.png");
	background->setPosition(winSize.width / 2, winSize.height / 2);
	this->addChild(background);

	
	auto pMenuItem = MenuItemImage::create(
		"ui/play.png",
		"ui/play_push.png",
		CC_CALLBACK_1(Intro::playbtn, this));
	pMenuItem->setScaleX(1.2);
	pMenuItem->setScaleY(1.4);
	auto pMenu = Menu::create(pMenuItem, nullptr);
	pMenu->setPosition(Vec2(winSize.width / 2, 100));
	this->addChild(pMenu);

	return true;
}


void Intro::playbtn(Ref * pSender)
{
	doReplaceScene();
}

// GameScene로 전환
void Intro::doReplaceScene()
{
	auto pScene = HelloWorld::createScene();
	Director::getInstance()->replaceScene(pScene);
}

