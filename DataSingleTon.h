#ifndef __SINGLETON__H__
#define __SINGLETON__H__
#include "cocos2d.h"
#include "HelloWorldScene.h"
#include "Monster.h"

// 모든 클래스에서 사용하기 위한 싱글톤 클래스
class DataSingleTon {
private:
	static DataSingleTon *m_pInstance;
	DataSingleTon();

	b2World* _world;
	std::vector<Monster*>* monsters;
	std::vector<b2Body*>* removeBullets;
	cocos2d::LayerColor* gameLayer;
	cocos2d::LayerColor* menuLayer;
	cocos2d::Size winSize;
	b2Body * groundBody;
public:
	
	static DataSingleTon *getInstance();
	b2World* get_world();
	std::vector<Monster*>* getMonsters();
	std::vector<b2Body*>* getRemoveBullets();
	cocos2d::LayerColor* getGameLayer();
	cocos2d::LayerColor* getMenuLayer();


};

#endif