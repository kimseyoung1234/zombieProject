#include "ResouceLoad.h"

USING_NS_CC;

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
	auto s_player_idle = Sprite::create("player_idle.png");
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
	auto s_player_attack = Sprite::create("player_attack.png");
	auto t_player_attack = s_player_attack->getTexture();
	auto a_player_attack = Animation::create();
	a_player_attack->setDelayPerUnit(0.05f);

	for (int i = 0; i < 8; i++)
	{
		int column = i % 4;
		int row = i / 4;
		a_player_attack->addSpriteFrameWithTexture(t_player_attack, Rect(column * 136, row * 72, 136, 72));
	}
	player_attackAnimate = Animate::create(a_player_attack);
	player_attackAnimate->retain();

	//////////////////////////// 몬스터 애니메이션

	// 브레인좀비 공격 애니메이션
	auto s_brain_attack = Sprite::create("monster/brain_attack.png");
	auto t_brain_attack = s_brain_attack->getTexture();
	auto a_brain_attack = Animation::create();
	a_brain_attack->setDelayPerUnit(0.074f);

	for (int i = 0; i < 27; i++)
	{
		int column = i % 15;
		int row = i / 15;
		a_brain_attack->addSpriteFrameWithTexture(t_brain_attack, Rect(column * 64, row * 64, 64, 64));
	}

	brain_attackAnimate = Animate::create(a_brain_attack);
	brain_attackAnimate->retain();

	// 브레인좀비 이동 애니메이션
	auto s_brain_move = Sprite::create("monster/brain_move.png");
	auto t_brain_move = s_brain_move->getTexture();
	auto a_brain_move = Animation::create();
	a_brain_move->setDelayPerUnit(0.05f);

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
	a_fat_attack->setDelayPerUnit(0.074f);

	for (int i = 0; i < 18; i++)
	{
		int column = i % 7;
		int row = i / 7;
		a_fat_attack->addSpriteFrameWithTexture(t_fat_attack, Rect(column * 48, row * 48, 48, 48));
	}

	fat_attackAnimate = Animate::create(a_fat_attack);
	fat_attackAnimate->retain();

	// 팻좀비 이동 애니메이션
	auto s_fat_move = Sprite::create("monster/fat_move.png");
	auto t_fat_move = s_fat_move->getTexture();
	auto a_fat_move = Animation::create();
	a_fat_move->setDelayPerUnit(0.05f);

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
	a_super_attack->setDelayPerUnit(0.074f);

	for (int i = 0; i < 20; i++)
	{
		int column = i % 7;
		int row = i / 7;
		a_super_attack->addSpriteFrameWithTexture(t_super_attack, Rect(column * 64, row * 64, 64, 64));
	}

	super_attackAnimate = Animate::create(a_super_attack);
	super_attackAnimate->retain();
	
	//슈퍼좀비 이동 애니메이션
	auto s_super_move = Sprite::create("monster/super_move.png");
	auto t_super_move = s_super_move->getTexture();
	auto a_super_move = Animation::create();
	a_super_move->setDelayPerUnit(0.05f);

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

	//////////////////////// 도우미 애니메이션

	// 도우미1 Idle 애니메이션

	auto s_helper1_idle = Sprite::create("helper1_idle.png");
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
	auto s_helper1_attack = Sprite::create("helper1_attack.png");
	auto t_helper1_attack = s_helper1_attack->getTexture();
	auto a_helper1_attack = Animation::create();
	a_helper1_attack->setDelayPerUnit(0.04f);

	for (int i = 0; i < 12; i++)
	{
		int column = i % 4;
		int row = i / 4;
		a_helper1_attack->addSpriteFrameWithTexture(t_helper1_attack, Rect(column * 56, row * 64, 56, 64));
	}

	helper1_attack = Animate::create(a_helper1_attack);
	helper1_attack->retain();

}