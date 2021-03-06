﻿#include "Intro.h"
#include "HelloWorldScene.h"
#include "ResouceLoad.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
USING_NS_CC;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//#include "platform\android\jni\JniHelper.h"

void callJavaMethod(std::string func)
{
	JniMethodInfo t;
	/*
	JniHelper를 통해 org/cocos2dx/application/에 있는
	AppActivity class의 JniTestFunc함수 정보르 가져온다
	*/
	if (JniHelper::getStaticMethodInfo(t
		, "org.cocos2dx.cpp.AppActivity"
		, func.c_str()
		, "()V"))
	{
		//함수 호출
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		// Release
		t.env->DeleteLocalRef(t.classID);
	}
}
#else 
//#include "Util/Admob/LayerAdmob.h"
#endif
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


#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	callJavaMethod("ShowAdFull");
#else
	// ShowAdmobFullAds();
#endif


	ResouceLoad::getInstance();
	winSize = Director::getInstance()->getWinSize();

	m_nSoundId = SimpleAudioEngine::getInstance()->playEffect("sounds/intro_backgrund.ogg");


	auto background = Sprite::create("intro/intro_background.png");
	background->setPosition(winSize.width / 2, winSize.height / 2);
	this->addChild(background);
	
	
	
	auto pMenuItem = MenuItemImage::create(
		"ui/play.png",
		"ui/play_push.png",
		CC_CALLBACK_1(Intro::playbtn, this));
	pMenuItem->setScaleX(1.2);
	pMenuItem->setScaleY(1.4);
	pMenu = Menu::create(pMenuItem, nullptr);
	pMenu->setPosition(Vec2(winSize.width / 2, 100));
	this->addChild(pMenu);

	doRain();

	this->schedule(schedule_selector(Intro::tick2));
	this->schedule(schedule_selector(Intro::tick),2.3);

	return true;
}

void Intro::tick(float dt)
{
	m_nSoundId = SimpleAudioEngine::getInstance()->playEffect("sounds/intro_backgrund.ogg");
}

void Intro::tick2(float dt)
{
	/*if (rand_0_1() < 0.002) {
		SimpleAudioEngine::getInstance()->playEffect("sounds/zombie_howl.wav");
	}*/
}
void Intro::doRain()
{
	ParticleSystem* m_emitter = ParticleRain::create();
	m_emitter->retain();

	Point p = m_emitter->getPosition();
	m_emitter->setPosition(Vec2(p.x, p.y - 100));
	m_emitter->setLife(4);
	auto texture = Director::getInstance()->getTextureCache()->addImage("explosion/fire.png");
	
	m_emitter->setTexture(texture);
	m_emitter->setColor(Color3B(166, 166, 166));
	m_emitter->setScaleY(15);

	if (m_emitter != NULL)
	{
		m_emitter->setPosition(Vec2(winSize.width/2, winSize.height));

		this->addChild(m_emitter);
	}
}


void Intro::playbtn(Ref * pSender)
{
	pMenu->setEnabled(false);
	SimpleAudioEngine::getInstance()->playEffect("sounds/menuSelect.ogg");
	this->unschedule(schedule_selector(Intro::tick2));
	this->unschedule(schedule_selector(Intro::tick));
	SimpleAudioEngine::getInstance()->stopEffect(m_nSoundId);

	doReplaceScene();
}

// GameScene로 전환
void Intro::doReplaceScene()
{
	auto pScene = HelloWorld::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(2,pScene));
}

