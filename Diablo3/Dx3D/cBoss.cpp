#include "stdafx.h"
#include "cBoss.h"
#include "cActionTrace.h"

cBoss::cBoss()
{
}


cBoss::~cBoss()
{
}

void cBoss::Setup(D3DXVECTOR3* vLookAt)
{
	m_fSpeed = 0.05f;
	m_fAttackRange = 2.0f;
	m_fTraceRange = 5.0f;

	cMonster::Setup("diablo", vLookAt);

	m_stStat.fHp = 1000.0f;
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
	cMonster::OnActionFinish(pSender);

}
