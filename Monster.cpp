#include "Monster.h"
#include "HelloWorldScene.h"
#include <GLES-Render.h>
#include "DataSingleTon.h"
#include "MonsterInfoSingleTon.h"
#include "PlayerInfoSingleTon.h"
USING_NS_CC;

// 생성자 변수 초기화와 공용 변수 불러오기
Monster::Monster(Vec2 position,int monsterType)
{
	_world = DataSingleTon::getInstance()->get_world();
	gameLayer = DataSingleTon::getInstance()->getGameLayer();

	this->position = position;
	this->monsterType = monsterType;
	
	if (monsterType == BrainZombie)
	{
		int direction = random(0, 1);
		if (direction == 0)
		{
			direction = -1;
		}

		this->hp = MonsterInfoSingleTon::getInstance()->brainZombie_HP;
		this->damage = MonsterInfoSingleTon::getInstance()->brainZombie_damage;
		this->xSpeed = MonsterInfoSingleTon::getInstance()->brainZombie_xSpeed;
		this->ySpeed = MonsterInfoSingleTon::getInstance()->brainZomie_ySpeed * direction;
	}
	else if (monsterType == FatZombie)
	{

		this->hp = MonsterInfoSingleTon::getInstance()->fatZombie_HP;
		this->damage = MonsterInfoSingleTon::getInstance()->fatZombie_damage;
		this->xSpeed = MonsterInfoSingleTon::getInstance()->fatZombie_xSpeed;
		this->ySpeed = MonsterInfoSingleTon::getInstance()->fatZomie_ySpeed;
	}
	else if (monsterType == SuperZombie)
	{
		this->hp = MonsterInfoSingleTon::getInstance()->superZombie_HP;
		this->damage = MonsterInfoSingleTon::getInstance()->superZombie_damage;
		this->xSpeed = MonsterInfoSingleTon::getInstance()->superZombie_xSpeed;
		this->ySpeed = MonsterInfoSingleTon::getInstance()->superZomie_ySpeed;
	}
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
		auto sprite = Sprite::create("monster/brain_move.png");
		texture = sprite->getTexture();
		for (int i = 0; i < 15; i++)
		{
			animation->addSpriteFrameWithTexture(texture, Rect(i * 64, 0, 64, 64));
		}
		zombie = Sprite::create("brainZombie_Move.png", Rect(0, 0, 64, 64));
	}
	else if (monsterType == FatZombie)
	{
		auto sprite = Sprite::create("fatZombie_Move.png");
		texture = sprite->getTexture();
		for (int i = 0; i < 14; i++)
		{
			animation->addSpriteFrameWithTexture(texture, Rect(i * 40, 0, 40, 45));
		}
		zombie = Sprite::create("fatZombie_Move.png", Rect(0, 0, 40, 45));
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
	this->sprite = zombie;
	zombie->setTag(MONSTER);
	zombie->setScale(1.5f);
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
	fixtureDef.restitution = 0.1;

	// 같은 몬스터끼리는 충돌하지 않는다
	//fixtureDef.filter.groupIndex = -20;

	body->SetLinearDamping(5.0f);
	body->CreateFixture(&fixtureDef);

	// HP바 스프라이트
	hpBar = Sprite::create("white-512x512.png");
	hpBar->setTextureRect(Rect(0, 0, 25, 5));
	hpBar->setColor(Color3B::RED);
	hpBar->setVisible(false);
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
	yTurnTime = yTurnTime + dt;
	attackDelay = attackDelay + dt;
	hpBarShowTime = hpBarShowTime + dt;
	// HP에 따른 HP바 크기
	hpBar->setScaleX(hp / 100.0f);
	// 바디 이동
	
	
	if (yTurnTime >= 2.0)
	{
		this->ySpeed = this->ySpeed * -1;
		yTurnTime = 0;
	}

	// 몬스터 피격스 3초동안만 hpBar가 보임
	if (hpBarShowTime >= 3.0)
	{
		hpBar->setVisible(false);
	}

	// 바리게이트와 충돌 시 2초마다 공격
	if (isAttack)
	{
		if (present_ani != ATTACK) {
			// 공격 애니메이션
			sprite->stopAllActions();
			auto sprite1 = Sprite::create("monster/brain_attack.png");
			auto texture = sprite1->getTexture();
			auto animation = Animation::create();
			animation->setDelayPerUnit(0.074f);

			for (int i = 0; i < 27; i++)
			{
				int column = i % 15;
				int row = i / 15;
				animation->addSpriteFrameWithTexture(texture, Rect(column * 64, row * 64, 64, 64));
			}

			auto animate = Animate::create(animation);
			auto rep = RepeatForever::create(animate);
			sprite->runAction(rep);
			present_ani = ATTACK;
		}
		if (attackDelay >= 2.0)
		{
			PlayerInfoSingleTon::getInstance()->hp = PlayerInfoSingleTon::getInstance()->hp - damage;
			log("플레이어 hp : %d", PlayerInfoSingleTon::getInstance()->hp);
			attackDelay = 0;
		}
	}
	else if(!isAttack){
		if (present_ani != MOVE) {
			sprite->stopAllActions();
			auto sprite1 = Sprite::create("monster/brain_move.png");
			auto texture = sprite1->getTexture();
			auto animation = Animation::create();
			animation->setDelayPerUnit(0.15f);

			for (int i = 0; i < 15; i++)
			{
				int column = i % 15;
				int row = i / 15;
				animation->addSpriteFrameWithTexture(texture, Rect(column * 64, row * 64, 64, 64));
			}
			auto animate = Animate::create(animation);
			auto rep = RepeatForever::create(animate);
			sprite->runAction(rep);
			present_ani = MOVE;
		}

		body->SetTransform(b2Vec2(body->GetPosition().x - xSpeed, body->GetPosition().y - ySpeed), 0);
	}
}