#include "HelloWorldScene.h"
#include "ShopScene.h"
#include "DataSingleTon.h"
#include "MonsterInfoSingleTon.h"
#include "PlayerInfoSingleTon.h"
#include <algorithm>
#include "MyQueryCallback.h"

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
	menuLayer = DataSingleTon::getInstance()->getMenuLayer();
	_world = DataSingleTon::getInstance()->get_world();
	monsters = DataSingleTon::getInstance()->getMonsters();
	bullets = DataSingleTon::getInstance()->getBullets();
	winSize = Director::getInstance()->getWinSize();
	barricade = DataSingleTon::getInstance()->getBarricade();
	traps = DataSingleTon::getInstance()->getTraps();
	
//	shopLayer = DataSingleTon::getInstance()->getShopLayer();

	// 게임레이어 추가
	this->addChild(gameLayer, 4);
	this->addChild(menuLayer, 5);
	//this->addChild(shopLayer, 6);
	//배경
	auto background = Sprite::create("background.png");
	background->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	this->addChild(background);

	// 사용자 UI 추가
	addMenu();

	
	//실험용 도우미



	///////////////////////
	//월드 생성
	if (this->createBox2dWorld(true))
	{
		this->schedule(schedule_selector(HelloWorld::tick));
	}
	// 플레이어 생성
	
	auto sprite = Sprite::create("player_idle.png");
	auto texture = sprite->getTexture();
	auto animation = Animation::create();
	animation->setDelayPerUnit(0.1f);

	for (int i = 0; i < 6; i++)
	{
		int column = i % 4;
		int row = i / 4;
		animation->addSpriteFrameWithTexture(texture, Rect(column * 136, row * 72, 136, 72));
	}
	player = Sprite::create("player_idle.png", Rect(0, 0, 136, 72));
	player->setScale(1.5f);
	player->setAnchorPoint(Vec2(0, 0));
	player->setPosition(Vec2(player->getContentSize().width / 2 + 30,
		winSize.height / 2 + 43));
	gameLayer->addChild(player);

	auto animate = Animate::create(animation);
	auto rep = RepeatForever::create(animate);
	player->runAction(rep);
	return true;
}

// 웨이브 시작 (나중에 조정)
void HelloWorld::waveStart(Ref* pSender)
{
	if (isWave == false) {
		int maxMonster = MonsterInfoSingleTon::getInstance()->maxMonster;
		for (int i = 0; i < maxMonster; i++) {
			int rand = random(50, 600);
			int r_monsterType = random(1, 3);
			Monster * mon = new Monster(Vec2(1200, rand), 1);
			gameLayer->addChild(mon);
			monsters->push_back(mon);
		}
	}
	isWave = true;
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
	groundEdge.Set(b2Vec2(0, 1), b2Vec2(winSize.width / PTM_RATIO, 1));
	groundBody->CreateFixture(&boxShapeDef);

	//왼쪽
	groundEdge.Set(b2Vec2(0, 0), b2Vec2(0, winSize.width / PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);

	//위쪽
	groundEdge.Set(b2Vec2(0, (winSize.height-100) / PTM_RATIO), b2Vec2(winSize.width / PTM_RATIO, (winSize.height-100) / PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);

	//오른쪽
	groundEdge.Set(b2Vec2(winSize.width / PTM_RATIO, winSize.height / PTM_RATIO), b2Vec2(winSize.width / PTM_RATIO, 0));
	groundBody->CreateFixture(&boxShapeDef);
	

	//월드생성 끝 -------------------------------------

	// 바리게이트 

	auto b_sprite = Sprite::create("white-512x512.png");
	b_sprite->setTextureRect(Rect(0, 0, 0, 0));
	b_sprite->setTag(BARRICADE);
	gameLayer->addChild(b_sprite);

	b2BodyDef barricadeBodyDef;
	barricadeBodyDef.position.Set((winSize.width/2- 360) /PTM_RATIO, winSize.height / 2 / PTM_RATIO);
	barricadeBodyDef.type = b2_staticBody;
	barricadeBodyDef.userData = b_sprite;

	auto _barricade = _world->CreateBody(&barricadeBodyDef);

	b2FixtureDef BoxShapeDef;
	b2PolygonShape barricadeBox;
	BoxShapeDef.shape = &barricadeBox;
	BoxShapeDef.filter.groupIndex = -10;

	barricadeBox.SetAsBox(100 / PTM_RATIO, winSize.height / PTM_RATIO);

	_barricade->CreateFixture(&BoxShapeDef);

	barricade->push_back(_barricade);

	myContactListener = new ContactListener();
	_world->SetContactListener((b2ContactListener *)myContactListener);
	return true;
}

void HelloWorld::tick(float dt)
{
	if (!isgameOver) {
		//게임오버 체크
		if (PlayerInfoSingleTon::getInstance()->hp <= 0)
		{
			PlayerInfoSingleTon::getInstance()->hp = 0;
			gameOver();
		}
		// 현재 장착된 무기 공격속도 적용
		int equipWeapon = PlayerInfoSingleTon::getInstance()->weaponSeleted;
		if (equipWeapon == 0)
		{
			attackRate = PlayerInfoSingleTon::getInstance()->plstol_Rate;
		}
		else if (equipWeapon == 1)
		{
			attackRate = PlayerInfoSingleTon::getInstance()->ak_Rate;
		}
		// 웨이브 진행상황 갱신
		waveProgress->setScaleX((float)(monsters->size() / (float)MonsterInfoSingleTon::getInstance()->maxMonster));

		// 플레이어 HP바 갱신
		playerHpBar->setScaleX(PlayerInfoSingleTon::getInstance()->hp / 100.0f);

		int velocityIterations = 8;
		int positionIterations = 3;

		_world->Step(dt, velocityIterations, positionIterations);

		// Z_order 재설정하기 위해 몬스터 Y축 기준으로 벡터의 오름차순 정렬
		sort(monsters->begin(), monsters->end(), comp);

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
			Vec2 nPos1 = Vec2(player->getContentSize().width - 50, player->getContentSize().height / 2 - 10);
			Vec2 nPos2 = player->convertToWorldSpace(nPos1);
			if (attackDelayTime >= attackRate) {
				attackDelayTime = 0;
				Bullet * bullet = new Bullet(nPos2, 1);
				bullets->push_back(bullet);
				bullet->body->SetLinearVelocity(b2Vec2(attackVector.x * 30, attackVector.y * 30));
			}
		}

		// 스테이지 클리어 체크
		if (monsters->size() == 0 && isWave == true)
		{
			log("클리어!");
			MonsterInfoSingleTon::getInstance()->level_up();
			PlayerInfoSingleTon::getInstance()->hp = 100;
			levelLabel->setString((String::createWithFormat("Level : %d", MonsterInfoSingleTon::getInstance()->level)->getCString()));
			isWave = false;
		}
	}
}

// 총알이나 몬스터 제거
void HelloWorld::removeObject()
{
	// 총알 제거
	for (int k = bullets->size() - 1; k >= 0; k--)
	{
		Bullet * bullet = (Bullet *)bullets->at(k);
		if (bullet->isRemove == true) {
			auto sprite = (Sprite*)bullet->body->GetUserData();
			if (sprite != nullptr) {
				gameLayer->removeChild(sprite);
				gameLayer->removeChild(bullet);
				bullets->erase(bullets->begin() + k);
				_world->DestroyBody(bullet->body);
				delete bullet;
			}
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


void HelloWorld::trigger(Vec2 position)
{
	log("x : %f  y : %f", position.x, position.y);
	MyQueryCallback queryCallback; //see "World querying topic"
	b2AABB aabb;
	// center : 폭탄 중심 위치
	b2Vec2 center = b2Vec2(position.x / PTM_RATIO, position.y / PTM_RATIO);
	// 폭발 범위
	float blastRadius = 5.0;
	// 폭발 바운딩박스 위치와 크기 
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
				monsters->at(k)->hp = monsters->at(k)->hp - 50;
				monsters->at(k)->hpBar->setVisible(true);
				monsters->at(k)->hpBarShowTime = 0;
				applyBlastImpulse(body, center, bodyCom, 100);
				break;
			}
		}
	}
}

void HelloWorld::applyBlastImpulse(b2Body* body, b2Vec2 blastCenter, b2Vec2 applyPoint, float blastPower)
{
	b2Vec2 blastDir = applyPoint - blastCenter;
	float distance = blastDir.Normalize();
	//ignore bodies exactly at the blast point - blast direction is undefined
	if (distance == 0)
		return;
	float invDistance = 1 / distance;
	float impulseMag = blastPower * invDistance * invDistance;
	body->ApplyLinearImpulse(impulseMag * blastDir, applyPoint, true);
}

bool HelloWorld::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	auto touchPoint = touch->getLocation();
	 
	// 웨이브 도중 트랩 누르면 폭발
	if (isWave) {
		for (int i = traps->size() - 1; i >= 0; i--)
		{
			auto trap = (Trap*)traps->at(i);
			// 트랩이 눌렸다면
			if (trap->sprite->getBoundingBox().containsPoint(touchPoint))
			{
				trigger(trap->sprite->getPosition());
				// 효과 적용 후 삭제
				gameLayer->removeChild(trap->sprite);
				gameLayer->removeChild(trap);
				traps->erase(traps->begin() + i);
				return true;
			}
		}
	}
	// 준비시간때 트랩 드래그앤 드롭으로 옮기기 위해 selectedTrap값 갱신
	else
	{
		for (int i = traps->size() - 1; i >= 0; i--)
		{
			auto trap = (Trap*)traps->at(i);
			// 트랩이 눌렸다면
			if (trap->sprite->getBoundingBox().containsPoint(touchPoint))
			{
				selectedTrap = trap->sprite;
				isSelectedTrap = true;
				return true;
			}
		}
	}

	// 스킬 실험
	if (skill->getBoundingBox().containsPoint(touchPoint))
	{
		auto target = static_cast<Sprite*>(skill->getChildByTag(50));
		target->setVisible(true);
		target->setPosition(skill->convertToNodeSpace(touchPoint));
		isSkill = true;
		return true;
	}

	// 누르고 공격가능 하면 총알 생성
	if (attackDelayTime >= attackRate) {

		// 플레이어 기준으로 터치지점 방향벡터 구하기

		Vec2 nPos1 = Vec2(player->getContentSize().width - 50, player->getContentSize().height / 2 - 10);
		Vec2 nPos2 = player->convertToWorldSpace(nPos1);
		Vec2 shootVector = touchPoint - nPos2;
		shootVector.normalize();

		attackVector = b2Vec2(shootVector.x, shootVector.y);


		isAttack = true;
		attackDelayTime = 0;
		Bullet * bullet = new Bullet(nPos2, 1);
		bullets->push_back(bullet);
		bullet->body->SetLinearVelocity(b2Vec2(shootVector.x * 30, shootVector.y * 30));


		// 공격 애니메이션 
		player->stopAllActions();
		auto sprite = Sprite::create("player_attack.png");
		auto texture = sprite->getTexture();
		auto animation = Animation::create();
		animation->setDelayPerUnit(0.05f);

		for (int i = 0; i < 8; i++)
		{
			int column = i % 4;
			int row = i / 4;
			animation->addSpriteFrameWithTexture(texture, Rect(column * 136, row * 72, 136, 72));
		}


		auto animate = Animate::create(animation);
		auto rep = RepeatForever::create(animate);
		player->runAction(rep);
	}

	return true;
}

void HelloWorld::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	auto touchPoint = touch->getLocation();

	// 트랩 옮기는 중이라면
	if (isSelectedTrap) {
		
		auto target = static_cast<Sprite*>(selectedTrap);
		target->setPosition(target->getPosition() + touch->getDelta());
	}
	else if (isSkill)
	{
		auto target = static_cast<Sprite*>(skill->getChildByTag(50));
		target->setPosition(target->getPosition() + touch->getDelta());
	}
	// 아니라면 공격
	else {
		Vec2 nPos1 = Vec2(player->getContentSize().width - 50, player->getContentSize().height / 2 - 10);
		Vec2 nPos2 = player->convertToWorldSpace(nPos1);
		Vec2 shootVector = touchPoint - nPos2;
		shootVector.normalize();
		attackVector = b2Vec2(shootVector.x, shootVector.y);

		if (attackDelayTime >= 0.2)
		{
			isAttack = true;
		}
	}
}

void HelloWorld::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	player->stopAllActions();
	auto sprite = Sprite::create("player_idle.png");
	auto texture = sprite->getTexture();
	auto animation = Animation::create();
	animation->setDelayPerUnit(0.1f);

	// Idle 애니메이션 다시 시작
	for (int i = 0; i < 6; i++)
	{
		int column = i % 4;
		int row = i / 4;
		animation->addSpriteFrameWithTexture(texture, Rect(column * 136, row * 72, 136, 72));
	}

	auto animate = Animate::create(animation);
	auto rep = RepeatForever::create(animate);
	player->runAction(rep);
	
	selectedTrap = nullptr;
	isSelectedTrap = false;
	isAttack = false;

	// 스킬실험
	if (isSkill) {
		isSkill = false;
		auto target = static_cast<Sprite*>(skill->getChildByTag(50));
		target->setVisible(false);
		Size parentSize;
		parentSize = skill->getContentSize();
		Vec2 w_position = skill->convertToWorldSpace(target->getPosition());
		trigger(w_position);
		target->setPosition(Vec2(parentSize.width / 2.0, parentSize.height / 2.0));

		//폭탄애니메이션 실험
		auto cache = SpriteFrameCache::getInstance();
		cache->addSpriteFramesWithFile("explosion/explosion.plist");

		Vector<SpriteFrame*> expFrames;

		char str[100] = { 0 };
		for (int i = 1; i < 90; i++)
		{
			sprintf(str, "explosion_100%02d.png", i + 1);
			SpriteFrame* frame = cache->getSpriteFrameByName(str);
			expFrames.pushBack(frame);
		}
		auto exp = Sprite::createWithSpriteFrameName("explosion_10002.png");
		exp->setPosition(w_position);
		exp->setScale(2.8f);
		gameLayer->addChild(exp,200);

		auto animation = Animation::createWithSpriteFrames(expFrames, 0.02f);
		auto animate = Animate::create(animation);
		auto rep = Sequence::create(animate,
			CallFunc::create(CC_CALLBACK_0(HelloWorld::remove_anim, this,exp)),nullptr);
		exp->runAction(rep);
	}
}

void HelloWorld::remove_anim(Node* sender)
{
	auto sprite = (Sprite*)sender;
	gameLayer->removeChild(sprite);
}


void HelloWorld::gameOver()
{
	//isgameOver = true;
	log("게임 오버!");
}
void HelloWorld::addMenu()
{
	// 시작 버튼

	auto pMenuItem = MenuItemImage::create(
		"btn-play-normal.png",
		"btn-play-normal.png",
		CC_CALLBACK_1(HelloWorld::waveStart, this));

	auto pMenu = Menu::create(pMenuItem, nullptr);
	pMenu->setPosition(Vec2(winSize.width - 100, winSize.height - 50));
	menuLayer->addChild(pMenu);

	// 현재 레벨

	levelLabel = Label::create("Level : 1", "Arial", 34);
	levelLabel->setPosition(Vec2(100, winSize.height - 50));
	levelLabel->setColor(Color3B::RED);
	menuLayer->addChild(levelLabel);

	// 웨이브 진행 상황

	waveProgress = Sprite::create("white-512x512.png");
	waveProgress->setTextureRect(Rect(0, 0, 300, 10));
	waveProgress->setColor(Color3B::BLUE);
	waveProgress->setAnchorPoint(Vec2(0, 0.5));
	waveProgress->setPosition(Vec2(winSize.width / 2 - 150, winSize.height - 50));
	menuLayer->addChild(waveProgress);

	// 플레이어 HP

	playerHpBar = Sprite::create("white-512x512.png");
	playerHpBar->setTextureRect(Rect(0, 0, 200, 10));
	playerHpBar->setColor(Color3B::RED);
	playerHpBar->setAnchorPoint(Vec2(0, 0.5));
	playerHpBar->setPosition(Vec2(200, winSize.height - 50));
	menuLayer->addChild(playerHpBar);

	// 상점 메뉴
	auto shop = MenuItemFont::create(
		"상점",
		CC_CALLBACK_1(HelloWorld::shopOpen, this));
	shop->setColor(Color3B::RED);
	auto shopMenu = Menu::create(shop, nullptr);

	shopMenu->setPosition(Vec2(winSize.width - 250, winSize.height - 50));
	menuLayer->addChild(shopMenu);

	//스킬창
	skill = Sprite::create("skill1.png");
	skill->setPosition(Vec2(100, 100));
	
	gameLayer->addChild(skill);

	auto bomb = Sprite::create("bomb.png");
	Size parentSize;
	parentSize = skill->getContentSize();
	bomb->setPosition(Vec2(parentSize.width / 2.0, parentSize.height /2.0));
	bomb->setScale(3.0f);
	skill->addChild(bomb);

	// 스킬
	auto range = Sprite::create("range.png");
	range->setPosition(Vec2(parentSize.width / 2.0, parentSize.height / 2.0));
	
	range->setTag(50);
	range->setVisible(false);
	skill->addChild(range);

}

void HelloWorld::shopOpen(Ref * pSender)
{
	if (isWave == false) {
		auto pScene = ShopScene::createScene();
		Director::getInstance()->pushScene(pScene);
	}
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
	//_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);

	Layer::onExit();
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