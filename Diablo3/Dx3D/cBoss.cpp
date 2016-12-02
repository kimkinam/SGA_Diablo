#include "stdafx.h"
#include "cBoss.h"
#include "cSkinnedMesh.h"


cBoss::cBoss()
{
}


cBoss::~cBoss()
{
}

void cBoss::Setup()
{
	cMonster::Setup("diablo");

	int a = 0;
}
void cBoss::Update()
{
	cMonster::Update();

	if (m_pAction)
		m_pAction->Update();

}
void cBoss::Render()
{
	cMonster::Render();
}

void cBoss::OnActionFinish(cAction * pSender)
{
	m_pAction = NULL;
	
	if (pSender->GetPassedTime() < m_fTraceTime)
	{
		m_pMesh->SetAnimationIndex("attack");
		m_emState = MONSTER_SKILL1;
	}
	else
	{
		m_pMesh->SetAnimationIndex("idle");
		m_emState = MONSTER_IDLE;
	}
		
}

void cBoss::Trace()
{
	cMonster::Trace();
}
