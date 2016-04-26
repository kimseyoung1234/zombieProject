#ifndef __SINGLETON__H__
#define __SINGLETON__H__
#include "cocos2d.h"
#include "HelloWorldScene.h"
#include "Monster.h"

class DataSingleTon {
private:
	static DataSingleTon *m_pInstance;
	b2World* _world;
	std::vector<Monster*>* monsters;
	cocos2d::LayerColor* gameLayer;
	DataSingleTon();
	cocos2d::Size winSize;
public:
	
	static DataSingleTon *getInstance();
	b2World* get_world();
	std::vector<Monster*>* getMonsters();
	cocos2d::LayerColor* getGameLayer();

};

#endif