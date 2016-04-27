#include "DataSingleton.h"
#include "cocos2d.h"
USING_NS_CC;

DataSingleTon * DataSingleTon::m_pInstance = nullptr;

DataSingleTon::DataSingleTon() {
	// 공용변수들 초기화
	b2Vec2 gravity = b2Vec2(0.0f, 0.0f);
	_world = new b2World(gravity);
	monsters = new std::vector<Monster *>;
	removeBullets = new std::vector<b2Body*>;
	//dieMonsters = new std::vector<b2Body*>;
	winSize = Director::getInstance()->getWinSize();
	gameLayer = LayerColor::create(Color4B(0, 0, 0, 0),winSize.width,winSize.height);
}

DataSingleTon * DataSingleTon::getInstance()
{

	if (!m_pInstance)
	{
		m_pInstance = new DataSingleTon;
	}
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

std::vector<b2Body*>* DataSingleTon::getRemoveBullets()
{
	return removeBullets;
}

/*std::vector<b2Body*>* DataSingleTon::getDieMonsters()
{
	return dieMonsters;
}
*/
LayerColor* DataSingleTon::getGameLayer()
{
	return gameLayer;
}