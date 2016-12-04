#include "stdafx.h"
#include "cBoss.h"
#include "cActionTrace.h"

cBoss::cBoss()
{
}


cBoss::~cBoss()
{
}

void cBoss::Setup()
{
	m_fSpeed = 0.05f;
	m_fAttackRange = 2.0f;
	m_fTraceRange = 5.0f;

	cMonster::Setup("diablo");

<<<<<<< HEAD

=======
	int a = 0;
>>>>>>> 65ab56b83d0f8d2a0d2f65015c3637a9a04789e0
}
void cBoss::Update()
{
	cMonster::Update();

<<<<<<< HEAD
=======
	if (m_pAction)
		m_pAction->Update();
>>>>>>> 65ab56b83d0f8d2a0d2f65015c3637a9a04789e0

}
void cBoss::Render()
{
	cMonster::Render();
}

void cBoss::Trace()
{
<<<<<<< HEAD
	cMonster::Trace();
}

void cBoss::OnActionFinish(cAction * pSender)
{
	cActionTrace* trace = (cActionTrace*)pSender;

	if (trace->GetIsAtk())
		m_emState = MONSTER_ATTACK_START;
	else
		m_emState = MONSTER_IDLE_START;

	m_pAction = NULL;

=======
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
>>>>>>> 65ab56b83d0f8d2a0d2f65015c3637a9a04789e0
}
