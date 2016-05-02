#include "Bullet.h"
#include "HelloWorldScene.h"
#include <GLES-Render.h>
#include "DataSingleTon.h"

USING_NS_CC;

// ������ ���� �ʱ�ȭ�� ���� ���� �ҷ�����
Bullet::Bullet(Vec2 nPos2, int bulletType)
{
	_world = DataSingleTon::getInstance()->get_world();
	gameLayer = DataSingleTon::getInstance()->getGameLayer();

	this->bulletType = bulletType;

	body = this->addNewSprite(nPos2, Size(9, 9), b2_dynamicBody, 1);

}

b2Body* Bullet::addNewSprite(Vec2 point, Size size, b2BodyType bodytype, int type)
{
	//�ٵ����� ����� �Ӽ����� �����Ѵ�.
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
	// ���忡 �ٵ����� ������ �ٵ� �����
	b2Body *body = _world->CreateBody(&bodyDef);

	//�ٵ� ������ ���� �Ӽ��� �ٵ��� ����� �����
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
	//�е�
	fixtureDef.density = 5.0f;
	// ������ - 0 ~ 1
	fixtureDef.friction = 0.0f;
	//�ݹ߷� - ��ü�� �ٸ� ��ü�� ����� �� ƨ��� ��
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
