#include "stdafx.h"
#include "cSkeleton.h"
#include "cActionTrace.h"


cSkeleton::cSkeleton()
{
}


cSkeleton::~cSkeleton()
{
}

void cSkeleton::Setup()
{
	m_fSpeed = 0.05f;
	m_fAttackRange = 1.0f;
	m_fTraceRange = 3.0f;
	if (m_pTarget)
		cMonster::Setup("Skeleton");
}

void cSkeleton::Update()
{
	if (m_pTarget)
	{
		cMonster::Update();
	}
	

}

void cSkeleton::Render()
{
	if (m_pTarget)
		cMonster::Render();
}

void cSkeleton::Trace()
{
	if (m_pTarget)
		cMonster::Trace();
}

void cSkeleton::OnActionFinish(cAction * pSender)
{
	cActionTrace* trace = (cActionTrace*)pSender;

	if (trace->GetIsAtk())
		m_emState = MONSTER_ATTACK_START;
	else
		m_emState = MONSTER_IDLE_START;

	m_pAction = NULL;
}
