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
	//if (!pOwner->GetTarget()) return;
	//
	//D3DXVECTOR3 vLength = pOwner->GetTarget()->GetPosition()
	//	- pOwner->GetPosition();
	//
	//float distance = D3DXVec3Length(&vLength);
	//
	//if (distance < pOwner->GetStat().fTraceRange)
	//{
	//	pOwner->m_pSateMachnie->ChangeState(cMonsterTrace::Instance());
	//}
}

void cMonsterDetecting::Exit(cMonster * pOwner)
{
}

bool cMonsterDetecting::OnMessage(cMonster* pOwner, const Telegram& msg)
{
	switch (msg.Msg)
	{
	case MSG_RUN:
		// 青悼 贸府
		return true;
		break;
	case MSG_NONE:
		// 青悼 贸府
		return true;
		break;
	}

	return false;
}