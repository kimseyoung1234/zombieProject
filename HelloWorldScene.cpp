#include "HelloWorldScene.h"
#include "ShopScene.h"
#include "DataSingleTon.h"
#include "MonsterInfoSingleTon.h"
#include "PlayerInfoSingleTon.h"
#include <algorithm>
#include "MyQueryCallback.h"
#include "ResouceLoad.h"
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
	helpers = DataSingleTon::getInstance()->getHelpers();

	
	ResouceLoad::getInstance();

	// 게임레이어 추가
	this->addChild(gameLayer, 4);
	this->addChild(menuLayer, 5);
	//배경
	auto background = Sprite::create("ui/background.png");
	background->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	background->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(background);

	auto background_down = Sprite::create("ui/background_down.png");
	background_down->setPosition(Vec2(winSize.width / 2, 53));
	gameLayer->addChild(background_down,2000);


	 bari = Sprite::create("ui/barricade.png");
	bari->setPosition(Vec2(200, winSize.height / 2+ 100));
	gameLayer->addChild(bari,1000);

	 bari2 = Sprite::create("ui/barricade.png");
	bari2->setPosition(Vec2(280, winSize.height / 2 - 45));
	gameLayer->addChild(bari2, 1000);

	 bari3 = Sprite::create("ui/barricade.png");
	bari3->setPosition(Vec2(370, winSize.height / 2 - 200));
	gameLayer->addChild(bari3, 1000);

	// 사용자 UI 추가
	addMenu();

	///////////////////////
	//월드 생성
	if (this->createBox2dWorld(true))
	{
		this->schedule(schedule_selector(HelloWorld::tick));
	}
	// 플레이어 생성
	

	player = Sprite::create("player/player_idle.png", Rect(0, 0, 136, 72));
	player->setScale(1.5f);
	player->setAnchorPoint(Vec2(0, 0));
	player->setPosition(Vec2(player->getContentSize().width / 2 - 20,
		winSize.height / 2- 70));
	gameLayer->addChild(player,1500);

	auto player_idle = ResouceLoad::getInstance()->player_idleAnimate->clone();
	auto rep = RepeatForever::create(player_idle);
	player->runAction(rep);

	return true;
}

// 웨이브 시작 (나중에 조정)
void HelloWorld::waveStart(Ref* pSender)
{
	// 시작전 실험
	if (!isWave && !isPlaySeleted) {
		isPlaySeleted = true;
		auto Level_str = String::createWithFormat("Level : %d", MonsterInfoSingleTon::getInstance()->level);

		auto Level = LabelBMFont::create(Level_str->getCString(), "fonts/futura-48.fnt");
		Level->setScale(0.0f);
		Level->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
		Level->setColor(Color3B::RED);
		gameLayer->addChild(Level, 2000);

		auto seq = Sequence::create(ScaleTo::create(0.5, 4.0f),
			DelayTime::create(0.5), ScaleTo::create(0.1, 20.0f),
			RemoveSelf::create(true), nullptr);
		Level->runAction(seq);

		auto start = LabelBMFont::create("Start", "fonts/futura-48.fnt");
		start->setScale(0.0f);
		start->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
		start->setColor(Color3B::RED);
		gameLayer->addChild(start, 2000);

		auto seq2 = Sequence::create(DelayTime::create(1.5f)
			, ScaleTo::create(0.3, 5.0f),
			DelayTime::create(0.5), ScaleTo::create(0.1, 20.0f),
			RemoveSelf::create(true),
			CallFunc::create(CC_CALLBACK_0(HelloWorld::monsterSpawn, this)), nullptr);
		start->runAction(seq2);

	}
}
void HelloWorld::monsterSpawn()
{
		int maxMonster = MonsterInfoSingleTon::getInstance()->maxMonster;
		for (int i = 0; i < maxMonster; i++) {
			int x_rand = random(1350, 1700);
			int y_rand = random(90, 520);
			int r_monsterType = random(1, 3);
			Monster * mon = new Monster(Vec2(500, y_rand), r_monsterType);
			gameLayer->addChild(mon);
			monsters->push_back(mon);
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
	groundBody = _world->CreateBody(&groundBodyDef);

	//가장자리(테두리) 경계선을 그릴 수 있는 모양의 객체를 만든다
	b2EdgeShape groundEdge;
	b2FixtureDef boxShapeDef;
	boxShapeDef.shape = &groundEdge;

	//에지 모양의 객체에 Set(점1,점2)로 선을 만든다
	//그리고 바디(groundBody)에 모양(groundEdge)을 고정시킨다.

	//아래
	groundEdge.Set(b2Vec2(0, 2.7), b2Vec2((winSize.width + 500) / PTM_RATIO, 2.7));
	groundBody->CreateFixture(&boxShapeDef);

	//왼쪽
	groundEdge.Set(b2Vec2(0, 0), b2Vec2(0, winSize.width / PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);

	//위쪽
	groundEdge.Set(b2Vec2(0, (winSize.height-195) / PTM_RATIO), b2Vec2((winSize.width + 500) / PTM_RATIO, (winSize.height-195) / PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);

	//오른쪽
	groundEdge.Set(b2Vec2( (winSize.width + 500) / PTM_RATIO, winSize.height / PTM_RATIO), b2Vec2((winSize.width + 500) / PTM_RATIO, 0));
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

	_barricade = _world->CreateBody(&barricadeBodyDef);

	b2EdgeShape barricade_Edge;
	b2FixtureDef bari_ShapeDef;

	bari_ShapeDef.shape = &barricade_Edge;
	bari_ShapeDef.filter.categoryBits = 0x0004;
	bari_ShapeDef.filter.maskBits = 0x0002 ;

	barricade_Edge.Set(b2Vec2(-3.0,5), b2Vec2(5.5,-10));

	_barricade->CreateFixture(&bari_ShapeDef);

	barricade->push_back(_barricade);

	myContactListener = new ContactListener();

	_world->SetContactListener((b2ContactListener *)myContactListener);
	return true;
}

void HelloWorld::tick(float dt)
{
	int velocityIterations = 8;
	int positionIterations = 3;

	_world->Step(dt, velocityIterations, positionIterations);

	//모든 물리 객체들은 링크드 리스트에 저장되어 참조해 볼 수 있게 구현돼 있다.
	//만들어진 객체만큼 루프를 돌리면서 바디에 붙인 스프라이트를 여기서 제어한다
	for (b2Body *b = _world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetUserData() != nullptr)
		{
			Sprite* spriteData = (Sprite *)b->GetUserData();
			spriteData->setPosition(Vec2(b->GetPosition().x * PTM_RATIO,
				b->GetPosition().y *PTM_RATIO));
			spriteData->setRotation(spriteData->getRotation());
		}
	}

	if (!isgameOver) {
		
		skill1DelayTime = skill1DelayTime + dt;
		skill2DelayTime = skill2DelayTime + dt;

		//현재 무기 갱신
		auto str = String::createWithFormat("item/gun%02d.png", PlayerInfoSingleTon::getInstance()->weaponSeleted+1);
		weapon->setTexture(str->getCString());

		//보유 아이템 수 갱신
		item_Label->setString((String::createWithFormat("%d", PlayerInfoSingleTon::getInstance()->have_trap1)->getCString()));
		item2_Label->setString((String::createWithFormat("%d", PlayerInfoSingleTon::getInstance()->have_trap2)->getCString()));

		//소지금 라벨 업데이트
		cocos2d::String *money_In_Hand;
		money_In_Hand = String	::createWithFormat("%d", PlayerInfoSingleTon::getInstance()->money_In_Hand);
		money_label->setString(money_In_Hand->getCString());

		//게임오버 체크
		if (PlayerInfoSingleTon::getInstance()->hp <= 0)
		{
			log("??");
			PlayerInfoSingleTon::getInstance()->hp = 0;
			gameOver();
		}
		// 현재 장착된 무기 공격속도 적용
		int equipWeapon = PlayerInfoSingleTon::getInstance()->weaponSeleted;
		if (equipWeapon == 0)
		{
			attackRate = PlayerInfoSingleTon::getInstance()->machine_Rate;
		}
		else if (equipWeapon == 1)
		{
			attackRate = PlayerInfoSingleTon::getInstance()->ak_Rate;
		}
		else if (equipWeapon == 2)
		{
			attackRate = PlayerInfoSingleTon::getInstance()->sniper_Rate;
		}
		// 웨이브 진행상황 갱신
		waveProgress->setScaleX((float)(monsters->size() / (float)MonsterInfoSingleTon::getInstance()->maxMonster));
		m_pro->setPositionX(winSize.width / 2 - 100 + (waveProgress->getContentSize().width * waveProgress->getScaleX()));
		// 플레이어 HP바 갱신
		playerHp->setScaleX(PlayerInfoSingleTon::getInstance()->hp / 100.0f);


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


		// 터치 누르고 있을 시 자동 공격
		attackDelayTime = attackDelayTime + dt;
	
		if (isAttack && !isResultLayer) {
			Vec2 nPos1 = Vec2(player->getContentSize().width - 50, player->getContentSize().height / 2 - 10);
			Vec2 nPos2 = player->convertToWorldSpace(nPos1);
			Vec2 shootVector = attackPoint - nPos2;


			// 각도 실험
			float shootAngle = shootVector.getAngle();
			cocosAngle = CC_RADIANS_TO_DEGREES(-1 * shootAngle);

			shootVector.normalize();

			if (attackDelayTime >= attackRate) {
				auto weapon_touchPoint = level_ui->convertToNodeSpace(attackPoint);
				if (weapon_ui->getBoundingBox().containsPoint(weapon_touchPoint))
				{}
				else
				{
					attackDelayTime = 0;
					int current_Weapon = PlayerInfoSingleTon::getInstance()->weaponSeleted;

					// 머신건
					if (current_Weapon == 0) {
						Bullet * bullet = new Bullet(nPos2, current_Weapon, cocosAngle);
						bullets->push_back(bullet);
						bullet->body->SetLinearVelocity(b2Vec2(shootVector.x * 30, shootVector.y * 30));
					}
					// 멀티공격
					else if (current_Weapon == 1)
					{
						float shootLength = shootVector.length();

						// 상탄
						// 원점에서 각도를 바꿔 일정 길이만큼 이동한 좌표값 구하기
						Vec2 shootVector2(shootLength * cosf(shootAngle + 0.15), shootLength * sinf(shootAngle + 0.15));
						shootVector2.normalize();
						float shootAngle2 = shootAngle + 0.15;
						float cocosAngle2 = CC_RADIANS_TO_DEGREES(-1 * shootAngle2);
						// 하탄
						Vec2 shootVector3(shootLength * cosf(shootAngle - 0.15), shootLength * sinf(shootAngle - 0.15));
						shootVector3.normalize();
						float shootAngle3 = shootAngle - 0.15;
						float cocosAngle3 = CC_RADIANS_TO_DEGREES(-1 * shootAngle3);

						// 총알 생성
						Bullet * bullet = new Bullet(nPos2, current_Weapon, cocosAngle);
						bullets->push_back(bullet);
						bullet->body->SetLinearVelocity(b2Vec2(shootVector.x * 30, shootVector.y * 30));

						Bullet * bullet2 = new Bullet(nPos2, current_Weapon, cocosAngle2);
						bullets->push_back(bullet2);
						bullet2->body->SetLinearVelocity(b2Vec2(shootVector2.x * 30, shootVector2.y * 30));

						Bullet * bullet3 = new Bullet(nPos2, current_Weapon, cocosAngle3);
						bullets->push_back(bullet3);
						bullet3->body->SetLinearVelocity(b2Vec2(shootVector3.x * 30, shootVector3.y * 30));
					}
					// 저격총
					else if (current_Weapon == 2)
					{
						Bullet * bullet = new Bullet(nPos2, current_Weapon, cocosAngle);
						bullets->push_back(bullet);
						bullet->body->SetLinearVelocity(b2Vec2(shootVector.x * 70, shootVector.y * 70));
						gameLayer->addChild(bullet);
					}
					player->stopAllActions();
					auto player_attack = ResouceLoad::getInstance()->player_attackAnimate->clone();
					auto seq = Sequence::create(player_attack,
						CallFunc::create(CC_CALLBACK_0(HelloWorld::re_Idle, this)), nullptr);
					player->runAction(seq);
				}
			}
		}

		// 스테이지 클리어 체크
		if (monsters->size() == 0 && isWave == true)
		{
			result();
			isPlaySeleted = false;
		}
	}
}
void HelloWorld::result()
{
	result_Layer->setVisible(true);

	// 게임 결과 배경
	result_background = Sprite::create("ui/ui_result.png");
	result_background->setPosition(winSize.width / 2, winSize.height / 2);
	result_Layer->addChild(result_background);
	result_background->setScale(0.0f);

	//결과창 애니메이션
	auto scale = ScaleTo::create(0.6f, 1.0f);
	auto rotate = RotateBy::create(0.3, 360);
	auto r_rep = Repeat::create(rotate, 2);
	auto spa = Spawn::create(scale, r_rep,nullptr);
	result_background->runAction(spa);


	auto complete = Sprite::create("ui/complete.png");
	complete->setPosition(Vec2(result_background->getContentSize().width / 2, result_background->getContentSize().height- 100));
	result_background->addChild(complete);

	auto okMenuItem = MenuItemImage::create(
		"ui/ok_btn.png",
		"ui/ok_btn_down.png",
		CC_CALLBACK_1(HelloWorld::resultClose, this));
	auto okMenu = Menu::create(okMenuItem, nullptr);
	okMenu->setPosition(Vec2(result_background->getContentSize().width / 2, 10));
	result_background->addChild(okMenu);

	// 터치 방지
	result_Layer->setVisible(true);
	isResultLayer = true;
	pMenu->setEnabled(false);
	shopMenu->setEnabled(false);

	auto result_gold = Sprite::create("item/gold.png");
	result_gold->setPosition(Vec2(100, result_background->getContentSize().height / 2 -75));
	result_gold->setScale(2.0f);
	result_background->addChild(result_gold);

	// 총 획득 골드

	auto gain_gold = String::createWithFormat(" + %d", take_gold);

	auto gain_gold_label = LabelBMFont::create(gain_gold->getCString(), "fonts/futura-48.fnt");
	gain_gold_label->setAnchorPoint(Vec2(0, 0.5));
	gain_gold_label->setPosition(Vec2(35, 20));
	gain_gold_label->setScale(0.7f);
	result_gold->addChild(gain_gold_label);

	// 죽인 몬스터 수
	auto kill_monster = Sprite::create("ui/zombie.png");
	kill_monster->setPosition(Vec2(100, result_background->getContentSize().height / 2 + 45));
	//kill_monster->setScale(1.5f);
	result_background->addChild(kill_monster);

	auto kill_monster_num = String::createWithFormat("   %d", MonsterInfoSingleTon::getInstance()->maxMonster);

	auto kill_monster_label = LabelBMFont::create(kill_monster_num->getCString(), "fonts/futura-48.fnt");
	kill_monster_label->setAnchorPoint(Vec2(0, 0.5));
	kill_monster_label->setPosition(Vec2(105, 50));
	kill_monster_label->setScale(1.35f);
	kill_monster_label->setColor(Color3B::RED);
	kill_monster->addChild(kill_monster_label);


	// 레벨업
	MonsterInfoSingleTon::getInstance()->level_up();
	PlayerInfoSingleTon::getInstance()->hp = 100;
	levelLabel->setString((String::createWithFormat("Level : %d", MonsterInfoSingleTon::getInstance()->level)->getCString()));
	isWave = false;


	// 클리어 하면 도우미 삭제
	for (int i = 0; i < helpers->size(); i++)
	{
		auto helper = (Helper*)helpers->at(i);
		gameLayer->removeChild(helper->sprite);
		gameLayer->removeChild(helper);
	}
	helpers->clear();
	PlayerInfoSingleTon::getInstance()->have_helper = 0;
}

// 총알이나 몬스터 제거
void HelloWorld::removeObject()
{
	// 총알 제거
	for (int k = bullets->size() - 1; k >= 0; k--)
	{
		Bullet * bullet = (Bullet *)bullets->at(k);
		// 총알이 화면넘어갔으면 삭제
		if (bullet->sprite->getPositionX() >= winSize.width)
		{
			bullet->isRemove = true;
		}

		if (bullet->isRemove == true) {
			// 저격총이면 삭제하기전에 이미 공격했던 몬스터의 hitBullet을 초기화
			if (bullet->bulletType == 2) {
				// 몬스터랑 비교해서
				for (int i = 0; i < monsters->size(); i++)
				{
					Monster * mon = (Monster*)monsters->at(i);
					if (mon->hitBullet == bullet->body)
					{
						mon->hitBullet = nullptr;
					}
				}
			}
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
				//몬스터가 가지고 있는 돈을 획득
				PlayerInfoSingleTon::getInstance()->money_In_Hand = PlayerInfoSingleTon::getInstance()->money_In_Hand + mon->reward;
				take_gold = mon->reward + take_gold;
				
				// 골드 획득 애니메이션
	
				auto kill_gold = String::createWithFormat(" + %d", mon->reward);

				auto kill_gold_label = LabelBMFont::create(kill_gold->getCString(), "fonts/futura-48.fnt");
				kill_gold_label->setAnchorPoint(Vec2(0, 0.5));
				kill_gold_label->setPosition(Vec2(220, 650));
				kill_gold_label->setScale(0.7f);
				menuLayer->addChild(kill_gold_label,1000);

				auto move = MoveBy::create(1.5, Vec2(0, 30));
				auto fade_seq = Sequence::create(DelayTime::create(0.7),
					FadeOut::create(0.8), nullptr);
				auto spawn = Spawn::create(move, fade_seq,nullptr);
				auto gold_seq = Sequence::create(spawn,
					CallFunc::create(CC_CALLBACK_0(HelloWorld::remove_label, this, kill_gold_label)), nullptr);
				kill_gold_label->runAction(gold_seq);

				//죽을때 애니메이션 

				auto dieAnimate = ResouceLoad::getInstance()->dieAnimate->clone();
				
				auto deadSprite = Sprite::create("monster/brain_move.png", Rect(0, 0, 104, 104));
				deadSprite->setPosition(sprite->getPosition());
				gameLayer->addChild(deadSprite,150);
				auto rep = Sequence::create(dieAnimate,
					CallFunc::create(CC_CALLBACK_0(HelloWorld::remove_anim, this, deadSprite)), nullptr);
				deadSprite->runAction(rep);

				// 제거
				gameLayer->removeChild(sprite);
				gameLayer->removeChild(mon);
				monsters->erase(monsters->begin() + i);
				_world->DestroyBody(mon->body);
				delete mon;
			}
		}
	}
}

// 폭파 밀림
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

	attackPoint = touchPoint;

	if (isResultLayer)
	{

	}
	else
	{
		if (!isgameOver) {
			// 웨이브 도중 트랩 누르면 폭발
			if (isWave) {
				for (int i = traps->size() - 1; i >= 0; i--)
				{
					auto trap = (Trap*)traps->at(i);
					// 트랩이 눌렸다면
					if (trap->sprite->getBoundingBox().containsPoint(touchPoint))
					{
						int type = trap->type;
						float blastRadius;

						//폭파 애니매이션
						if (type == 0)
						{
							blastRadius = PlayerInfoSingleTon::getInstance()->trap1_blastRadius;
							auto cache = SpriteFrameCache::getInstance();
							cache->addSpriteFramesWithFile("explosion/ExplosionPlist.plist");

							auto exp = Sprite::createWithSpriteFrameName("explosion_10002.png");
							exp->setPosition(trap->sprite->getPosition());
							exp->setScale(2.8f);
							gameLayer->addChild(exp, 1200);

							auto explosion1 = ResouceLoad::getInstance()->explosion1->clone();
							auto rep = Sequence::create(explosion1,
								CallFunc::create(CC_CALLBACK_0(HelloWorld::remove_anim, this, exp)), nullptr);
							exp->runAction(rep);
						}
						if (type == 1)
						{
							blastRadius = PlayerInfoSingleTon::getInstance()->trap2_blastRadius;

							auto cache = SpriteFrameCache::getInstance();
							cache->addSpriteFramesWithFile("explosion/Explosion2Plist.plist");

							auto exp = Sprite::createWithSpriteFrameName("explosion_11002.png");
							exp->setPosition(trap->sprite->getPosition());
							exp->setScale(2.8f);
							gameLayer->addChild(exp, 1200);

							auto explosion2 = ResouceLoad::getInstance()->explosion2->clone();
							auto rep = Sequence::create(explosion2,
								CallFunc::create(CC_CALLBACK_0(HelloWorld::remove_anim, this, exp)), nullptr);
							exp->runAction(rep);
						}
						myContactListener->trigger(trap->sprite->getPosition(), blastRadius, type, 100);
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

				if (item->getBoundingBox().containsPoint(touchPoint))
				{
					if (PlayerInfoSingleTon::getInstance()->have_trap1 > 0)
					{
						PlayerInfoSingleTon::getInstance()->have_trap1--;

						auto trap = new Trap(Vec2(touchPoint.x, touchPoint.y), 0);
						gameLayer->addChild(trap);
						traps->push_back(trap);

						selectedTrap = trap->sprite;
						isSelectedTrap = true;
					}
					return true;
				}
				if (item2->getBoundingBox().containsPoint(touchPoint))
				{
					if (PlayerInfoSingleTon::getInstance()->have_trap2 > 0) {
						PlayerInfoSingleTon::getInstance()->have_trap2--;

						auto trap = new Trap(Vec2(touchPoint.x, touchPoint.y), 1);
						gameLayer->addChild(trap);
						traps->push_back(trap);

						selectedTrap = trap->sprite;
						isSelectedTrap = true;
					}
					return true;
				}
			}

			// 스킬 실험
			if (skill->getBoundingBox().containsPoint(touchPoint))
			{
				if (!skill_cool) {
					auto target = static_cast<Sprite*>(skill->getChildByTag(50));
					target->setVisible(true);
					target->setPosition(skill->convertToNodeSpace(touchPoint));
					isSkill = true;
				}
				//	skill1DelayTime = 0;
				return true;
			}
			if (skill2->getBoundingBox().containsPoint(touchPoint))
			{
				if (!skill2_cool) {
					auto target = static_cast<Sprite*>(skill2->getChildByTag(51));
					target->setVisible(true);
					target->setPosition(skill2->convertToNodeSpace(touchPoint));
					isSkill2 = true;
					skill2DelayTime = 0;
				}
				return true;

			}

			// 무기눌렀을때 교체
			auto weapon_touchPoint = level_ui->convertToNodeSpace(touchPoint);

			if (weapon_ui->getBoundingBox().containsPoint(weapon_touchPoint))
			{
				PlayerInfoSingleTon::getInstance()->weaponSeleted = (PlayerInfoSingleTon::getInstance()->weaponSeleted + 1) % 3;
				auto str = String::createWithFormat("item/gun%02d.png", PlayerInfoSingleTon::getInstance()->weaponSeleted + 1);
				weapon->setTexture(str->getCString());
				return true;
			}



			// 누르고 공격가능 하면 총알 생성
			if (attackDelayTime >= attackRate && !isgameOver) {

				isAttack = true;
				// 플레이어 기준으로 터치지점 방향벡터 구하기

				Vec2 nPos1 = Vec2(player->getContentSize().width - 50, player->getContentSize().height / 2 - 10);
				Vec2 nPos2 = player->convertToWorldSpace(nPos1);
				Vec2 shootVector = attackPoint - nPos2;
				// 각도 실험
				float shootAngle = shootVector.getAngle();
				cocosAngle = CC_RADIANS_TO_DEGREES(-1 * shootAngle);

				shootVector.normalize();

				// 공격한번 했으면 딜레이 초기화
				attackDelayTime = 0;

				// 현재 장착된 무기에 따라 총알 생성
				int current_Weapon = PlayerInfoSingleTon::getInstance()->weaponSeleted;
				if (current_Weapon == 0) {
					Bullet * bullet = new Bullet(nPos2, current_Weapon, cocosAngle);
					bullets->push_back(bullet);
					bullet->body->SetLinearVelocity(b2Vec2(shootVector.x * 30, shootVector.y * 30));
				}
				else if (current_Weapon == 1)
				{

					float shootLength = shootVector.length();

					// 상탄
					// 원점에서 각도를 바꿔 일정 길이만큼 이동한 좌표값 구하기
					Vec2 shootVector2(shootLength * cosf(shootAngle + 0.15), shootLength * sinf(shootAngle + 0.15));
					shootVector2.normalize();
					float shootAngle2 = shootAngle + 0.15;
					float cocosAngle2 = CC_RADIANS_TO_DEGREES(-1 * shootAngle2);
					// 하탄
					Vec2 shootVector3(shootLength * cosf(shootAngle - 0.15), shootLength * sinf(shootAngle - 0.15));
					shootVector3.normalize();
					float shootAngle3 = shootAngle - 0.15;
					float cocosAngle3 = CC_RADIANS_TO_DEGREES(-1 * shootAngle3);

					// 총알 생성
					Bullet * bullet = new Bullet(nPos2, current_Weapon, cocosAngle);
					bullets->push_back(bullet);
					bullet->body->SetLinearVelocity(b2Vec2(shootVector.x * 30, shootVector.y * 30));

					Bullet * bullet2 = new Bullet(nPos2, current_Weapon, cocosAngle2);
					bullets->push_back(bullet2);
					bullet2->body->SetLinearVelocity(b2Vec2(shootVector2.x * 30, shootVector2.y * 30));

					Bullet * bullet3 = new Bullet(nPos2, current_Weapon, cocosAngle3);
					bullets->push_back(bullet3);
					bullet3->body->SetLinearVelocity(b2Vec2(shootVector3.x * 30, shootVector3.y * 30));
				}
				else if (current_Weapon == 2)
				{
					Bullet * bullet = new Bullet(nPos2, current_Weapon, cocosAngle);
					bullets->push_back(bullet);
					bullet->body->SetLinearVelocity(b2Vec2(shootVector.x * 70, shootVector.y * 70));
					gameLayer->addChild(bullet);
				}

				// 공격 애니메이션 
				player->stopAllActions();
				auto player_attack = ResouceLoad::getInstance()->player_attackAnimate->clone();
				auto seq = Sequence::create(player_attack,
					CallFunc::create(CC_CALLBACK_0(HelloWorld::re_Idle, this)), nullptr);
				player->runAction(seq);
			}
		}
	}

	return true;
}

void HelloWorld::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	auto touchPoint = touch->getLocation();

	// 트랩 옮기는 중이라면
	if (isSelectedTrap) {
		
		auto target = static_cast<Sprite*>(selectedTrap);
		if (touchPoint.y > 500)
		{
			log("리턴");
			return;
		}
		target->setPosition(target->getPosition() + touch->getDelta());
	}
	else if (isSkill)
	{
		auto target = static_cast<Sprite*>(skill->getChildByTag(50));
		target->setPosition(target->getPosition() + touch->getDelta());
	}
	else if (isSkill2)
	{
		auto target = static_cast<Sprite*>(skill2->getChildByTag(51));
		target->setPosition(target->getPosition() + touch->getDelta());
	}

	// 아니라면 공격
	else {
		if(attackDelayTime>= attackRate){
			isAttack = true;
		}
		attackPoint = touchPoint;
	}
}

void HelloWorld::re_Idle()
{
	player->stopAllActions();

		auto player_idle = ResouceLoad::getInstance()->player_idleAnimate->clone();
		auto rep = RepeatForever::create(player_idle);
		player->runAction(rep);
	
}
void HelloWorld::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{

	auto touchPoint = touch->getLocation();
	isAttack = false;
	// 플레이어 idle 애니메이션 재시작

	// 스킬실험
	if (isSkill) {
		isSkill = false;
		skill_cool = true;

		auto target = static_cast<Sprite*>(skill->getChildByTag(50));
		target->setVisible(false);
		Size parentSize;
		parentSize = skill->getContentSize();
		Vec2 w_position = skill->convertToWorldSpace(target->getPosition());
		if (!isgameOver) {
		myContactListener->trigger(w_position, PlayerInfoSingleTon::getInstance()->skill_blastRadius, 0, 100);
		target->setPosition(Vec2(parentSize.width / 2.0, parentSize.height / 2.0));

		//폭탄애니메이션 실험

			auto cache = SpriteFrameCache::getInstance();
			cache->addSpriteFramesWithFile("explosion/ExplosionPlist.plist");

			auto exp = Sprite::createWithSpriteFrameName("explosion_10002.png");
			exp->setPosition(w_position);
			exp->setScale(2.8f);
			gameLayer->addChild(exp, 1200);

			auto explosion1 = ResouceLoad::getInstance()->explosion1->clone();
			auto rep = Sequence::create(explosion1,
				CallFunc::create(CC_CALLBACK_0(HelloWorld::remove_anim, this, exp)), nullptr);
			exp->runAction(rep);

			// 프로그래스 시험
			auto ui_cooldown = Sprite::create("ui/ui_cooldown.png");

			auto to1 = ProgressFromTo::create(PlayerInfoSingleTon::getInstance()->skill_cooltime, 100, 0);

			auto progress = ProgressTimer::create(ui_cooldown);
			progress->setType(ProgressTimer::Type::RADIAL);
			progress->setReverseProgress(true);
			progress->setOpacity(180.0f);
			skill->addChild(progress);
			progress->setPosition(Vec2(parentSize.width / 2.0, parentSize.height / 2.0));

			auto p_seq = Sequence::create(to1,
				CallFunc::create(CC_CALLBACK_0(HelloWorld::cooldown_finish, this, progress, 0)), nullptr);
			progress->runAction(p_seq);
		}
	}
	else if (isSkill2)
	{
		isSkill2 = false;
		skill2_cool = true;

		auto target = static_cast<Sprite*>(skill2->getChildByTag(51));
		target->setVisible(false);
		Size parentSize;
		parentSize = skill2->getContentSize();
		Vec2 w_position = skill2->convertToWorldSpace(target->getPosition());
		if (!isgameOver)
		{
		myContactListener->trigger(w_position, PlayerInfoSingleTon::getInstance()->skill2_blastRadius, 2, 100);
		target->setPosition(Vec2(parentSize.width / 2.0, parentSize.height / 2.0));


		//파이프폭탄 이펙트 시험
			auto cache = SpriteFrameCache::getInstance();
			cache->addSpriteFramesWithFile("explosion/ExplosionPlist.plist");

			auto exp = Sprite::createWithSpriteFrameName("explosion_10002.png");
			exp->setPosition(w_position);
			exp->setScale(1.5f);
			gameLayer->addChild(exp, 1200);

			auto explosion1 = ResouceLoad::getInstance()->explosion1->clone();
			auto rep = Sequence::create(explosion1,
				CallFunc::create(CC_CALLBACK_0(HelloWorld::remove_anim, this, exp)), nullptr);
			exp->runAction(rep);

			ParticleSystem * smoke = ParticleSmoke::create();

			auto texture = Director::getInstance()->getTextureCache()->addImage("explosion/fire.png");
			smoke->setTexture(texture);
			if (smoke != nullptr)
			{
				smoke->setScaleX(1.5);
				smoke->setScaleY(3.0);
				smoke->setDuration(3.0);
				smoke->setPosition(w_position);
				gameLayer->addChild(smoke, 1000);
			}

			// 프로그래스 시험
			auto ui_cooldown = Sprite::create("ui/ui_cooldown.png");

			auto to1 = ProgressFromTo::create(PlayerInfoSingleTon::getInstance()->skill2_cooltime, 100, 0);

			auto progress = ProgressTimer::create(ui_cooldown);
			progress->setType(ProgressTimer::Type::RADIAL);
			progress->setReverseProgress(true);
			progress->setOpacity(180.0f);
			skill2->addChild(progress);
			progress->setPosition(Vec2(parentSize.width / 2.0, parentSize.height / 2.0));

			auto p_seq = Sequence::create(to1,
				CallFunc::create(CC_CALLBACK_0(HelloWorld::cooldown_finish, this, progress, 1)), nullptr);
			progress->runAction(p_seq);
		}
	}
	else if (selectedTrap)
	{
		if (item->getBoundingBox().containsPoint(touchPoint))
		{
			for (int i = 0; i < traps->size(); i++)
			{
				auto trap = (Trap*)traps->at(i);

				if (trap->sprite == selectedTrap)
				{
					if (trap->type == 0)
					{
						PlayerInfoSingleTon::getInstance()->have_trap1++;
						gameLayer->removeChild(trap->sprite);
						gameLayer->removeChild(trap);
						traps->erase(traps->begin() + i);
						break;
					}
				}
			}
		}
		else if(item2->getBoundingBox().containsPoint(touchPoint))
		{
			for (int i = 0; i < traps->size(); i++)
			{
				auto trap = (Trap*)traps->at(i);

				if (trap->sprite == selectedTrap)
				{
					if (trap->type == 1)
					{
						PlayerInfoSingleTon::getInstance()->have_trap2++;
						gameLayer->removeChild(trap->sprite);
						gameLayer->removeChild(trap);
						traps->erase(traps->begin() + i);
						break;
					}
				}
			}
		}
	}


	selectedTrap = nullptr;
	isSelectedTrap = false;
}
void HelloWorld::cooldown_finish(Node* sender,int type)
{
	auto progress = (ProgressTimer *)sender;
	if (type == 0) {
		skill_cool = false;
		skill->removeChild(progress, true);
	}
	else if(type == 1)
	{
		skill2_cool = false;
		skill2->removeChild(progress, true);
	}
}

void HelloWorld::remove_label(Node* sender)
{
	auto label = (LabelBMFont*)sender;
	menuLayer->removeChild(label);
}
void HelloWorld::remove_anim(Node* sender)
{
	auto sprite = (Sprite*)sender;
	gameLayer->removeChild(sprite);
}


void HelloWorld::gameOver()
{

	pMenu->setEnabled(false);
	shopMenu->setEnabled(false);

	for (int i = 0; i < monsters->size(); i++)
	{
		monsters->at(i)->isPipe = false;
	}
	for (int k = 0; k < bullets->size(); k++)
	{
		gameLayer->removeChild(bullets->at(k)->sprite);
		_world->DestroyBody(bullets->at(k)->body);;
		//delete bullets->at(k);
	}
	for (int z = 0; z < helpers->size(); z++)
	{
		gameLayer->removeChild(helpers->at(z)->sprite);
		gameLayer->removeChild(helpers->at(z));
		//delete helpers->at(z);
	}
	helpers->clear();
	bullets->clear();

	// ------------ 게임 오버 애니매이션 ---------

	// 바리게이트 페이드
	auto ba_seq = Sequence::create(FadeOut::create(1.5),
		CallFunc::create(CC_CALLBACK_0(HelloWorld::remove_anim, this, bari)), nullptr);
	auto ba_seq2 = Sequence::create(FadeOut::create(1.5),
		CallFunc::create(CC_CALLBACK_0(HelloWorld::remove_anim, this, bari)), nullptr);
	auto ba_seq3 = Sequence::create(FadeOut::create(1.5),
		CallFunc::create(CC_CALLBACK_0(HelloWorld::remove_anim, this, bari)), nullptr);

	bari->runAction(ba_seq);
	bari2->runAction(ba_seq2);
	bari3->runAction(ba_seq3);
	

	// 바리게이트 폭탄
	auto b_bomb = Sprite::create("explosion/barricade_bomb.png");
	b_bomb->setTextureRect(Rect(262, 324, 66, 81));
	b_bomb->setPosition(Vec2(200, winSize.height / 2 + 100));
	b_bomb->setScale(3.0f);
	gameLayer->addChild(b_bomb,2000);
	auto barri_bomb_anim = ResouceLoad::getInstance()->barricade_bomb->clone();
	auto rep = Sequence::create(Repeat::create(barri_bomb_anim,2),
		CallFunc::create(CC_CALLBACK_0(HelloWorld::remove_anim, this, b_bomb)), nullptr);
	b_bomb->runAction(rep);

	auto b_bomb2 = Sprite::create("explosion/barricade_bomb.png");
	b_bomb2->setTextureRect(Rect(262, 324, 66, 81));
	b_bomb2->setPosition(Vec2(280, winSize.height / 2 - 45));
	b_bomb2->setScale(3.0f);
	gameLayer->addChild(b_bomb2, 2000);
	auto barri_bomb_anim2 = ResouceLoad::getInstance()->barricade_bomb->clone();
	auto rep2 = Sequence::create(DelayTime::create(0.1f)
		,barri_bomb_anim2,
		CallFunc::create(CC_CALLBACK_0(HelloWorld::remove_anim, this, b_bomb2)), nullptr);
	b_bomb2->runAction(rep2);

	auto b_bomb3 = Sprite::create("explosion/barricade_bomb.png");
	b_bomb3->setTextureRect(Rect(262, 324, 66, 81));
	b_bomb3->setPosition(Vec2(370, winSize.height / 2 - 100));
	b_bomb3->setScale(3.0f);
	gameLayer->addChild(b_bomb3, 2000);
	auto barri_bomb_anim3 = ResouceLoad::getInstance()->barricade_bomb->clone();
	auto rep3 = Sequence::create(DelayTime::create(0.3f)
		,barri_bomb_anim3,
		CallFunc::create(CC_CALLBACK_0(HelloWorld::remove_anim, this, b_bomb3)), nullptr);
	b_bomb3->runAction(rep3);
	
	auto b_bomb4 = Sprite::create("explosion/barricade_bomb.png");
	b_bomb4->setTextureRect(Rect(262, 324, 66, 81));
	b_bomb4->setPosition(Vec2(390, winSize.height / 2 - 150));
	b_bomb4->setScale(3.0f);
	gameLayer->addChild(b_bomb4, 2000);
	auto barri_bomb_anim4 = ResouceLoad::getInstance()->barricade_bomb->clone();
	auto rep4 = Sequence::create(DelayTime::create(0.1f)
		, Repeat::create(barri_bomb_anim4,2)
		,CallFunc::create(CC_CALLBACK_0(HelloWorld::remove_anim, this, b_bomb4)), nullptr);
	b_bomb4->runAction(rep4);
	
	auto b_bomb5 = Sprite::create("explosion/barricade_bomb.png");
	b_bomb5->setTextureRect(Rect(262, 324, 66, 81));
	b_bomb5->setPosition(Vec2(290, winSize.height / 2 - 60));
	b_bomb5->setScale(3.0f);
	gameLayer->addChild(b_bomb5, 2000);
	auto barri_bomb_anim5 = ResouceLoad::getInstance()->barricade_bomb->clone();
	auto rep5 = Sequence::create(DelayTime::create(0.15f)
		, Repeat::create(barri_bomb_anim5, 2)
		, CallFunc::create(CC_CALLBACK_0(HelloWorld::remove_anim, this, b_bomb5)), nullptr);
	b_bomb5->runAction(rep5);
	
	auto b_bomb6 = Sprite::create("explosion/barricade_bomb.png");
	b_bomb6->setTextureRect(Rect(262, 324, 66, 81));
	b_bomb6->setPosition(Vec2(195, winSize.height / 2 + 190));
	b_bomb6->setScale(3.0f);
	gameLayer->addChild(b_bomb6, 2000);
	auto barri_bomb_anim6 = ResouceLoad::getInstance()->barricade_bomb->clone();
	auto rep6 = Sequence::create(DelayTime::create(0.01f)
		, Repeat::create(barri_bomb_anim6, 1)
		, barri_bomb_anim6,
		CallFunc::create(CC_CALLBACK_0(HelloWorld::remove_anim, this, b_bomb6)), nullptr);
	b_bomb6->runAction(rep6);
	
	auto b_bomb7 = Sprite::create("explosion/barricade_bomb.png");
	b_bomb7->setTextureRect(Rect(262, 324, 66, 81));
	b_bomb7->setPosition(Vec2(300, winSize.height / 2 ));
	b_bomb7->setScale(3.0f);
	gameLayer->addChild(b_bomb7, 2000);
	auto barri_bomb_anim7 = ResouceLoad::getInstance()->barricade_bomb->clone();
	auto rep7 = Sequence::create(DelayTime::create(0.08f)
		, Repeat::create(barri_bomb_anim7, 1)
		, barri_bomb_anim7,
		CallFunc::create(CC_CALLBACK_0(HelloWorld::remove_anim, this, b_bomb7)), nullptr);
	b_bomb7->runAction(rep7);

	auto b_bomb8 = Sprite::create("explosion/barricade_bomb.png");
	b_bomb8->setTextureRect(Rect(262, 324, 66, 81));
	b_bomb8->setPosition(Vec2(240, winSize.height / 2 + 50));
	b_bomb8->setScale(3.0f);
	gameLayer->addChild(b_bomb8, 2000);
	auto barri_bomb_anim8 = ResouceLoad::getInstance()->barricade_bomb->clone();
	auto rep8 = Sequence::create(DelayTime::create(0.02f)
		, Repeat::create(barri_bomb_anim8, 1)
		, barri_bomb_anim8,
		CallFunc::create(CC_CALLBACK_0(HelloWorld::remove_anim, this, b_bomb8)), nullptr);
	b_bomb8->runAction(rep8);
	
	//////////////////
	
	auto blood = Sprite::create("player/blood.png");
	blood->setTextureRect(Rect(0, 0,0,0));
	blood->setPosition(Vec2(player->getPosition().x + 40,player->getPosition().y + 70));
	gameLayer->addChild(blood, 1600);
	auto blood_anim = ResouceLoad::getInstance()->blood->clone();

	auto b_seq = Sequence::create(DelayTime::create(2.6)
		, blood_anim,
		CallFunc::create(CC_CALLBACK_0(HelloWorld::remove_anim, this, blood)), nullptr);
	blood->runAction(b_seq);

	auto blood2 = Sprite::create("player/blood.png");
	blood2->setTextureRect(Rect(0, 0, 0, 0));
	blood2->setRotation(200.0f);
	blood2->setPosition(Vec2(player->getPosition().x + 60, player->getPosition().y + 20));
	gameLayer->addChild(blood2, 1600);
	auto blood_anim2 = ResouceLoad::getInstance()->blood->clone();
	auto b_seq2 = Sequence::create(DelayTime::create(2.7)
		,blood_anim2,
		CallFunc::create(CC_CALLBACK_0(HelloWorld::remove_anim, this, blood2)), nullptr);
	blood2->runAction(b_seq2);

	auto blood3 = Sprite::create("player/blood.png");
	blood3->setTextureRect(Rect(0, 0, 0, 0));
	blood3->setRotation(250.0f);
	blood3->setPosition(Vec2(player->getPosition().x + 20, player->getPosition().y + 20));
	gameLayer->addChild(blood3,1600);
	auto blood_anim3 = ResouceLoad::getInstance()->blood->clone();
	auto b_seq3 = Sequence::create(DelayTime::create(2.85)
		, blood_anim3,
		CallFunc::create(CC_CALLBACK_0(HelloWorld::remove_anim, this, blood3)),
		nullptr);
	blood3->runAction(b_seq3);

	player->stopAllActions();
	gameLayer->removeChild(player);
//	player = nullptr;


	auto player = Sprite::create("player/player_idle.png", Rect(0, 0, 136, 72));
	player->setScale(1.5f);
	player->setAnchorPoint(Vec2(0, 0));
	player->setPosition(Vec2(player->getContentSize().width / 2 - 20,
		winSize.height / 2 - 70));
	gameLayer->addChild(player, 1500);
	
	auto dead = ResouceLoad::getInstance()->player_deadAnimate->clone();
	auto p_seq = Sequence::create(DelayTime::create(2.6)
		, dead, FadeOut::create(1.5f), nullptr);
		
	player->runAction(p_seq);

	auto gameOver = Sprite::create("ui/GameOver.png");
	gameOver->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	gameOver->setScale(0.0f);
	gameLayer->addChild(gameOver, 2000);

	auto scale = ScaleTo::create(0.5, 5.0f);
	auto r_scale = ScaleTo::create(0.1, 1.0f);
	auto seq = Sequence::create(DelayTime::create(4.0f),
		ScaleTo::create(0.3, 5.0f),
		EaseElasticIn::create(r_scale->clone(), 5.0),
		DelayTime::create(2.0f),
		CallFunc::create(CC_CALLBACK_0(HelloWorld::gameOverMenu, this)),
		ScaleTo::create(0.1, 5.0f),
		RemoveSelf::create(true),
		nullptr);
	gameOver->runAction(seq);

	isgameOver = true;
	_world->DestroyBody(_barricade);
	_world->DestroyBody(groundBody);
}

void HelloWorld::gameOverMenu()
{
	result_Layer->setVisible(true);
	result_Layer->setOpacity(0);

	auto fade = FadeTo::create(2.5, 150);
	result_Layer->runAction(fade);
	
	auto highLevel_str = String::createWithFormat("High Level :   %d", UserDefault::getInstance()->getIntegerForKey("int_key"));

	highLevel = LabelBMFont::create(highLevel_str->getCString(), "fonts/futura-48.fnt");
	highLevel->setScale(0.0f);
	highLevel->setPosition(Vec2(winSize.width / 2, winSize.height / 2 + 150));
	highLevel->setColor(Color3B::RED);
	result_Layer->addChild(highLevel);

	auto seq2 = Sequence::create(DelayTime::create(2.6f),
		ScaleTo::create(0.6f, 2.0f), nullptr);
	highLevel->runAction(seq2);

	auto nowLevel_str = String::createWithFormat("Last Level :   %d", MonsterInfoSingleTon::getInstance()->level);

	nowLevel = LabelBMFont::create(nowLevel_str->getCString(), "fonts/futura-48.fnt");
	nowLevel->setScale(0.0f);
	nowLevel->setPosition(Vec2(winSize.width / 2, winSize.height / 2 - 100));
	nowLevel->setColor(Color3B::RED);
	result_Layer->addChild(nowLevel);

	auto seq = Sequence::create(DelayTime::create(3.0f),
		ScaleTo::create(0.6f, 2.0f), nullptr);
	nowLevel->runAction(seq);
	
	// 최고기록 갱신
	int high_Level = UserDefault::getInstance()->getIntegerForKey("int_key");
	if (high_Level < MonsterInfoSingleTon::getInstance()->level)
	{
		UserDefault::getInstance()->setIntegerForKey("int_key", MonsterInfoSingleTon::getInstance()->level);
		this->scheduleOnce(schedule_selector(HelloWorld::high_LevelUpdate), 5.0f);
	}
	else {
		this->scheduleOnce(schedule_selector(HelloWorld::back_button), 4.0f);
	}
}

void HelloWorld::high_LevelUpdate(float f)
{
	auto seq = Sequence::create(DelayTime::create(0.3f)
		,ScaleTo::create(0.5, 0.0),
		CallFunc::create(CC_CALLBACK_0(HelloWorld::LabelUpdate, this)),
		ScaleTo::create(0.7, 3.0), nullptr);
	nowLevel->runAction(seq);

	auto seq1 = Sequence::create(ScaleTo::create(0.5, 0),
		RemoveSelf::create(true), nullptr);

	highLevel->runAction(seq1);


	this->scheduleOnce(schedule_selector(HelloWorld::back_button), 2.0f);
	this->schedule(schedule_selector(HelloWorld::high_particle),5.0 / 60);
}

void HelloWorld::high_particle(float f)
{
	ParticleSystem* emitter = ParticleSystemQuad::create("explosion/ExplodingRing.plist");

	emitter->setPosition(Vec2(random(50, 1200), random(50, 680)));
	emitter->setAutoRemoveOnFinish(true);

	result_Layer->addChild(emitter);
}
void HelloWorld::back_button(float f)
{
	auto pMenuItem = MenuItemImage::create(
		"ui/back.png",
		"ui/back_push.png",
		CC_CALLBACK_1(HelloWorld::Intro, this));
	pMenuItem->setScale(0.0f);
	auto pMenu = Menu::create(pMenuItem, nullptr);
	pMenu->setPosition(Vec2(winSize.width/2, 100));
	result_Layer->addChild(pMenu);

	auto scale = ScaleTo::create(0.2, 1.0f);
	pMenuItem->runAction(scale);
}

void HelloWorld::LabelUpdate()
{
	auto highLevel_str = String::createWithFormat("High Level :   %d", UserDefault::getInstance()->getIntegerForKey("int_key"));
	nowLevel->setString(highLevel_str->getCString());
	nowLevel->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
}
void HelloWorld::Intro(Ref* pSender)
{
	log("인트로");
}
void HelloWorld::addMenu()
{

	// 레벨UI
	level_ui = Sprite::create("ui/ui_level.png");
	level_ui->setPosition(Vec2(105, winSize.height - 75));
	level_ui->setScale(1.2f);
	menuLayer->addChild(level_ui);
	Size level_ui_Size = level_ui->getContentSize();

	// 현재 무기
	weapon_ui = Sprite::create("ui/ui_weapon.png");
	weapon_ui->setPosition(Vec2(level_ui_Size.width / 2 - 35,10));
	level_ui->addChild(weapon_ui);
	Size weapon_ui_Size = weapon_ui->getContentSize();

	weapon = Sprite::create("item/gun01.png");
	weapon->setPosition(Vec2(weapon_ui_Size.width / 2, weapon_ui_Size.height / 2));
	weapon->setScale(0.7f);
	weapon_ui->addChild(weapon);

	// 현재 레벨

	levelLabel = LabelBMFont::create("Level : 1", "fonts/futura-48.fnt");
	levelLabel->setScale(0.7f);
	levelLabel->setPosition(Vec2(level_ui_Size.width/2,level_ui_Size.height/2));
	levelLabel->setColor(Color3B::RED);
	level_ui->addChild(levelLabel);

	// 웨이브 진행 상황

	auto _waveProgress = Sprite::create("ui/m_pro.png");
	
	_waveProgress->setAnchorPoint(Vec2(0, 0.5));
	_waveProgress->setPosition(Vec2(winSize.width / 2 - 100, winSize.height - 50));
	menuLayer->addChild(_waveProgress);

	waveProgress = Sprite::create("ui/m_pro2.png");
	//waveProgress->setTextureRect(Rect(0, 0, 300, 10));
	//waveProgress->setColor(Color3B::BLUE);
	waveProgress->setAnchorPoint(Vec2(0, 0.5));
	waveProgress->setPosition(Vec2(winSize.width / 2 - 100, winSize.height - 50));
	menuLayer->addChild(waveProgress);

	m_pro = Sprite::create("ui/m_pro3.png");
	m_pro->setPosition(Vec2(((winSize.width / 2 - 100)), winSize.height - 40));
	menuLayer->addChild(m_pro);

	// 플레이어 HP

	playerHpBar = Sprite::create("ui/ui_hp_bar.png");
	playerHpBar->setScale(1.5f);
	playerHpBar->setAnchorPoint(Vec2(0, 0.5));
	playerHpBar->setPosition(Vec2(level_ui_Size.width - 7,level_ui_Size.height/2 + 20));
	level_ui->addChild(playerHpBar);

	playerHp = Sprite::create("ui/ui_hp.png");
	playerHp->setAnchorPoint(Vec2(0.071, 0.5));
	playerHp->setPosition(Vec2(12, 13));
	playerHpBar->addChild(playerHp);
	
	// 골드 
	auto gold = Sprite::create("item/gold.png");
	gold->setPosition(Vec2(level_ui_Size.width+20, level_ui_Size.height / 2-20));
	level_ui->addChild(gold);
	
	// 골드라벨
	cocos2d::String *money_In_Hand;
	money_In_Hand = String::createWithFormat("%d", PlayerInfoSingleTon::getInstance()->money_In_Hand);

	money_label = LabelBMFont::create(money_In_Hand->getCString(), "fonts/futura-48.fnt");
	money_label->setPosition(Vec2(level_ui_Size.width + 90, level_ui_Size.height / 2 - 20));
	money_label->setScale(0.5f);
	level_ui->addChild(money_label);

	// 시작 버튼

	auto pMenuItem = MenuItemImage::create(
		"ui/play.png",
		"ui/play_push.png",
		CC_CALLBACK_1(HelloWorld::waveStart, this));
	pMenuItem->setScaleX(0.6);
	pMenuItem->setScaleY(0.7);
	pMenu = Menu::create(pMenuItem, nullptr);
	pMenu->setPosition(Vec2(winSize.width - 100, winSize.height - 50));
	menuLayer->addChild(pMenu);

	// 상점 메뉴
	auto shop = MenuItemImage::create(
		"ui/shop.png",
		"ui/shop_push.png",
		CC_CALLBACK_1(HelloWorld::shopOpen, this));
	shop->setScaleX(0.6f);
	shop->setScaleY(0.7f);
	 shopMenu = Menu::create(shop, nullptr);

	shopMenu->setPosition(Vec2(winSize.width - 280, winSize.height - 50));
	menuLayer->addChild(shopMenu);

	//스킬1
	skill = Sprite::create("ui/ui_item.png");
	skill->setPosition(Vec2(winSize.width - 225, 60));

	menuLayer->addChild(skill,1000);

	auto bomb = Sprite::create("item/bomb.png");
	Size parentSize;
	parentSize = skill->getContentSize();
	bomb->setPosition(Vec2(parentSize.width / 2.0, parentSize.height / 2.0));
	bomb->setScale(3.0f);
	skill->addChild(bomb);

	auto range = Sprite::create("ui/ui_range.png");
	range->setPosition(Vec2(parentSize.width / 2.0, parentSize.height / 2.0));
	range->setScale(PlayerInfoSingleTon::getInstance()->skill_blastRadius);
	range->setTag(50);
	range->setVisible(false);
	skill->addChild(range);


	//스킬2
	
	skill2 = Sprite::create("ui/ui_item.png");
	skill2->setPosition(Vec2(winSize.width - 75, 60));
	menuLayer->addChild(skill2,1000);

	auto bomb2 = Sprite::create("item/pipe_bomb.png");
	Size parentSize2;
	parentSize2 = skill2->getContentSize();
	bomb2->setPosition(Vec2(parentSize2.width / 2.0, parentSize2.height / 2.0));
	bomb2->setScale(3.0f);
	skill2->addChild(bomb2);

	auto range2 = Sprite::create("ui/ui_range.png");
	range2->setPosition(Vec2(parentSize2.width / 2.0, parentSize2.height / 2.0));
	range2->setScale(PlayerInfoSingleTon::getInstance()->skill2_blastRadius);
	range2->setTag(51);
	range2->setVisible(false);
	skill2->addChild(range2);

	//아이템 창
	item = Sprite::create("ui/ui_item.png");
	item->setPosition(Vec2(75, 60));
	menuLayer->addChild(item,1000);

	auto trap1 = Sprite::create("item/trap01.png");
	trap1->setPosition(Vec2(parentSize.width / 2.0, parentSize.height / 2.0));
	trap1->setScale(2.0f);
	item->addChild(trap1);

	item_Label = LabelBMFont::create("0", "fonts/futura-48.fnt");
	
	item_Label->setPosition(parentSize.width/2.0, 15);
	item_Label->setColor(Color3B::RED);
	item_Label->setScale(0.8f);
	item->addChild(item_Label);
	
	item2 = Sprite::create("ui/ui_item.png");
	item2->setPosition(Vec2(225, 60));
	menuLayer->addChild(item2,1000);

	auto trap2 = Sprite::create("item/trap02.png");
	trap2->setPosition(Vec2(parentSize.width / 2.0, parentSize.height / 2.0));
	trap2->setScale(2.0f);
	item2->addChild(trap2);
	
	item2_Label = LabelBMFont::create("0", "fonts/futura-48.fnt");
	item2_Label->setPosition(parentSize.width / 2.0, 15);
	item2_Label->setColor(Color3B::RED);
	item2_Label->setScale(0.8f);
	item2->addChild(item2_Label);


	// 게임 결과 레이어
	result_Layer = LayerColor::create(Color4B(0, 0, 0, 100), winSize.width, winSize.height);

	this->addChild(result_Layer, 1000);
	result_Layer->setVisible(false);

}

void HelloWorld::resultClose(Ref* pSender)
{

	//결과창 애니메이션
	auto scale = ScaleTo::create(0.6f, 0.0f);
	auto rotate = RotateBy::create(0.3, 360);
	auto r_rep = Repeat::create(rotate, 2);
	auto spa = Spawn::create(scale, r_rep, nullptr);	
	auto seq = Sequence::create(spa,
		CallFunc::create(CC_CALLBACK_0(HelloWorld::close, this)), nullptr);
	result_background->runAction(seq);

}
void HelloWorld::close()
{
	take_gold = 0;
	result_Layer->removeAllChildrenWithCleanup(true);
	result_Layer->setVisible(false);
	isResultLayer = false;
	pMenu->setEnabled(true);
	shopMenu->setEnabled(true);
}
void HelloWorld::shopOpen(Ref * pSender)
{
	if (isWave == false) {
		auto pScene = ShopScene::createScene();
		Director::getInstance()->pushScene(TransitionFadeTR::create(1, pScene));
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

	//this->unschedule(schedule_selector(HelloWorld::tick));
	this->unschedule(schedule_selector(HelloWorld::high_particle));
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