#include "MonsterInfoSingleTon.h"

USING_NS_CC;

MonsterInfoSingleTon * MonsterInfoSingleTon::m_pInstance = nullptr;

MonsterInfoSingleTon::MonsterInfoSingleTon() {
	// ���뺯���� �ʱ�ȭ
	
}

MonsterInfoSingleTon * MonsterInfoSingleTon::getInstance()
{

	if (!m_pInstance)
	{
		m_pInstance = new MonsterInfoSingleTon;
	}
	return m_pInstance;
}
