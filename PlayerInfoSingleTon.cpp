#include "PlayerInfoSingleTon.h"

USING_NS_CC;

PlayerInfoSingleTon * PlayerInfoSingleTon::m_pInstance = nullptr;

PlayerInfoSingleTon::PlayerInfoSingleTon() {
	// ���뺯���� �ʱ�ȭ

}
PlayerInfoSingleTon::~PlayerInfoSingleTon()
{
	log("�÷��̾� �̱��� ����");
}

PlayerInfoSingleTon * PlayerInfoSingleTon::getInstance()
{

	if (!m_pInstance)
	{
		m_pInstance = new PlayerInfoSingleTon;
	}
	return m_pInstance;
}

void PlayerInfoSingleTon::FreeInstance()
{
	if (m_pInstance != nullptr)
	{

		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
///////
