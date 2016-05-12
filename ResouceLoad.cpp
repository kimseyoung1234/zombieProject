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
}