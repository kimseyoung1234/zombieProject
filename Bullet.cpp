#include "Bullet.h"
#include "HelloWorldScene.h"
#include <GLES-Render.h>
#include "DataSingleTon.h"
#include "PlayerInfoSingleTon.h"
#include "MyQueryCallback.h"
USING_NS_CC;

// 생성자 변수 초기화와 공용 변수 불러오기
Bullet::Bullet(Vec2 nPos2, int bulletType,float angle)
{
	_world = DataSingleTon::getInstance()->get_world();
	gameLayer = DataSingleTon::getInstance()->getGameLayer();

	this->angle = angle;
	
	this->bulletType = bulletType;
	if (bulletType == 0) {
		this->damage = PlayerInfoSingleTon::getInstance()->machine_Damage;
	}
	else if (bulletType == 1)
	{
		this->damage = PlayerInfoSingleTon::getInstance()->ak_Damage;
	}
	
	else if (bulletType == 2)
	{
		monsters = DataSingleTon::getInstance()->getMonsters();
		this->damage = PlayerInfoSingleTon::getInstance()->sniper_Damage;
		this->schedule(schedule_selector(Bullet::tick));
	}
	body = this->addNewSprite(nPos2, Size(10, 10), b2_dynamicBody, 1);

}

b2Body* Bullet::addNewSprite(Vec2 point, Size size, b2BodyType bodytype, int type)
{
	//바디데프를 만들고 속성들을 지정한다.
	b2BodyDef bodyDef;

	auto sprite = Sprite::create("bullet1.png");
	sprite->setRotation(angle);
	sprite->setTag(BULLET);
	sprite->setScaleX(2.0f);
	
	this->sprite = sprite;
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
	if(bulletType == 2)
	{
		fixtureDef.density = 0.0f;
		fixtureDef.filter.categoryBits = 0x0008;
	}
	else {
		fixtureDef.density = 5.0f;
	}
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
// 저격총일 경우 몬스터들을 관통하면서 공격
void Bullet::tick(float dt)
{
	log("저격");
	MyQueryCallback queryCallback; //see "World querying topic"
	b2AABB aabb;

	b2Vec2 center = body->GetWorldCenter();

	float blastRadius = 0.8f;
	
	aabb.lowerBound = center - b2Vec2(blastRadius, blastRadius);
	aabb.upperBound = center + b2Vec2(blastRadius, blastRadius);
	_world->QueryAABB(&queryCallback, aabb);
	
	//check which of these bodies have their center of mass within the blast radius
	for (int i = 0; i < queryCallback.foundBodies.size(); i++) {
		b2Body* body = queryCallback.foundBodies[i];
		b2Vec2 bodyCom = body->GetWorldCenter();

		//ignore bodies outside the blast range
		if ((bodyCom - center).Length() >= blastRadius)
		{
			continue;
		}
		for (int k = 0; k < monsters->size(); k++)
		{
			b2Body * m_body = (b2Body*)monsters->at(k)->body;
			if (body == m_body)
			{
				if (monsters->at(k)->hitBullet != this->body) {
					monsters->at(k)->hitBullet = this->body;
					monsters->at(k)->hp = monsters->at(k)->hp - damage;
					monsters->at(k)->hpBar->setVisible(true);
					monsters->at(k)->hpBarShowTime = 0;
				}
				break;
			}
		}
	}
}
