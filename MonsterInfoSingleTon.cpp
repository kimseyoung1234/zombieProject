#include "MonsterInfoSingleTon.h"

USING_NS_CC;

MonsterInfoSingleTon * MonsterInfoSingleTon::m_pInstance = nullptr;

MonsterInfoSingleTon::MonsterInfoSingleTon() {
	// ���뺯���� �ʱ�ȭ
	
}

MonsterInfoSingleTon::~MonsterInfoSingleTon()
{
	log("���ͽ̱��� ����");
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

// �������� Ŭ�����ϸ� ������
void MonsterInfoSingleTon::level_up()
{
	this->level++;
	// ���� ���� �� ������ ���� ��ü �� 2���� ���� 
	maxMonster = maxMonster + 2;
	// ������ 3�� ��� �� ������ ���� ���� ����
	if (level % 3 == 0)
	{
		// ��ġ�� ���߿� ����
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