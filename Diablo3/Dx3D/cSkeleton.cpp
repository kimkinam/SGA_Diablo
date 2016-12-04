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

	cMonster::Setup("Skeleton");
}

void cSkeleton::Update()
{
	cMonster::Update();
}

void cSkeleton::Render()
{
	cMonster::Render();
}

void cSkeleton::Trace()
{
	cMonster::Trace();
}

void cSkeleton::OnActionFinish(cAction * pSender)
{
	cMonster::OnActionFinish(pSender);
}
