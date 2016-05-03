#include "PlayerInfoSingleTon.h"

USING_NS_CC;

PlayerInfoSingleTon * PlayerInfoSingleTon::m_pInstance = nullptr;

PlayerInfoSingleTon::PlayerInfoSingleTon() {
	// 공용변수들 초기화

}

PlayerInfoSingleTon * PlayerInfoSingleTon::getInstance()
{

	if (!m_pInstance)
	{
		m_pInstance = new PlayerInfoSingleTon;
	}
	return m_pInstance;
}

