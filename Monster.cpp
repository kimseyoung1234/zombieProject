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


	Sprite* sprite1;
	Texture2D* texture1;
	Sprite* sprite2;
	Texture2D* texture2;
	Animation* animation1;
	Animation* animation2;
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

		// 공격 애니메이션 등록
		auto sprite1 = Sprite::create("monster/brain_attack.png");
		auto texture1 = sprite1->getTexture();
		auto animation1 = Animation::create();
		animation1->setDelayPerUnit(0.074f);

		for (int i = 0; i < 27; i++)
		{
			int column = i % 15;
			int row = i / 15;
			animation1->addSpriteFrameWithTexture(texture1, Rect(column * 64, row * 64, 64, 64));
		}

		attackAnimate = Animate::create(animation1);
		attackAnimate->retain();
		auto sprite2 = Sprite::create("monster/brain_move.png");
		auto texture2 = sprite2->getTexture();
		auto animation2 = Animation::create();
		animation2->setDelayPerUnit(0.05f);

		for (int i = 0; i < 15; i++)
		{
			int column = i % 15;
			int row = i / 15;
			animation2->addSpriteFrameWithTexture(texture2, Rect(column * 64, row * 64, 64, 64));
		}
		moveAnimate = Animate::create(animation2);
		moveAnimate->retain();
	}
	else if (monsterType == FatZombie)
	{

		this->hp = MonsterInfoSingleTon::getInstance()->fatZombie_HP;
		this->damage = MonsterInfoSingleTon::getInstance()->fatZombie_damage;
		this->xSpeed = MonsterInfoSingleTon::getInstance()->fatZombie_xSpeed;
		this->ySpeed = MonsterInfoSingleTon::getInstance()->fatZomie_ySpeed;

		auto sprite1 = Sprite::create("monster/fat_attack.png");
		auto texture1 = sprite1->getTexture();
		auto animation1 = Animation::create();
		animation1->setDelayPerUnit(0.074f);

		for (int i = 0; i < 18; i++)
		{
			int column = i % 7;
			int row = i / 7;
			animation1->addSpriteFrameWithTexture(texture1, Rect(column * 48, row * 48, 48, 48));
		}

		attackAnimate = Animate::create(animation1);
		attackAnimate->retain();
		auto sprite2 = Sprite::create("monster/fat_move.png");
		auto texture2 = sprite2->getTexture();
		auto animation2 = Animation::create();
		animation2->setDelayPerUnit(0.05f);

		for (int i = 0; i < 14; i++)
		{
			int column = i % 7;
			int row = i / 7;
			animation2->addSpriteFrameWithTexture(texture2, Rect(column * 48, row * 48, 48, 48));
		}
		moveAnimate = Animate::create(animation2);
		moveAnimate->retain();
	}
	else if (monsterType == SuperZombie)
	{
		this->hp = MonsterInfoSingleTon::getInstance()->superZombie_HP;
		this->damage = MonsterInfoSingleTon::getInstance()->superZombie_damage;
		this->xSpeed = MonsterInfoSingleTon::getInstance()->superZombie_xSpeed;
		this->ySpeed = MonsterInfoSingleTon::getInstance()->superZomie_ySpeed;


		auto sprite1 = Sprite::create("monster/super_attack.png");
		auto texture1 = sprite1->getTexture();
		auto animation1 = Animation::create();
		animation1->setDelayPerUnit(0.074f);

		for (int i = 0; i < 20; i++)
		{
			int column = i % 7;
			int row = i / 7;
			animation1->addSpriteFrameWithTexture(texture1, Rect(column * 64, row * 64, 64, 64));
		}

		attackAnimate = Animate::create(animation1);
		attackAnimate->retain();
		auto sprite2 = Sprite::create("monster/super_move.png");
		auto texture2 = sprite2->getTexture();
		auto animation2 = Animation::create();
		animation2->setDelayPerUnit(0.05f);

		for (int i = 0; i < 12; i++)
		{
			int column = i % 7;
			int row = i / 7;
			animation2->addSpriteFrameWithTexture(texture2, Rect(column * 64, row * 64, 64, 64));
		}
		moveAnimate = Animate::create(animation2);
		moveAnimate->retain();
	}
	body = addNewSprite(position, Size(30, 40), b2_dynamicBody, 0);

	this->schedule(schedule_selector(Monster::moving));
}

b2Body* Monster::addNewSprite(Vec2 point, Size size, b2BodyType bodytype, int type)
{
	//바디데프를 만들고 속성들을 지정한다.
	b2BodyDef bodyDef;
	bodyDef.type = bodytype;
	//auto animation = Animation::create();
//	animation->setDelayPerUnit(0.05f);
	Texture2D* texture;
	Sprite* zombie;
	if(monsterType == BrainZombie)
	{
		auto sprite = Sprite::create("monster/brain_move.png");
	
		zombie = Sprite::create("monster/brain_move.png", Rect(0, 0, 64, 64));
	}
	else if (monsterType == FatZombie)
	{
		auto sprite = Sprite::create("monster/fat_move.png");

		zombie = Sprite::create("monster/fat_move.png", Rect(0, 0, 48, 48));
	}
	else if (monsterType == SuperZombie)
	{
		auto sprite = Sprite::create("superZombie_Move.png");

		zombie = Sprite::create("superZombie_Move.png", Rect(0, 0, 50, 45));
	}
	this->sprite = zombie;
	zombie->setTag(MONSTER);
	zombie->setScale(1.5f);
	gameLayer->addChild(zombie);

	//auto animate = Animate::create(animation);
	auto rep = RepeatForever::create(moveAnimate);
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

	// 공격중
	if (isAttack)
	{
		if (present_ani != ATTACK) {
			// 공격 애니메이션
			sprite->stopAllActions();
			auto rep = RepeatForever::create(attackAnimate);
			
			sprite->runAction(rep);
			present_ani = ATTACK;
		}
		// 바리게이트와 충돌 시 2초마다 공격
		if (attackDelay >= 2.0)
		{
			PlayerInfoSingleTon::getInstance()->hp = PlayerInfoSingleTon::getInstance()->hp - damage;
			log("플레이어 hp : %d", PlayerInfoSingleTon::getInstance()->hp);
			attackDelay = 0;
		}
	}
	else if(!isAttack){
		if (present_ani != MOVE) {
			// 무브 애니메이션
			sprite->stopAllActions();
			auto rep = RepeatForever::create(moveAnimate);
			sprite->runAction(rep);
			present_ani = MOVE;
		}
		// 바디 이동
		body->SetTransform(b2Vec2(body->GetPosition().x - xSpeed, body->GetPosition().y - ySpeed), 0);
	}
}