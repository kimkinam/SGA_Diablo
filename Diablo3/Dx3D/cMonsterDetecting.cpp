#include "stdafx.h"
#include "cMonsterDetecting.h"
#include "cMonster.h"
#include "cMonsterTrace.h"


void cMonsterDetecting::Enter(cMonster * pOwner)
{
	if (pOwner)
	{
		pOwner->GetMesh()->SetAnimationIndex("idle");
	}
}

void cMonsterDetecting::Execute(cMonster * pOwner)
{
	D3DXVECTOR3 vLength = pOwner->GetTarget()->GetPosition()
		- pOwner->GetPosition();

	float distance = D3DXVec3Length(&vLength);

	if (distance < pOwner->GetStat().fTraceRange)
	{
		pOwner->m_pSateMachnie->ChangeState(cMonsterTrace::Instance());
	}
}

void cMonsterDetecting::Exit(cMonster * pOwner)
{
}
