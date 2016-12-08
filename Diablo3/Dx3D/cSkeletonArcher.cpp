#include "stdafx.h"
#include "cSkeletonArcher.h"


cSkeletonArcher::cSkeletonArcher()
{
}


cSkeletonArcher::~cSkeletonArcher()
{
}

void cSkeletonArcher::Setup()
{
	m_stStat = ST_MONSTER_STAT(100, 0, 10, 5, 0.05f, 1.0f, 3.0f);

	cMonster::Setup("SkeletonArcher.x");
}

void cSkeletonArcher::Update()
{
}

void cSkeletonArcher::Render()
{
}

void cSkeletonArcher::OnActionFinish(cAction * pSender)
{
}
