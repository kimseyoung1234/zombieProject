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
	auto background = Sprite::create("background.png");
	background->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	background->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(background);

	// 사용자 UI 추가
	addMenu();


	///////////////////////
	//월드 생성
	if (this->createBox2dWorld(true))
	{
		this->schedule(schedule_selector(HelloWorld::tick));
	}
	// 플레이어 생성
	

	player = Sprite::create("player_idle.png", Rect(0, 0, 136, 72));
	player->setScale(1.5f);
	player->setAnchorPoint(Vec2(0, 0));
	player->setPosition(Vec2(player->getContentSize().width / 2 + 30,
		winSize.height / 2 + 43));
	gameLayer->addChild(player);

	auto player_idle = ResouceLoad::getInstance()->player_idleAnimate->clone();
	auto rep = RepeatForever::create(player_idle);
	player->runAction(rep);


	result();
	return true;
}

// 웨이브 시작 (나중에 조정)
void HelloWorld::waveStart(Ref* pSender)
{
	if (isWave == false) {
		int maxMonster = MonsterInfoSingleTon::getInstance()->maxMonster;
		for (int i = 0; i < maxMonster; i++) {
			int x_rand = random(1350, 1700);
			int y_rand = random(50, 600);
			int r_monsterType = random(1, 3);
			Monster * mon = new Monster(Vec2(x_rand, y_rand),r_monsterType);
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
	groundEdge.Set(b2Vec2(0, 1), b2Vec2((winSize.width + 500) / PTM_RATIO, 1));
	groundBody->CreateFixture(&boxShapeDef);

	//왼쪽
	groundEdge.Set(b2Vec2(0, 0), b2Vec2(0, winSize.width / PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);

	//위쪽
	groundEdge.Set(b2Vec2(0, (winSize.height-100) / PTM_RATIO), b2Vec2((winSize.width + 500) / PTM_RATIO, (winSize.height-100) / PTM_RATIO));
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

	auto _barricade = _world->CreateBody(&barricadeBodyDef);

	b2FixtureDef BoxShapeDef;
	b2PolygonShape barricadeBox;
	BoxShapeDef.shape = &barricadeBox;
	BoxShapeDef.filter.categoryBits = 0x0004;
	BoxShapeDef.filter.maskBits = 0x0002 ;

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

		// 플레이어 HP바 갱신
		playerHp->setScaleX(PlayerInfoSingleTon::getInstance()->hp / 100.0f);

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
				spriteData->setRotation(spriteData->getRotation());
			}
		}

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
		}
	}
}
void HelloWorld::result()
{
	log("클리어!");

	// 게임 결과 배경
	auto result_background = Sprite::create("ui/ui_result.png");
	result_background->setPosition(winSize.width / 2, winSize.height / 2);
	result_Layer->addChild(result_background);

	result_Layer->setVisible(true);

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
						gameLayer->addChild(exp, 200);

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
						gameLayer->addChild(exp, 200);

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
		if (attackDelayTime >= attackRate) {

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
/*	player->stopAllActions();
	
	auto player_idle = ResouceLoad::getInstance()->player_idleAnimate->clone();
	auto rep = RepeatForever::create(player_idle);
	player->runAction(rep);
	*/

	// 스킬실험
	if (isSkill) {
		isSkill = false;
		skill_cool = true;

		auto target = static_cast<Sprite*>(skill->getChildByTag(50));
		target->setVisible(false);
		Size parentSize;
		parentSize = skill->getContentSize();
		Vec2 w_position = skill->convertToWorldSpace(target->getPosition());
		myContactListener->trigger(w_position, PlayerInfoSingleTon::getInstance()->skill_blastRadius, 0, 100);
		target->setPosition(Vec2(parentSize.width / 2.0, parentSize.height / 2.0));

		//폭탄애니메이션 실험
		auto cache = SpriteFrameCache::getInstance();
		cache->addSpriteFramesWithFile("explosion/ExplosionPlist.plist");
	
		auto exp = Sprite::createWithSpriteFrameName("explosion_10002.png");
		exp->setPosition(w_position);
		exp->setScale(2.8f);
		gameLayer->addChild(exp,200);

		auto explosion1 = ResouceLoad::getInstance()->explosion1->clone();
		auto rep = Sequence::create(explosion1,
			CallFunc::create(CC_CALLBACK_0(HelloWorld::remove_anim, this,exp)),nullptr);
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
			CallFunc::create(CC_CALLBACK_0(HelloWorld::cooldown_finish, this, progress,0)), nullptr);
		progress->runAction(p_seq);
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
		myContactListener->trigger(w_position, PlayerInfoSingleTon::getInstance()->skill2_blastRadius, 2, 100);
		target->setPosition(Vec2(parentSize.width / 2.0, parentSize.height / 2.0));

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
			CallFunc::create(CC_CALLBACK_0(HelloWorld::cooldown_finish, this, progress,1)), nullptr);
		progress->runAction(p_seq);
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
	//isgameOver = true;
	log("게임 오버!");
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

	waveProgress = Sprite::create("white-512x512.png");
	waveProgress->setTextureRect(Rect(0, 0, 300, 10));
	waveProgress->setColor(Color3B::BLUE);
	waveProgress->setAnchorPoint(Vec2(0, 0.5));
	waveProgress->setPosition(Vec2(winSize.width / 2 - 150, winSize.height - 50));
	menuLayer->addChild(waveProgress);

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
		"ui/ui_play.png",
		"ui/ui_play.png",
		CC_CALLBACK_1(HelloWorld::waveStart, this));
	pMenuItem->setScale(0.7f);
	pMenu = Menu::create(pMenuItem, nullptr);
	pMenu->setPosition(Vec2(winSize.width - 100, winSize.height - 50));
	menuLayer->addChild(pMenu);

	// 상점 메뉴
	auto shop = MenuItemImage::create(
		"ui/ui_shop.png",
		"ui/ui_shop.png",
		CC_CALLBACK_1(HelloWorld::shopOpen, this));
	shop->setScale(0.7f);
	 shopMenu = Menu::create(shop, nullptr);

	shopMenu->setPosition(Vec2(winSize.width - 250, winSize.height - 50));
	menuLayer->addChild(shopMenu);

	//스킬1
	skill = Sprite::create("ui/ui_item.png");
	skill->setPosition(Vec2(winSize.width - 225, 60));
	skill->setOpacity(180.0f);

	gameLayer->addChild(skill,1000);

	auto bomb = Sprite::create("item/bomb.png");
	Size parentSize;
	parentSize = skill->getContentSize();
	bomb->setPosition(Vec2(parentSize.width / 2.0, parentSize.height / 2.0));
	bomb->setScale(3.0f);
	bomb->setOpacity(180.0f);
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
	skill2->setOpacity(180.0f);
	gameLayer->addChild(skill2,1000);

	auto bomb2 = Sprite::create("item/pipe_bomb.png");
	Size parentSize2;
	parentSize2 = skill2->getContentSize();
	bomb2->setPosition(Vec2(parentSize2.width / 2.0, parentSize2.height / 2.0));
	bomb2->setScale(3.0f);
	bomb2->setOpacity(180.0f);
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
	//item->setOpacity(180.0f);
	gameLayer->addChild(item,1000);

	auto trap1 = Sprite::create("item/trap01.png");
	trap1->setPosition(Vec2(parentSize.width / 2.0, parentSize.height / 2.0));
	trap1->setScale(2.0f);
	trap1->setOpacity(180.0f);
	item->addChild(trap1);

	item_Label = LabelBMFont::create("0", "fonts/futura-48.fnt");
	
	item_Label->setPosition(parentSize.width/2.0, 15);
	item_Label->setColor(Color3B::RED);
	item_Label->setScale(0.8f);
	item->addChild(item_Label);
	

	///
	item2 = Sprite::create("ui/ui_item.png");
	item2->setPosition(Vec2(225, 60));
	//item2->setOpacity(180.0f);
	gameLayer->addChild(item2,1000);

	auto trap2 = Sprite::create("item/trap02.png");
	trap2->setPosition(Vec2(parentSize.width / 2.0, parentSize.height / 2.0));
	trap2->setScale(2.0f);
	trap2->setOpacity(180.0f);
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