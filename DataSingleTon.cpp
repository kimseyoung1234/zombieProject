#include "DataSingleton.h"
#include "cocos2d.h"
USING_NS_CC;

DataSingleTon * DataSingleTon::m_pInstance = nullptr;

DataSingleTon::DataSingleTon() {
	// 공용변수들 초기화
	b2Vec2 gravity = b2Vec2(0.0f, 0.0f);
	_world = new b2World(gravity);
	monsters = new std::vector<Monster *>;
	bullets = new std::vector<Bullet*>;
	barricade = new std::vector<b2Body*>;
	traps = new std::vector<Trap*>;
	helpers = new std::vector<Helper*>;
	winSize = Director::getInstance()->getWinSize();
	gameLayer = LayerColor::create(Color4B(0, 0, 0, 0),winSize.width,winSize.height);
	menuLayer = LayerColor::create(Color4B(0, 0, 0, 0), winSize.width, winSize.height);
	shopLayer = LayerColor::create(Color4B(0, 0, 0, 0), winSize.width, winSize.height);

}
DataSingleTon::~DataSingleTon()
{
	delete monsters;
	delete bullets;
	delete barricade;
	delete traps;
	delete helpers;
	log("데이타 싱글톤 삭제");
}
DataSingleTon * DataSingleTon::getInstance()
{

	if (!m_pInstance)
	{
		m_pInstance = new DataSingleTon;
	}
	return m_pInstance;
}

void DataSingleTon::FreeInstance()
{
	if (m_pInstance != nullptr)
	{
		
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

b2World* DataSingleTon::get_world()
{
	return _world;
}

std::vector<Monster*>* DataSingleTon::getMonsters()
{
	return monsters;
}

std::vector<Bullet*>* DataSingleTon::getBullets()
{
	return bullets;
}

std::vector<Trap*>* DataSingleTon::getTraps()
{
	return traps;
}

std::vector<Helper*>* DataSingleTon::getHelpers()
{
	return helpers;
}

LayerColor* DataSingleTon::getGameLayer()
{
	return gameLayer;
}
LayerColor* DataSingleTon::getMenuLayer()
{
	return menuLayer;
}
LayerColor* DataSingleTon::getShopLayer()
{
	return shopLayer;
}

std::vector<b2Body*>* DataSingleTon::getBarricade()
{
	return barricade;
}

void DataSingleTon::remove_anim(Node* sender)
{
	// 게임레이어 어케함
	auto sprite = (Sprite*)sender;
	if (sprite != nullptr) {
		gameLayer->removeChild(sprite);
		log("삭제완료");
	}
}