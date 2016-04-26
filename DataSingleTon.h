#ifndef __SINGLETON__H__
#define __SINGLETON__H__
#include "cocos2d.h"
#include "HelloWorldScene.h"
#include "Monster.h"

// 모든 클래스에서 사용하기 위한 싱글톤 클래스
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