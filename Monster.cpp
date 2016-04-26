#include "Monster.h"
#include "HelloWorldScene.h"
#include <GLES-Render.h>
#include "DataSingleTon.h"
USING_NS_CC;

Monster::Monster()
	:hp(100),
	speed(0)
{
	_world = DataSingleTon::getInstance()->get_world();
//	body = addNewSprite(Vec2(HelloWorld::getInstance()->winSize.width - 100, HelloWorld::getInstance()->winSize.width / 2), Size(50, 50), b2_dynamicBody, 0);
	body = addNewSprite(Vec2(1200,480), Size(50, 50), b2_dynamicBody, 0);
}

b2Body* Monster::addNewSprite(Vec2 point, Size size, b2BodyType bodytype, int type)
{
	
	
	//바디데프를 만들고 속성들을 지정한다.
	b2BodyDef bodyDef;
	bodyDef.type = bodytype;

	bodyDef.position.Set(point.x / PTM_RATIO, point.y / PTM_RATIO);
	bodyDef.userData = nullptr;
	//bodyDef.bullet = true;
	bodyDef.fixedRotation = true;

	// 월드에 바디데프의 정보로 바디를 만든다
	b2Body *body = _world->CreateBody(&bodyDef);
	log("미미");
	//바디에 적용할 물리 속석용 바디의 모양을 만든다
	b2FixtureDef fixtureDef;
	b2PolygonShape dynamicBox;
	b2CircleShape circle;

	if (type == 0)
	{
		dynamicBox.SetAsBox(size.width / 2 / PTM_RATIO, size.height / 2 / PTM_RATIO);
		fixtureDef.shape = &dynamicBox;
	}
	else
	{
		circle.m_radius = (size.width / 2) / PTM_RATIO;
		fixtureDef.shape = &circle;
	}

	//Define the dynamic body fixture.
	//밀도
	fixtureDef.density = 1.0f;
	// 마찰력 - 0 ~ 1
	fixtureDef.friction = 1.0f;
	//반발력 - 물체가 다른 물체에 닿았을 때 튕기는 값
	fixtureDef.restitution = 0.0;

	body->CreateFixture(&fixtureDef);

	return body;
}
