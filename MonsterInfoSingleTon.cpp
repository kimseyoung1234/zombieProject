#include "MonsterInfoSingleTon.h"

USING_NS_CC;

MonsterInfoSingleTon * MonsterInfoSingleTon::m_pInstance = nullptr;

MonsterInfoSingleTon::MonsterInfoSingleTon() {
	// 공용변수들 초기화
	
}

MonsterInfoSingleTon::~MonsterInfoSingleTon()
{
	log("몬스터싱글톤 삭제");
}
MonsterInfoSingleTon * MonsterInfoSingleTon::getInstance()
{

	if (!m_pInstance)
	{
		m_pInstance = new MonsterInfoSingleTon;
	}
	return m_pInstance;
}


void MonsterInfoSingleTon::FreeInstance()
{
	if (m_pInstance != nullptr)
	{

		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

// 스테이지 클리어하면 레벨업
void MonsterInfoSingleTon::level_up()
{
	this->level++;
	// 레벨 증가 할 때마다 몬스터 개체 수 2마리 증가 
	maxMonster = maxMonster + 2;
	// 레벨이 3의 배수 일 때마다 몬스터 스텟 증가
	if (level % 3 == 0)
	{
		// 수치는 나중에 조정
		fatZombie_HP = fatZombie_HP + 10;
	//	fatZombie_xSpeed = 0.05;
	//	fatZomie_ySpeed = 0;

		brainZombie_HP = brainZombie_HP + 10;
	//	brainZombie_xSpeed = 0.06;
	//	brainZomie_ySpeed = 0.03;

		superZombie_HP = superZombie_HP + 10;
	//	superZombie_xSpeed = 0.07;
	//	superZomie_ySpeed = 0;
	}
}