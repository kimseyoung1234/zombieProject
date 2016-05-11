#include "MyQueryCallback.h"
USING_NS_CC;

bool MyQueryCallback::ReportFixture(b2Fixture* fixture) {

	foundBodies.push_back(fixture->GetBody());

	return true;
}