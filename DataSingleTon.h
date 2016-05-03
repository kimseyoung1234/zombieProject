#ifndef __SINGLETON__H__
#define __SINGLETON__H__
#include "cocos2d.h"
#include "HelloWorldScene.h"
#include "Monster.h"
#include "Bullet.h"

// 모든 클래스에서 사용하기 위한 싱글톤 클래스
class DataSingleTon {
private:
	static DataSingleTon *m_pInstance;
	DataSingleTon();

	b2World* _world;
	std::vector<Monster*>* monsters;
	std::vector<Bullet*>* bullets;
	cocos2d::LayerColor* gameLayer;
	cocos2d::LayerColor* menuLayer;
	cocos2d::Size winSize;
	b2Body * groundBody;
public:
	
	b2Body * barricade;
	static DataSingleTon *getInstance();
	b2World* get_world();
	std::vector<Monster*>* getMonsters();
	std::vector<Bullet*>* getBullets();
	cocos2d::LayerColor* getGameLayer();
	cocos2d::LayerColor* getMenuLayer();
	b2Body* getBarricade();
};

#endif