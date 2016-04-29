#include "Monster.h"
#include "HelloWorldScene.h"
#include <GLES-Render.h>
#include "DataSingleTon.h"

USING_NS_CC;

// 생성자 변수 초기화와 공용 변수 불러오기
Monster::Monster(Vec2 position,int monsterType)
	:hp(100),
	xSpeed(0.05)
{
	this->position = position;
	this->monsterType = monsterType;

	_world = DataSingleTon::getInstance()->get_world();
	gameLayer = DataSingleTon::getInstance()->getGameLayer();

	body = addNewSprite(position, Size(30, 40), b2_dynamicBody, 0);
	this->schedule(schedule_selector(Monster::moving));
}

b2Body* Monster::addNewSprite(Vec2 point, Size size, b2BodyType bodytype, int type)
{
	//바디데프를 만들고 속성들을 지정한다.
	b2BodyDef bodyDef;
	bodyDef.type = bodytype;
	auto animation = Animation::create();
	animation->setDelayPerUnit(0.05f);
	Texture2D* texture;
	Sprite* zombie;
	if(monsterType == BrainZombie)
	{
		auto sprite = Sprite::create("brainZombie_Move.png");
		texture = sprite->getTexture();
		for (int i = 0; i < 15; i++)
		{
			animation->addSpriteFrameWithTexture(texture, Rect(i * 40, 0, 40, 45));
		}
		zombie = Sprite::create("brainZombie_Move.png", Rect(0, 0, 40, 45));
	}
	else if (monsterType == FatZombie)
	{
		auto sprite = Sprite::create("FatZombie_Move.png");
		texture = sprite->getTexture();
		for (int i = 0; i < 14; i++)
		{
			animation->addSpriteFrameWithTexture(texture, Rect(i * 40, 0, 40, 45));
		}
		zombie = Sprite::create("FatZombie_Move.png", Rect(0, 0, 40, 45));
	}
	else if (monsterType == SuperZombie)
	{
		auto sprite = Sprite::create("superZombie_Move.png");
		texture = sprite->getTexture();
		for (int i = 0; i < 12; i++)
		{
			animation->addSpriteFrameWithTexture(texture, Rect(i * 50, 0, 50, 45));
		}
		zombie = Sprite::create("superZombie_Move.png", Rect(0, 0, 50, 45));
	}

	zombie->setTag(MONSTER);
	gameLayer->addChild(zombie);

	auto animate = Animate::create(animation);
	auto rep = RepeatForever::create(animate);
	zombie->runAction(rep);



	bodyDef.position.Set(point.x / PTM_RATIO, point.y / PTM_RATIO);
	bodyDef.userData = zombie;

	bodyDef.fixedRotation = true;

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
	//Define the dynamic body fixture.
	//밀도
	fixtureDef.density = 0.5f;
	// 마찰력 - 0 ~ 1
	fixtureDef.friction = 1.0f;
	//반발력 - 물체가 다른 물체에 닿았을 때 튕기는 값
	fixtureDef.restitution = 1.0;

	body->SetLinearDamping(2.0);
	body->CreateFixture(&fixtureDef);

	// HP바 스프라이트
	hpBar = Sprite::create("white-512x512.png");
	hpBar->setTextureRect(Rect(0, 0, 25, 5));
	hpBar->setColor(Color3B::RED);

	Size parentSize;
	parentSize = zombie->getContentSize();
	hpBar->setPosition(Vec2(parentSize.width / 2.0, parentSize.height + 10));
	zombie->addChild(hpBar);

	return body;
}


Monster::~Monster()
{
	this->unschedule(schedule_selector(Monster::moving));
}

void Monster::moving(float dt)
{
	// HP에 따른 HP바 크기
	hpBar->setScaleX(hp / 100.0f);
	// 바디 이동
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
