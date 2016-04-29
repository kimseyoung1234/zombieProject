#include "HelloWorldScene.h"
#include "DataSingleTon.h"
#include <algorithm>
USING_NS_CC;

// 몬스터 Y축 값에 따른 벡터 정렬
bool comp(const Monster *a, const Monster *b)
{
	float a_y, b_y;
	a_y = (float)a->body->GetPosition().y;
	b_y = (float)b->body->GetPosition().y;
	return a_y < b_y;
}

Scene* HelloWorld::createScene()
{
	auto scene = Scene::create();

	auto layer = HelloWorld::create();

	scene->addChild(layer);

	return scene;
}

bool HelloWorld::init()
{
	
	if (!Layer::init())
	{
		return false;
	}
	////////////////////////////////////
	//공용변수들 가져오기
	gameLayer = DataSingleTon::getInstance()->getGameLayer();
	_world = DataSingleTon::getInstance()->get_world();
	monsters = DataSingleTon::getInstance()->getMonsters();
	removeBullets = DataSingleTon::getInstance()->getRemoveBullets();
	winSize = Director::getInstance()->getWinSize();
	
	// 게임레이어 추가
	this->addChild(gameLayer, 4);
	
	//배경
	auto background = Sprite::create("background.png");
	background->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	this->addChild(background);


	//월드 생성
	if (this->createBox2dWorld(false))
	{
		this->schedule(schedule_selector(HelloWorld::tick));
	}
	player = Sprite::create("turret.png");
	player->setPosition(Vec2(player->getContentSize().width / 2 + 80,
		winSize.height / 2));
	gameLayer->addChild(player);

	for (int i = 0; i < 20; i++) {
	    int rand = random(50, 700);
		int r_monsterType = random(1, 3);
		Monster * mon = new Monster(Vec2(1200, rand),r_monsterType);
		gameLayer->addChild(mon);
		monsters->push_back(mon);
	}
	

	return true;
}

void HelloWorld::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	Layer::draw(renderer, transform, flags);

	_customCommand.init(_globalZOrder, transform, flags);
	_customCommand.func = CC_CALLBACK_0(HelloWorld::onDraw, this, transform, flags);
	renderer->addCommand(&_customCommand);
}

void HelloWorld::onDraw(const Mat4 &transform, uint32_t flags)
{
	Director* director = Director::getInstance();
	CCASSERT(nullptr != director, "Director is null when seting matrix stack");
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);

	GL::enableVertexAttribs(cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION);
	_world->DrawDebugData();
	
	CHECK_GL_ERROR_DEBUG();

	director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

bool HelloWorld::createBox2dWorld(bool debug)
{
	//월드 생성 시작----------------------------------------------------------

	//휴식 상태일 때 포함된 바디들을멈추게(sleep)할 것인지 결정한다
	_world->SetAllowSleeping(true);

	//지속적인 물리작용을 할 것인지를 결정한다 : 테스트
	_world->SetContinuousPhysics(true);

	//디버그 드로잉 설정
	if (debug)
	{
		//적색 : 현재 물리 운동을 하는 것
		//회색 : 현재 물리 운동이 없는 것
		m_debugDraw = new GLESDebugDraw(PTM_RATIO);
		_world->SetDebugDraw(m_debugDraw);

		uint32 flags = 0;
		flags += b2Draw::e_shapeBit;
		//flags += b2Draw::e_jointBit;
		//flags += b2Draw::e_aabbBit;
		//flags += b2Draw::e_pairBit;
		//flags += b2Draw::e_centerOfMassBit;
		m_debugDraw->SetFlags(flags);
	}

	// -----가장자리(테두리)를 지정해 공간(Ground Box)를 만든다----

	auto sprite = Sprite::create("white-512x512.png");
	sprite->setTextureRect(Rect(0, 0, 0, 0));
	sprite->setTag(WORLD);
	gameLayer->addChild(sprite);

	//바디데프에 좌표를 설정한다
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0, 0);
	groundBodyDef.userData = sprite;
	//월드에 바디데프의 정보(좌표)로 바디를 만ㄷ느다.
	auto groundBody = _world->CreateBody(&groundBodyDef);

	//가장자리(테두리) 경계선을 그릴 수 있는 모양의 객체를 만든다
	b2EdgeShape groundEdge;
	b2FixtureDef boxShapeDef;
	boxShapeDef.shape = &groundEdge;

	//에지 모양의 객체에 Set(점1,점2)로 선을 만든다
	//그리고 바디(groundBody)에 모양(groundEdge)을 고정시킨다.

	//아래
	groundEdge.Set(b2Vec2(0, 0), b2Vec2(winSize.width / PTM_RATIO, 0));
	groundBody->CreateFixture(&boxShapeDef);

	//왼쪽
	groundEdge.Set(b2Vec2(0, 0), b2Vec2(0, winSize.width / PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);

	//위쪽
	groundEdge.Set(b2Vec2(0, winSize.height / PTM_RATIO), b2Vec2(winSize.width / PTM_RATIO, winSize.height / PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);

	//오른쪽
	groundEdge.Set(b2Vec2(winSize.width / PTM_RATIO, winSize.height / PTM_RATIO), b2Vec2(winSize.width / PTM_RATIO, 0));
	groundBody->CreateFixture(&boxShapeDef);

	//월드생성 끝 -------------------------------------


	myContactListener = new ContactListener();
	_world->SetContactListener((b2ContactListener *)myContactListener);
	return true;

}
HelloWorld::~HelloWorld()
{
	//월드를 c++의 new로 생성했으므로 여기서 지워준다
	delete _world;
	_world = nullptr;
}
 
void HelloWorld::onEnter()
{
	Layer::onEnter();

	//싱글 터치 모드로 터치 리스너 등록
	auto listener = EventListenerTouchOneByOne::create();
	//Swallow touches only available in OneByOne mode.
	//핸들링된 터치 이벤트를 터치 이벤트 array에서 지우겠다는 의미다.
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);

	// The prioriry of the touch listener is based on the draw order of sprite
	// 터치 리스너의 우선순위를 (노드가) 화면에 그려진 순서대로 한다
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}
void HelloWorld::onExit()
{
	_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);

	Layer::onExit();
}



void HelloWorld::tick(float dt)
{
	int velocityIterations = 8;
	int positionIterations = 3;

	_world->Step(dt, velocityIterations, positionIterations);

	// Z_order 재설정하기 위해 몬스터 Y축 기준으로 벡터의 오름차순 정렬
	sort(monsters->begin(), monsters->end(),comp);

	// 몬스터 Y축 값에 따른 Z_order 재설정
	for (int i = 0; i < monsters->size(); i++)
	{
		auto mon = (Monster*)monsters->at(i);
		auto sprite = (Sprite*)mon->body->GetUserData();
		sprite->setZOrder(i + 100);
		mon->hpBar->setZOrder(i + 100);
	}

	// 오브젝트 제거
	removeObject();
	
	//모든 물리 객체들은 링크드 리스트에 저장되어 참조해 볼 수 있게 구현돼 있다.
	//만들어진 객체만큼 루프를 돌리면서 바디에 붙인 스프라이트를 여기서 제어한다
	for (b2Body *b = _world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetUserData() != nullptr)
		{
			
			Sprite* spriteData = (Sprite *)b->GetUserData();
			spriteData->setPosition(Vec2(b->GetPosition().x * PTM_RATIO,
				b->GetPosition().y *PTM_RATIO));
			spriteData->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}
	}

	// 터치 누르고 있을 시 자동 공격
	attackDelayTime = attackDelayTime + dt;
	if (isAttack) {
		Vec2 nPos1 = Vec2(player->getContentSize().width, player->getContentSize().height / 2);
		Vec2 nPos2 = player->convertToWorldSpace(nPos1);
		if (attackDelayTime >= 0.2) {
			attackDelayTime = 0;
			body = this->addNewSprite(nPos2, Size(9, 9), b2_dynamicBody, 1);
			body->SetLinearVelocity(b2Vec2(attackVector.x * 20, attackVector.y * 20));
		}
	}
}

// 총알이나 몬스터 제거
void HelloWorld::removeObject()
{
	//log("불렛 : %d", removeBullets->size());

	// 총알 제거
	for (int k = removeBullets->size() - 1; k >= 0; k--)
	{
		b2Body * r_body = (b2Body*)removeBullets->at(k);
		auto sprite = (Sprite*)r_body->GetUserData();
		if (sprite != nullptr) {
			gameLayer->removeChild(sprite);
			removeBullets->erase(removeBullets->begin() + k);
			_world->DestroyBody(r_body);
		}
	}

	// 몬스터 HP 체크 후 0이하이면 제거
	for (int i = monsters->size() - 1; i >= 0; i--)
	{
		Monster * mon = (Monster*)monsters->at(i);
		if (mon->hp <= 0)
		{
			auto sprite = (Sprite *)mon->body->GetUserData();
			if (sprite != nullptr) {
				gameLayer->removeChild(sprite);
				gameLayer->removeChild(mon);
				monsters->erase(monsters->begin() + i);
				_world->DestroyBody(mon->body);
				delete mon;
			}
		}
	}
}


// 총알 생성
b2Body* HelloWorld::addNewSprite(Vec2 point, Size size, b2BodyType bodytype, int type)
{
	//바디데프를 만들고 속성들을 지정한다.
	b2BodyDef bodyDef;

	auto sprite = Sprite::create("mole_1.png");
	sprite->setTag(BULLET);
	sprite->setScale(0.1f);
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

	if(type == 0)
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
	fixtureDef.density = 5.0f;
	// 마찰력 - 0 ~ 1
	fixtureDef.friction = 0.0f;
	//반발력 - 물체가 다른 물체에 닿았을 때 튕기는 값
	fixtureDef.restitution = 0.0;

	body->CreateFixture(&fixtureDef);

	return body;
}

bool HelloWorld::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	auto touchPoint = touch->getLocation();

	// 플레이어 기준으로 터치지점 방향벡터 구하기
	Vec2 shootVector = touchPoint - player->getPosition();
	Vec2 nPos1 = Vec2(player->getContentSize().width, player->getContentSize().height / 2);
	Vec2 nPos2 = player->convertToWorldSpace(nPos1);
	shootVector.normalize();

	attackVector = b2Vec2(shootVector.x, shootVector.y);

	// 누르고 공격가능 하면 총알 생성
	if (attackDelayTime >= 0.2) {
		isAttack = true;
		attackDelayTime = 0;
		body = this->addNewSprite(nPos2, Size(9, 9), b2_dynamicBody, 1);
		body->SetLinearVelocity(b2Vec2(shootVector.x * 20, shootVector.y * 20));
	}
	return true;
}


void HelloWorld::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	auto touchPoint = touch->getLocation();

	Vec2 shootVector = touchPoint - player->getPosition();
	shootVector.normalize();
	attackVector = b2Vec2(shootVector.x, shootVector.y);

	if (attackDelayTime >= 0.2)
	{
		isAttack = true;
	}
}

// 손가락을 화면에서 떼는 순간 호출된다
void HelloWorld::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	isAttack = false;
}



