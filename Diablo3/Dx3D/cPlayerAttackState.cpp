#include "stdafx.h"
#include "cPlayerAttackState.h"
#include "cPlayer.h"

void cPlayerAttackState::Enter(cPlayer * pOwner)
{
	if (!pOwner) return;
	if (!pOwner->GetTarget()) return;

	pOwner->GetMesh()->SetAnimationIndex("attack");

	double totalTime = pOwner->GetCurAniTime();
	g_pMessageManager->MessageSend(totalTime / 2, pOwner->GetID(), pOwner->GetTarget()->GetID(),
		MESSAGE_TYPE::MSG_HITTED, &(float)pOwner->GetStat().fAtk);
}

void cPlayerAttackState::Execute(cPlayer * pOwner)
{
	D3DXVECTOR3 vDir = pOwner->GetTarget()->GetPosition() - pOwner->GetPosition();
	D3DXVec3Normalize(&vDir, &vDir);

	pOwner->SetNewDirection(vDir);
	if (pOwner->IsDoneCurAni())
	{
		
		g_pMessageManager->MessageSend(0.0f, pOwner->GetID(),
			pOwner->GetID(), MESSAGE_TYPE::MSG_IDLE);
	}
		
}

void cPlayerAttackState::Exit(cPlayer * pOwner)
{
	pOwner->SetTarget(NULL);
}

bool cPlayerAttackState::OnMessage(cPlayer * pOwner, const Telegram & msg)
{
	//if  return false;

	switch (msg.emMessageType)
	{
	case MSG_RUN:
		if (pOwner->IsDoneCurAni())
		pOwner->m_pSateMachnie->ChangeState(cPlayerMoveState::Instance());
		return true;
	break;
	case MSG_ATTACK:
		if (pOwner->IsDoneCurAni())
			pOwner->m_pSateMachnie->ChangeState(cPlayerAttackState::Instance());
		return true;
		break;
	case MSG_IDLE:
		if (pOwner->IsDoneCurAni())
		pOwner->m_pSateMachnie->ChangeState(cPlayerIdleState::Instance());
		return true;
		break;
	case MSG_NONE:
		break;
	default:
		break;
	}

	return false;
}
