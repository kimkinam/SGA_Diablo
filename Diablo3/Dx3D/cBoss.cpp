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


}
void cBoss::Update()
{
	cMonster::Update();


}
void cBoss::Render()
{
	cMonster::Render();
}

void cBoss::Trace()
{
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

}
