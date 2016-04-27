#include "Monster.h"
#include "HelloWorldScene.h"
#include <GLES-Render.h>
#include "DataSingleTon.h"

USING_NS_CC;

// ������ ���� �ʱ�ȭ�� ���� ���� �ҷ�����



Monster::Monster(Vec2 position)
	:hp(100),
	xSpeed(0.1)
{
	this->position = position;
	_world = DataSingleTon::getInstance()->get_world();
	gameLayer = DataSingleTon::getInstance()->getGameLayer();

	body = addNewSprite(position, Size(50, 50), b2_dynamicBody, 0);
	this->schedule(schedule_selector(Monster::moving));

}

b2Body* Monster::addNewSprite(Vec2 point, Size size, b2BodyType bodytype, int type)
{
	//�ٵ����� ����� �Ӽ����� �����Ѵ�.
	b2BodyDef bodyDef;
	bodyDef.type = bodytype;

	auto sprite = Sprite::create("mole_1.png");
	sprite->setTag(MONSTER);
	
	gameLayer->addChild(sprite);
	bodyDef.position.Set(point.x / PTM_RATIO, point.y / PTM_RATIO);
	bodyDef.userData = sprite;
	
	//bodyDef.bullet = true;
	bodyDef.fixedRotation = true;

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
	//Define the dynamic body fixture.
	//�е�
	fixtureDef.density = 0.5f;
	// ������ - 0 ~ 1
	fixtureDef.friction = 1.0f;
	//�ݹ߷� - ��ü�� �ٸ� ��ü�� ����� �� ƨ��� ��
	fixtureDef.restitution = 1.0;

	body->SetLinearDamping(2.0);
	body->CreateFixture(&fixtureDef);
	return body;
}


Monster::~Monster()
{
	this->unschedule(schedule_selector(Monster::moving));
}

void Monster::moving(float dt)
{
	body->SetTransform(b2Vec2(body->GetPosition().x - xSpeed,body->GetPosition().y), 0);
}

void Monster::onEnter()
{
	Node::onEnter();
	
}
void Monster::onExit()
{
	Node::onExit();
}
