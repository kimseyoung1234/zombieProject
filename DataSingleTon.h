#ifndef __SINGLETON__H__
#define __SINGLETON__H__
#include "cocos2d.h"
#include "HelloWorldScene.h"


class DataSingleTon {
private:
	static DataSingleTon *m_pInstance;
	b2World* _world;
	DataSingleTon();
public:
	
	static DataSingleTon *getInstance();
	b2World* get_world();
//	void set_world(b2World* sender);
};

#endif