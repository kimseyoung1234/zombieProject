#include "Bullet.h"
#include "HelloWorldScene.h"
#include <GLES-Render.h>
#include "DataSingleTon.h"

USING_NS_CC;

// 생성자 변수 초기화와 공용 변수 불러오기
Bullet::Bullet(Vec2 nPos2, int bulletType)
{
	_world = DataSingleTon::getInstance()->get_world();
	gameLayer = DataSingleTon::getInstance()->getGameLayer();

	this->bulletType = bulletType;

	body = this->addNewSprite(nPos2, Size(9, 9), b2_dynamicBody, 1);

}

b2Body* Bullet::addNewSprite(Vec2 point, Size size, b2BodyType bodytype, int type)
{
	//바디데프를 만들고 속성들을 지정한다.
	b2BodyDef bodyDef;

	auto sprite = Sprite::create("bullet1.png");
	sprite->setTag(BULLET);
	gameLayer->addChild(sprite);

	bodyDef.type = bodytype;
	bodyDef.position.Set(point.x / PTM_RATIO, point.y / PTM_RATIO);
	bodyDef.userData = sprite;
	bodyDef.bullet = true;
	bodyDef.fixedRotation = true;

	//bodyDef.linearDamping = 0.5;
	// 월드에 바디데프의 정보로 바디를 만든다
	b2Body *body = _world->CreateBody(&bodyDef);

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

	fixtureDef.filter.groupIndex = -10;
	//Define the dynamic body fixture.
	//밀도
	fixtureDef.density = 5.0f;
	// 마찰력 - 0 ~ 1
	fixtureDef.friction = 0.0f;
	//반발력 - 물체가 다른 물체에 닿았을 때 튕기는 값
	fixtureDef.restitution = 0.0;

	body->CreateFixture(&fixtureDef);

	return body;
}


Bullet::~Bullet()
{
}
/*
void Monster::onEnter()
{
Node::onEnter();

}
void Monster::onExit()
{
Node::onExit();
}*/
