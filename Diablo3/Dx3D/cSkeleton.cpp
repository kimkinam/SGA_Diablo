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
	m_stStat = ST_MONSTER_STAT(100, 0, 10, 5, 0.05f, 1.5f, 3.0f);

	cMonster::Setup("Skeleton.x");
}

void cSkeleton::Update()
{
	cMonster::Update();
}

void cSkeleton::Render()
{
	cMonster::Render();
}


void cSkeleton::OnActionFinish(cAction * pSender)
{
	cMonster::OnActionFinish(pSender);
}
