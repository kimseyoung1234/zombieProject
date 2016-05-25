#ifndef __SINGLETON__H__
#define __SINGLETON__H__
#include "cocos2d.h"
#include "HelloWorldScene.h"
#include "Monster.h"
#include "Bullet.h"
#include "Trap.h"
#include "Helper.h"
#include "ContactListener.h"
// 모든 클래스에서 사용하기 위한 싱글톤 클래스
class DataSingleTon {
private:
	static DataSingleTon *m_pInstance;
	DataSingleTon();
	~DataSingleTon();
	b2World* _world;
	std::vector<b2Body*> * barricade;
	std::vector<Monster*>* monsters;
	std::vector<Bullet*>* bullets;
	std::vector<Trap *>* traps;
	std::vector<Helper *>* helpers;

	cocos2d::LayerColor* gameLayer;
	cocos2d::LayerColor* menuLayer;
	cocos2d::LayerColor* shopLayer;
	cocos2d::Size winSize;
	b2Body * groundBody;

public:

	static void FreeInstance();
	static DataSingleTon *getInstance();
	b2World* get_world();

	std::vector<b2Body*> * getBarricade();
	std::vector<Monster*>* getMonsters();
	std::vector<Bullet*>* getBullets();
	std::vector<Helper*>* getHelpers();
	std::vector<Trap*>* getTraps();
	cocos2d::LayerColor* getGameLayer();
	cocos2d::LayerColor* getMenuLayer();
	cocos2d::LayerColor* getShopLayer();

	void remove_anim(Node* sender);
	
};

#endif