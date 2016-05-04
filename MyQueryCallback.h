#ifndef __MYQUERYCALLBACK__H__
#define __MYQUERYCALLBACK__H__
#include "cocos2d.h"


class MyQueryCallback : public b2QueryCallback {
public:
	std::vector<b2Body*> foundBodies;

	bool ReportFixture(b2Fixture* fixture) {
		foundBodies.push_back(fixture->GetBody());
		return true;//keep going to find all fixtures in the query area
	}
};

//in Step() function
MyQueryCallback queryCallback;
b2AABB aabb;
aabb.lowerBound = lower;
aabb.upperBound = upper;
m_world->QueryAABB(&queryCallback, aabb);


//draw a point on each body in the query area
glPointSize(6);
glBegin(GL_POINTS);
for (int i = 0; i < queryCallback.foundBodies.size(); i++) {
	b2Vec2 pos = queryCallback.foundBodies[i]->GetPosition();
	glVertex2f(pos.x, pos.y);
}
glEnd();

#endif