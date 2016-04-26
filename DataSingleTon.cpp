#include "DataSingleton.h"
#include "cocos2d.h"
USING_NS_CC;

DataSingleTon * DataSingleTon::m_pInstance = nullptr;

DataSingleTon::DataSingleTon() {
	b2Vec2 gravity = b2Vec2(0.0f, 0.0f);
	_world = new b2World(gravity);
	monsters = new std::vector<Monster *>;
	winSize = Director::getInstance()->getWinSize();
	gameLayer = LayerColor::create(Color4B(0, 0, 0, 0),winSize.width,winSize.height);
}

DataSingleTon * DataSingleTon::getInstance()
{

	if (!m_pInstance)
	{
		m_pInstance = new DataSingleTon;
	}
	b2Vec2 gravity = b2Vec2(0.0f, 0.0f);


	return m_pInstance;
}

b2World* DataSingleTon::get_world()
{

	return _world;
}
std::vector<Monster*>* DataSingleTon::getMonsters()
{
	return monsters;
}

LayerColor* DataSingleTon::getGameLayer()
{
	return gameLayer;
}