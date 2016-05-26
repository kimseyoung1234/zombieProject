#include "ResouceLoad.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

ResouceLoad * ResouceLoad::m_pInstance = nullptr;

ResouceLoad::ResouceLoad() {
	
	Loading();
}

ResouceLoad * ResouceLoad::getInstance()
{

	if (!m_pInstance)
	{
		m_pInstance = new ResouceLoad;
	}
	return m_pInstance;
}

void ResouceLoad::Loading()
{
	/////////////////////// 플레이어
	// 플레이어 Idle
	auto s_player_idle = Sprite::create("player/player_idle.png");
	auto t_player_idle = s_player_idle->getTexture();
	auto a_player_idle = Animation::create();
	a_player_idle->setDelayPerUnit(0.1f);

	for (int i = 0; i < 6; i++)
	{
		int column = i % 4;
		int row = i / 4;
		a_player_idle->addSpriteFrameWithTexture(t_player_idle, Rect(column * 136, row * 72, 136, 72));
	}
	player_idleAnimate = Animate::create(a_player_idle);
	player_idleAnimate->retain();

	//플레이어 공격
	auto s_player_attack = Sprite::create("player/player_attack.png");
	auto t_player_attack = s_player_attack->getTexture();
	auto a_player_attack = Animation::create();
	a_player_attack->setDelayPerUnit(0.02f);

	for (int i = 0; i < 8; i++)
	{
		int column = i % 4;
		int row = i / 4;
		a_player_attack->addSpriteFrameWithTexture(t_player_attack, Rect(column * 136, row * 72, 136, 72));
	}
	player_attackAnimate = Animate::create(a_player_attack);
	player_attackAnimate->retain();

	//플레이어 죽음
	auto s_player_dead = Sprite::create("player/player_dead.png");
	auto t_player_dead = s_player_dead->getTexture();
	auto a_player_dead = Animation::create();
	a_player_dead->setDelayPerUnit(0.1f);

	for (int i = 0; i < 12; i++)
	{
		int column = i % 6;
		int row = i / 6;
		a_player_dead->addSpriteFrameWithTexture(t_player_dead, Rect(column * 136, row * 72, 136, 72));
	}
	player_deadAnimate = Animate::create(a_player_dead);
	player_deadAnimate->retain();

	//////////////////////////// 몬스터 애니메이션

	// 바리게이트 히트

	auto cache3 = SpriteFrameCache::getInstance();
	cache3->addSpriteFramesWithFile("monster/hit.plist");

	Vector<SpriteFrame*> hitFrames;

	char str3[100] = { 0 };
	for (int i = 0; i < 5; i++)
	{
		sprintf(str3, "hit%02d.png", i + 1);
		SpriteFrame* frame = cache3->getSpriteFrameByName(str3);
		hitFrames.pushBack(frame);
	}

	auto a_hit = Animation::createWithSpriteFrames(hitFrames, 0.1f);
	hit = Animate::create(a_hit);
	hit->retain();

	// 브레인좀비 공격 애니메이션
	auto s_brain_attack = Sprite::create("monster/brain_attack.png");
	auto t_brain_attack = s_brain_attack->getTexture();
	auto a_brain_attack = Animation::create();
	auto a_brain_attack2 = Animation::create();
	a_brain_attack->setDelayPerUnit(0.074f);
	a_brain_attack2->setDelayPerUnit(0.074f);

	for (int i = 0; i < 12; i++)
	{
		int column = i % 15;
		int row = i / 15;
		a_brain_attack->addSpriteFrameWithTexture(t_brain_attack, Rect(column * 64, row * 64, 64, 64));
	}

	brain_attackAnimate = Animate::create(a_brain_attack);
	brain_attackAnimate->retain();

	for (int i = 12; i < 27; i++)
	{
		int column = i % 15;
		int row = i / 15;
		a_brain_attack2->addSpriteFrameWithTexture(t_brain_attack, Rect(column * 64, row * 64, 64, 64));
	}

	brain_attack2Animate = Animate::create(a_brain_attack2);
	brain_attack2Animate->retain();

	// 브레인좀비 이동 애니메이션
	auto s_brain_move = Sprite::create("monster/brain_move.png");
	auto t_brain_move = s_brain_move->getTexture();
	auto a_brain_move = Animation::create();
	a_brain_move->setDelayPerUnit(0.06f);

	for (int i = 0; i < 15; i++)
	{
		int column = i % 15;
		int row = i / 15;
		a_brain_move->addSpriteFrameWithTexture(t_brain_move, Rect(column * 64, row * 64, 64, 64));
	}
	brain_moveAnimate = Animate::create(a_brain_move);
	brain_moveAnimate->retain();

	// 팻좀비 공격 애니메이션
	auto s_fat_attack = Sprite::create("monster/fat_attack.png");
	auto t_fat_attack = s_fat_attack->getTexture();
	auto a_fat_attack = Animation::create();
	auto a_fat_attack2 = Animation::create();
	a_fat_attack->setDelayPerUnit(0.074f);
	a_fat_attack2->setDelayPerUnit(0.074f);

	for (int i = 0; i < 12; i++)
	{
		int column = i % 7;
		int row = i / 7;
		a_fat_attack->addSpriteFrameWithTexture(t_fat_attack, Rect(column * 48, row * 48, 48, 48));
	}

	fat_attackAnimate = Animate::create(a_fat_attack);
	fat_attackAnimate->retain();

	for (int i = 12; i < 18; i++)
	{
		int column = i % 7;
		int row = i / 7;
		a_fat_attack2->addSpriteFrameWithTexture(t_fat_attack, Rect(column * 48, row * 48, 48, 48));
	}

	fat_attack2Animate = Animate::create(a_fat_attack2);
	fat_attack2Animate->retain();

	// 팻좀비 이동 애니메이션
	auto s_fat_move = Sprite::create("monster/fat_move.png");
	auto t_fat_move = s_fat_move->getTexture();
	auto a_fat_move = Animation::create();
	a_fat_move->setDelayPerUnit(0.06f);

	for (int i = 0; i < 14; i++)
	{
		int column = i % 7;
		int row = i / 7;
		a_fat_move->addSpriteFrameWithTexture(t_fat_move, Rect(column * 48, row * 48, 48, 48));
	}
	fat_moveAnimate = Animate::create(a_fat_move);
	fat_moveAnimate->retain();

	//슈퍼좀비 공격 애니메이션
	auto s_super_attack = Sprite::create("monster/super_attack.png");
	auto t_super_attack = s_super_attack->getTexture();
	auto a_super_attack = Animation::create();
	auto a_super_attack2 = Animation::create();
	a_super_attack->setDelayPerUnit(0.074f);
	a_super_attack2->setDelayPerUnit(0.074f);

	for (int i = 0; i < 10; i++)
	{
		int column = i % 7;
		int row = i / 7;
		a_super_attack->addSpriteFrameWithTexture(t_super_attack, Rect(column * 64, row * 64, 64, 64));
	}

	super_attackAnimate = Animate::create(a_super_attack);
	super_attackAnimate->retain();

	for (int i = 10; i < 20; i++)
	{
		int column = i % 7;
		int row = i / 7;
		a_super_attack2->addSpriteFrameWithTexture(t_super_attack, Rect(column * 64, row * 64, 64, 64));
	}

	super_attack2Animate = Animate::create(a_super_attack2);
	super_attack2Animate->retain();
	
	//슈퍼좀비 이동 애니메이션
	auto s_super_move = Sprite::create("monster/super_move.png");
	auto t_super_move = s_super_move->getTexture();
	auto a_super_move = Animation::create();
	a_super_move->setDelayPerUnit(0.07f);

	for (int i = 0; i < 12; i++)
	{
		int column = i % 7;
		int row = i / 7;
		a_super_move->addSpriteFrameWithTexture(t_super_move, Rect(column * 64, row * 64, 64, 64));
	}
	super_moveAnimate = Animate::create(a_super_move);
	super_moveAnimate->retain();

	// 죽었을때 애니메이션
	auto s_die = Sprite::create("monster/dead.png");
	auto t_die = s_die->getTexture();
	auto a_die = Animation::create();
	a_die->setDelayPerUnit(0.074f);

	for (int i = 0; i < 15; i++)
	{
		int column = i % 15;
		int row = i / 15;
		a_die->addSpriteFrameWithTexture(t_die, Rect(column * 104, row * 104, 104, 104));
	}
	dieAnimate = Animate::create(a_die);
	dieAnimate->retain();


	// 보스 좀비 이동 애니메이션
	auto s_boss_move = Sprite::create("monster/boss_move.png");
	auto t_boss_move = s_boss_move->getTexture();
	auto a_boss_move = Animation::create();
	a_boss_move->setDelayPerUnit(0.1f);

	for (int i = 0; i < 12; i++)
	{
		int column = i % 7;
		int row = i / 7;
		a_boss_move->addSpriteFrameWithTexture(t_boss_move, Rect(column * 104, row * 104, 104, 104));
	}
	boss_moveAnimate = Animate::create(a_boss_move);
	boss_moveAnimate->retain();

	// 보스 좀비 공격 애니메이션
	 

	auto s_boss_attack = Sprite::create("monster/boss_attack.png");
	auto t_boss_attack = s_boss_attack->getTexture();
	auto a_boss_attack = Animation::create();
	auto a_boss_attack2 = Animation::create();
	a_boss_attack->setDelayPerUnit(0.1f);
	a_boss_attack2->setDelayPerUnit(0.1f);

	for (int i = 0; i < 13; i++)
	{
		int column = i % 7;
		int row = i / 7;
		a_boss_attack->addSpriteFrameWithTexture(t_boss_attack, Rect(column * 104, row * 104, 104, 104));
	}

	boss_attackAnimate = Animate::create(a_boss_attack);
	boss_attackAnimate->retain();

	for (int i = 13; i < 26; i++)
	{
		int column = i % 7;
		int row = i / 7;
		a_boss_attack2->addSpriteFrameWithTexture(t_boss_attack, Rect(column * 104, row * 104, 104, 104));
	}

	boss_attack2Animate = Animate::create(a_boss_attack2);
	boss_attack2Animate->retain();


	//보스 공격중 터지는 애니메이션
	auto s_boss_bomb = Sprite::create("monster/boss_attack_bomb.png");
	auto t_boss_bomb = s_boss_bomb->getTexture();
	auto a_boss_bomb = Animation::create();
	a_boss_bomb->setDelayPerUnit(0.04f);

	for (int i = 13; i < 31; i++)
	{
		int column = i % 7;
		int row = i / 7;
		a_boss_bomb->addSpriteFrameWithTexture(t_boss_bomb, Rect(column * 104, row * 104, 104, 104));
	}

	boss_bombAnimate = Animate::create(a_boss_bomb);
	boss_bombAnimate->retain();
	////////////////////////////////////////////// 폭파

	//폭파1 애니메이션
	auto cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("explosion/ExplosionPlist.plist");

	Vector<SpriteFrame*> expFrames;

	char str[100] = { 0 };
	for (int i = 1; i < 90; i++)
	{
		sprintf(str, "explosion_100%02d.png", i + 1);
		SpriteFrame* frame = cache->getSpriteFrameByName(str);
		expFrames.pushBack(frame);
	}

	auto a_exp = Animation::createWithSpriteFrames(expFrames, 0.02f);
	explosion1 = Animate::create(a_exp);
	explosion1->retain();

	//폭파2 애니메이션
	auto cache2 = SpriteFrameCache::getInstance();
	cache2->addSpriteFramesWithFile("explosion/Explosion2Plist.plist");

	Vector<SpriteFrame*> expFrames2;

	char str2[100] = { 0 };
	for (int i = 1; i < 90; i++)
	{
		sprintf(str2, "explosion_110%02d.png", i + 1);
		SpriteFrame* frame = cache2->getSpriteFrameByName(str2);
		expFrames2.pushBack(frame);
	}

	auto a_exp2 = Animation::createWithSpriteFrames(expFrames2, 0.02f);
	explosion2 = Animate::create(a_exp2);
	explosion2->retain();


	//바리게이트 폭파 애니메이션
	auto s_bari = Sprite::create("explosion/barricade_bomb.png");
	auto t_bari = s_bari->getTexture();
	auto a_bari = Animation::create();
	a_bari->setDelayPerUnit(0.03f);

	for (int i = 0; i < 25; i++)
	{
		int column = i % 5;
		int row = i / 5;
		a_bari->addSpriteFrameWithTexture(t_bari, Rect(column * 66, row * 81, 66, 81));
	}

	barricade_bomb = Animate::create(a_bari);
	barricade_bomb->retain();
	// 슬로우 흔적
	auto s_slow = Sprite::create("explosion/slow_effect.png");
	auto t_slow = s_slow->getTexture();
	auto a_slow = Animation::create();
	a_slow->setDelayPerUnit(0.1f);

	for (int i = 0; i < 10; i++)
	{
		int column = i % 10;
		int row = i / 10;
		a_slow->addSpriteFrameWithTexture(t_slow, Rect(column * 40, row * 32, 40, 32));
	}

	slow_effect = Animate::create(a_slow);
	slow_effect->retain();


	//////////////////////// 도우미 애니메이션



	// 도우미1 Idle 애니메이션

	auto s_helper1_idle = Sprite::create("helper/helper1_idle.png");
	auto t_helper1_idle = s_helper1_idle->getTexture();
	auto a_helper1_idle = Animation::create();
	a_helper1_idle->setDelayPerUnit(0.15f);

	for (int i = 0; i < 6; i++)
	{
		int column = i % 6;
		int row = i / 6;
		a_helper1_idle->addSpriteFrameWithTexture(t_helper1_idle, Rect(column * 56, row * 64, 56, 64));
	}
	helper1_idle = Animate::create(a_helper1_idle);
	helper1_idle->retain();

	//도우미1 공격 애니메이션
	auto s_helper1_attack = Sprite::create("helper/helper1_attack.png");
	auto t_helper1_attack = s_helper1_attack->getTexture();
	auto a_helper1_attack = Animation::create();
	a_helper1_attack->setDelayPerUnit(0.04f);

	for (int i = 0; i < 14; i++)
	{
		int column = i % 4;
		int row = i / 4;
		a_helper1_attack->addSpriteFrameWithTexture(t_helper1_attack, Rect(column * 88, row * 64, 88, 64));
	}

	helper1_attack = Animate::create(a_helper1_attack);
	helper1_attack->retain();


	// 도우미2 Idle 애니메이션

	auto s_helper2_idle = Sprite::create("helper/helper2_idle.png");
	auto t_helper2_idle = s_helper2_idle->getTexture();
	auto a_helper2_idle = Animation::create();
	a_helper2_idle->setDelayPerUnit(0.15f);

	for (int i = 0; i < 8; i++)
	{
		int column = i % 8;
		int row = i / 8;
		a_helper2_idle->addSpriteFrameWithTexture(t_helper2_idle, Rect(column * 72, row * 56, 72, 56));
	}
	helper2_idle = Animate::create(a_helper2_idle);
	helper2_idle->retain();

	//도우미2 공격 애니메이션
	auto s_helper2_attack = Sprite::create("helper/helper2_attack.png");
	auto t_helper2_attack = s_helper2_attack->getTexture();
	auto a_helper2_attack = Animation::create();
	a_helper2_attack->setDelayPerUnit(0.04f);

	for (int i = 0; i < 12; i++)
	{
		int column = i % 7;
		int row = i / 7;
		a_helper2_attack->addSpriteFrameWithTexture(t_helper2_attack, Rect(column * 72, row * 56, 72, 56));
	}

	helper2_attack = Animate::create(a_helper2_attack);
	helper2_attack->retain();

	// 피 애니매이션
	auto s_blood = Sprite::create("player/blood.png");
	auto t_blood = s_blood->getTexture();
	auto a_blood = Animation::create();
	a_blood->setDelayPerUnit(0.07f);

	for (int i = 0; i < 8; i++)
	{
		int column = i % 4;
		int row = i / 4;
		a_blood->addSpriteFrameWithTexture(t_blood, Rect(column * 128, row * 128, 128, 128));
	}

	blood = Animate::create(a_blood);
	blood->retain();

	// ---------------------------------------------------사운드 프리로드

	SimpleAudioEngine::getInstance()->preloadEffect("sounds/intro_backgrund.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sounds/barricade_hit.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sounds/weapon_change.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sounds/sniper.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sounds/monster_die2.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sounds/waveStart.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sounds/monster_hit.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sounds/upgrade.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sounds/menuSelect.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sounds/explosion.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sounds/machine.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sounds/bazooka.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sounds/player_die.wav");	
	SimpleAudioEngine::getInstance()->preloadEffect("sounds/player_die2.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sounds/highLevel.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sounds/barricade_die.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sounds/ak.ogg");
	SimpleAudioEngine::getInstance()->preloadEffect("sounds/item_buy.wav");
}