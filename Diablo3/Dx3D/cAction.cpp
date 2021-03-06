#include "stdafx.h"
#include "cAction.h"


cAction::cAction()
	: m_fPassedTime(0.0f)
	, m_fActionTime(0.0f)
	, m_pTarget(NULL)
	, m_pDelegate(NULL)
	, m_vDirection(0, 0, 0)
	, m_fDistance(0.0f)
{
}


cAction::~cAction()
{
}

void cAction::Start()
{
	m_fPassedTime = 0.0f;
}

void cAction::Update()
{
	m_fPassedTime += g_pTimeManager->GetDeltaTime();
}

cAction* cAction::Create()
{
	return new cAction();
}
