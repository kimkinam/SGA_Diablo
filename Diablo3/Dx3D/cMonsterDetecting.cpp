#include "stdafx.h"
#include "cMonsterDetecting.h"
#include "cMonster.h"
#include "cMonsterTrace.h"
#include "cActionMove.h"


void cMonsterDetecting::Enter(cMonster * pOwner)
{
	
	pOwner->GetMesh()->SetAnimationIndex("idle");
	

}

void cMonsterDetecting::Execute(cMonster * pOwner)
{
	if (!pOwner->GetTarget()) return;
	
	D3DXVECTOR3 vLength = pOwner->GetTarget()->GetPosition()
		- pOwner->GetPosition();
	float distance = D3DXVec3Length(&vLength);

	if (distance < pOwner->GetStat().fTraceRange)
	{
		this->Exit(pOwner);
		g_pMessageManager->MessageSend(0.0f, pOwner->GetID(), pOwner->GetID(),
			MESSAGE_TYPE::MSG_RUN, NULL);
	}
}

void cMonsterDetecting::Exit(cMonster * pOwner)
{
	if (!pOwner->GetTarget()) return;
	D3DXVECTOR3 vLength = pOwner->GetTarget()->GetPosition()
		- pOwner->GetPosition();

	D3DXVECTOR3 vDir;
	D3DXVec3Normalize(&vDir, &vLength);
	pOwner->SetNewDirection(vDir);
}

bool cMonsterDetecting::OnMessage(cMonster* pOwner, const Telegram& msg)
{
	if (pOwner->GetStat().fHp <= 0) return false;

	switch (msg.emMessageType)
	{
	case MSG_RUN:
	{
		pOwner->m_pSateMachnie->ChangeState(cMonsterTrace::Instance());
		SOUNDMANAGER->play("SkeletonAttack", 0.5f);
		return true;
	}
		break;
	case MSG_ATTACK:
		break;
	case MSG_HITTED:
		return false;
		break;
	case MSG_NONE:
		break;
	default:
		break;
	}

	return false;
}