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
}
void cBoss::Update()
{
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

	m_pMesh->SetAnimationIndex("idle");
}



