#include "MyQueryCallback.h"
USING_NS_CC;

bool MyQueryCallback::ReportFixture(b2Fixture* fixture) {

	foundBodies.push_back(fixture->GetBody());

	log("범위안에있다");
	return true;
}