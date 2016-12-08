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
	
	m_stStat = ST_MONSTER_STAT(100, 0, 10, 5, 0.05f, 2.0f, 5.0f);


	cMonster::Setup("diablo", vLookAt);

}
void cBoss::Update()
{
	cMonster::Update();
	
}
void cBoss::Render()
{
	cMonster::Render();
}


void cBoss::OnActionFinish(cAction * pSender)
{
	cMonster::OnActionFinish(pSender);

}
