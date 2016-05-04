#ifndef __MYQUERYCALLBACK__H__
#define __MYQUERYCALLBACK__H__
#include "cocos2d.h"
#include "Box2D/Box2D.h"

class MyQueryCallback : public b2QueryCallback {
public:
	std::vector<b2Body*> foundBodies;
	virtual bool ReportFixture(b2Fixture* fixture);
};

#endif