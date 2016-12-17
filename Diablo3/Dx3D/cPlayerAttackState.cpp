#include "stdafx.h"
#include "cPlayerAttackState.h"
#include "cPlayer.h"
#include "cPlayerWarCryState.h"

void cPlayerAttackState::Enter(cPlayer * pOwner)
{
	if (!pOwner) return;
	if (!pOwner->GetTarget()) return;
	SOUNDMANAGER->play("SwordSwing", 0.8f);
	pOwner->GetMesh()->SetAnimationIndex("attack");
	int a = 0;
	double totalTime = pOwner->GetCurAniTime();
	pOwner->GetTarget()->SetIsHit(true);

	ST_HIT_EXTRAINFO msgHit;
	msgHit.dwHitType = 1;
	msgHit.fDamage = pOwner->GetStat().fAtk;

	

	g_pMessageManager->MessageSend(totalTime / 2, pOwner->GetID(), pOwner->GetTarget()->GetID(),
		MESSAGE_TYPE::MSG_HITTED, &msgHit);

}


void cPlayerAttackState::Execute(cPlayer * pOwner)
{
	if (!pOwner->GetTarget()) return;
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
	//pOwner->GetTarget()->Release();
	pOwner->SetTarget(NULL);
}

bool cPlayerAttackState::OnMessage(cPlayer * pOwner, const Telegram & msg)
{
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
	case MSG_WARCRY:
		pOwner->m_pSateMachnie->ChangeState(cPlayerWarCryState::Instance());
		break;
	case MSG_NONE:
		break;
	default:
		break;
	}

	return false;
}
