#include "PlayerInfoSingleTon.h"

USING_NS_CC;

PlayerInfoSingleTon * PlayerInfoSingleTon::m_pInstance = nullptr;

PlayerInfoSingleTon::PlayerInfoSingleTon() {
	// 공용변수들 초기화

}
PlayerInfoSingleTon::~PlayerInfoSingleTon()
{
	log("플레이어 싱글톤 삭제");
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
