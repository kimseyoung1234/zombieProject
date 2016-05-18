#include "Monster.h"
#include "HelloWorldScene.h"
#include <GLES-Render.h>
#include "DataSingleTon.h"
#include "MonsterInfoSingleTon.h"
#include "PlayerInfoSingleTon.h"
#include "ResouceLoad.h"
USING_NS_CC;

// ������ ���� �ʱ�ȭ�� ���� ���� �ҷ�����
Monster::Monster(Vec2 position,int monsterType)
{
	_world = DataSingleTon::getInstance()->get_world();
	gameLayer = DataSingleTon::getInstance()->getGameLayer();

	this->position = position;
	this->monsterType = monsterType;


	if (monsterType == BrainZombie)
	{
		this->hp = MonsterInfoSingleTon::getInstance()->brainZombie_HP;
		this->damage = MonsterInfoSingleTon::getInstance()->brainZombie_damage;
		this->xSpeed = MonsterInfoSingleTon::getInstance()->brainZombie_xSpeed;
		this->ySpeed = MonsterInfoSingleTon::getInstance()->brainZomie_ySpeed;

		// ���� �ִϸ��̼� ���
		
		attackAnimate = ResouceLoad::getInstance()->brain_attackAnimate->clone();
		attackAnimate->retain();

		attack2Animate = ResouceLoad::getInstance()->brain_attack2Animate->clone();
		attack2Animate->retain();
		
		moveAnimate = ResouceLoad::getInstance()->brain_moveAnimate->clone();
		moveAnimate->retain();
	}
	else if (monsterType == FatZombie)
	{

		this->hp = MonsterInfoSingleTon::getInstance()->fatZombie_HP;
		this->damage = MonsterInfoSingleTon::getInstance()->fatZombie_damage;
		this->xSpeed = MonsterInfoSingleTon::getInstance()->fatZombie_xSpeed;
		this->ySpeed = MonsterInfoSingleTon::getInstance()->fatZomie_ySpeed;

		attackAnimate = ResouceLoad::getInstance()->fat_attackAnimate->clone();
		attackAnimate->retain();

		attack2Animate = ResouceLoad::getInstance()->fat_attack2Animate->clone();
		attack2Animate->retain();

		moveAnimate = ResouceLoad::getInstance()->fat_moveAnimate->clone();
		moveAnimate->retain();
	}
	else if (monsterType == SuperZombie)
	{
		this->hp = MonsterInfoSingleTon::getInstance()->superZombie_HP;
		this->damage = MonsterInfoSingleTon::getInstance()->superZombie_damage;
		this->xSpeed = MonsterInfoSingleTon::getInstance()->superZombie_xSpeed;
		this->ySpeed = MonsterInfoSingleTon::getInstance()->superZomie_ySpeed;


		attackAnimate = ResouceLoad::getInstance()->super_attackAnimate->clone();
		attackAnimate->retain();

		attack2Animate = ResouceLoad::getInstance()->super_attack2Animate->clone();
		attack2Animate->retain();

		moveAnimate = ResouceLoad::getInstance()->super_moveAnimate->clone();
		moveAnimate->retain();
	}

	else if (monsterType == BossZombie)
	{
		this->hp = MonsterInfoSingleTon::getInstance()->bossZombie_HP;
		this->damage = MonsterInfoSingleTon::getInstance()->bossZombie_damage;
		this->xSpeed = MonsterInfoSingleTon::getInstance()->bossZombie_xSpeed;
		this->ySpeed = MonsterInfoSingleTon::getInstance()->bossZomie_ySpeed;

		attackAnimate = ResouceLoad::getInstance()->boss_attackAnimate->clone();
		attackAnimate->retain();

		attack2Animate = ResouceLoad::getInstance()->boss_attack2Animate->clone();
		attack2Animate->retain();

		moveAnimate = ResouceLoad::getInstance()->boss_moveAnimate->clone();
		moveAnimate->retain();
	}
	if (monsterType == BossZombie)
	{
		body = addNewSprite(position, Size(140, 50), b2_dynamicBody, 0);
	}
	else
	{
		body = addNewSprite(position, Size(30, 50), b2_dynamicBody, 0);
	}
	this->schedule(schedule_selector(Monster::moving));
}

b2Body* Monster::addNewSprite(Vec2 point, Size size, b2BodyType bodytype, int type)
{
	//�ٵ����� ����� �Ӽ����� �����Ѵ�.
	b2BodyDef bodyDef;
	bodyDef.type = bodytype;
	//auto animation = Animation::create();
//	animation->setDelayPerUnit(0.05f);
	Texture2D* texture;
	Sprite* zombie;
	//�ٵ� ������ ���� �Ӽ��� �ٵ��� ����� �����
	b2FixtureDef fixtureDef;
	//Define the dynamic body fixture.
	//�е�
	fixtureDef.density = 0.5f;
	// ������ - 0 ~ 1
	fixtureDef.friction = 1.0f;
	//�ݹ߷� - ��ü�� �ٸ� ��ü�� ����� �� ƨ��� ��
	fixtureDef.restitution = 0.1;

	if(monsterType == BrainZombie)
	{
		auto sprite = Sprite::create("monster/brain_move.png");
	
		zombie = Sprite::create("monster/brain_move.png", Rect(0, 0, 64, 64));
		zombie->setScale(1.75f);
	}
	else if (monsterType == FatZombie)
	{
		auto sprite = Sprite::create("monster/fat_move.png");

		zombie = Sprite::create("monster/fat_move.png", Rect(0, 0, 48, 48));
		zombie->setScale(1.75f);
	}
	else if (monsterType == SuperZombie)
	{
		auto sprite = Sprite::create("monster/super_move.png");

		zombie = Sprite::create("monster/super_move.png", Rect(0, 0, 64, 64));
		zombie->setScale(1.75f);
	}
	else if (monsterType == BossZombie)
	{
		auto sprite = Sprite::create("monster/boss_move.png");

		zombie = Sprite::create("monster/boss_move.png", Rect(0, 0, 100, 100));
		zombie->setScale(4.0f);

		fixtureDef.density = 7.0f;
	}
	this->sprite = zombie;
	zombie->setTag(MONSTER);

	gameLayer->addChild(zombie);

	//auto animate = Animate::create(animation);
	auto rep = RepeatForever::create(moveAnimate);
	rep->setTag(600);
	zombie->runAction(rep);


	bodyDef.position.Set(point.x / PTM_RATIO, point.y / PTM_RATIO);
	bodyDef.userData = zombie;
	
	bodyDef.fixedRotation = true;
	
	// ���忡 �ٵ����� ������ �ٵ� �����
	b2Body *body = _world->CreateBody(&bodyDef);

	//fixtureDef.filter.groupIndex = -12;
	fixtureDef.filter.categoryBits = 0x0002;
	fixtureDef.filter.maskBits = 0x0001 | 0x0002 | 0x0004;

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



	body->SetLinearDamping(7.0f);
	body->CreateFixture(&fixtureDef);

	// HP�� ��������Ʈ
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
	delete moveAnimate;
	delete attackAnimate;
	delete attack2Animate;
}

void Monster::moving(float dt)
{
	pipeTime = pipeTime + dt;
	//attackDelay = attackDelay + dt;
	hpBarShowTime = hpBarShowTime + dt;
	slowTime = slowTime + dt;
	stiffenTime = stiffenTime + dt;

	if (stiffenTime >= 0.1)
	{
		//log("���?");
		isHit = false;
	}

	if (slowTime >= 5.0)
	{
		isSlow = false;
	}
	// HP�� ���� HP�� ũ��
	hpBar->setScaleX(hp / 100.0f);
	
	
	if (pipeTime >= 3.0)
	{
		isPipe= false;
		pipe_positon = Vec2(0,0);
	}
	
	// ���� �ǰݽ� 3�ʵ��ȸ� hpBar�� ����
	if (hpBarShowTime >= 3.0)
	{
		hpBar->setVisible(false);
	}

	// ������
	if (isAttack)
	{
		if (present_ani != ATTACK) {
			// ���� �ִϸ��̼�
			sprite->stopActionByTag(600);
			sprite->stopActionByTag(700);
		//	sprite->stopActionByTag(800);
			auto seq = Sequence::create(attackAnimate,
				CallFunc::create(CC_CALLBACK_0(Monster::attackFinish, this)),
				attack2Animate,nullptr);
			auto rep = RepeatForever::create(seq);
			rep->setTag(700);                                    
			sprite->runAction(rep);
			present_ani = ATTACK;
		}
		// �ٸ�����Ʈ�� �浹 �� 2�ʸ��� ����
	}
	else if(!isAttack && !isHit){
		if (present_ani != MOVE) {
			// ���� �ִϸ��̼�
			sprite->stopActionByTag(600);
			sprite->stopActionByTag(700);
		//	sprite->stopActionByTag(800);
			sprite->setColor(Color3B::WHITE);
			auto rep = RepeatForever::create(moveAnimate);
			rep->setTag(600);
			sprite->runAction(rep);
			present_ani = MOVE;
		}
		// �ٵ� �̵�

		// ���ο�Ʈ�� �¾����� �̵�
		if (isSlow)
		{
			if(isPipe)
			{
				Vec2 transVector = pipe_positon - sprite->getPosition();
				transVector.normalize();

				if (transVector.x >= 0 && !isRight)
				{
					isRight = true;
					isLeft = false;
					auto FlipX = OrbitCamera::create(
						0.2f,
						1.0f, 0,
						0, 180.0f,
						0, 0);
					sprite->runAction(FlipX);
				}
				else if (transVector.x < 0 && !isLeft)
				{
					isRight = false;
					isLeft = true;
					auto FlipX = OrbitCamera::create(
						0.2f,
						1.0f, 0,
						180.0f, 180.0f,
						0, 0);
					sprite->runAction(FlipX);
				}
		
				body->ApplyLinearImpulse(b2Vec2(xSpeed * transVector.x / 3, ySpeed * transVector.y / 3), body->GetWorldCenter(), true);
			}
			else
			{
				if (isRight)
				{
					isRight = false;
					isLeft = true;
					auto FlipX = OrbitCamera::create(
						0.2f,
						1.0f, 0,
						180.0f, 180.0f,
						0, 0);
					
					sprite->runAction(FlipX);
				}

				body->ApplyLinearImpulse(b2Vec2(-xSpeed/3, 0), body->GetWorldCenter(), true);
			}
		}
		else
		{	
			// ��������ź �¾����� �̵�
			if (isPipe)
			{
				Vec2 transVector = pipe_positon - sprite->getPosition();
				transVector.normalize();
			
				if (transVector.x >= 0 && !isRight)
				{
					isRight = true;
					isLeft = false;
					auto FlipX = OrbitCamera::create(
						0.2f,
						1.0f, 0,
						0, 180.0f,
						0, 0);
				
					sprite->runAction(FlipX);
				}
				else if (transVector.x < 0 && !isLeft)
				{
					isRight = false;
					isLeft = true;
					auto FlipX = OrbitCamera::create(
						0.2f,
						1.0f, 0,
						180.0f, 180.0f,
						0, 0);

					sprite->runAction(FlipX);
				}

				body->ApplyLinearImpulse(b2Vec2(xSpeed * transVector.x, ySpeed * transVector.y), body->GetWorldCenter(), true);
			}
			// ��ҿ� �̵�
			else
			{
				//log("���");
				if (isRight)
				{
					isRight = false;
					isLeft = true;
					auto FlipX = OrbitCamera::create(
						0.2f,
						1.0f, 0,
						180.0f, 180.0f,
						0, 0);

					sprite->runAction(FlipX);
				}

				body->ApplyLinearImpulse(b2Vec2(-xSpeed, 0), body->GetWorldCenter(), true);
			}
		}
	}
	// �¾����� ����ȿ��
	else if (isHit && present_ani != HIT)
	{
		log("����");
		if(monsterType == 4)
		{
			sprite->stopActionByTag(700);
			sprite->stopActionByTag(800);
		}
		else {
			sprite->stopActionByTag(600);
			sprite->stopActionByTag(700);
			sprite->stopActionByTag(800);
		}
		auto tint = TintTo::create(0.1, Color3B::RED);
		//auto r_colo = colo->reverse();
		auto r_tint = TintTo::create(0.1, Color3B::WHITE);
		auto seq = Sequence::create(tint, r_tint,
			CallFunc::create(CC_CALLBACK_0(Monster::finish_hit,this)),nullptr);

		auto rep2 = Repeat::create(seq,2.0f);
		rep2->setTag(800);
		sprite->runAction(rep2);
		present_ani = HIT;
	}
}

void Monster::attackFinish()
{
	PlayerInfoSingleTon::getInstance()->hp = PlayerInfoSingleTon::getInstance()->hp - damage;
	log("�¾Ҵ�");
	log("�÷��̾� hp : %d", PlayerInfoSingleTon::getInstance()->hp);

	if (monsterType == 4)
	{
		auto exp = Sprite::create("monster/boss_attack_bomb.png");
		exp->setTextureRect(Rect(104, 644, 104, 104));
		Vec2 position = sprite->getPosition();
		exp->setPosition(position.x-60,position.y - 10);
		exp->setScale(2.7f);
		gameLayer->addChild(exp, 1200);

		auto explosion1 = ResouceLoad::getInstance()->boss_bombAnimate->clone();
		auto rep = Sequence::create(explosion1,
			CallFunc::create(CC_CALLBACK_0(Monster::remove_anim, this, exp)), nullptr);
		exp->runAction(rep);
	}
}
void Monster::finish_hit()
{
	present_ani = 0;
	log("�ǰݳ���");
}
void Monster::remove_anim(Node* sender)
{
	// ���ӷ��̾� ������
	auto sprite = (Sprite*)sender;
	if (sprite != nullptr) {
		DataSingleTon::getInstance()->remove_anim(sprite);
	}
}
