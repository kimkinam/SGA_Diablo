#include "stdafx.h"
#include "cPlayerIdleState.h"
#include "cPlayer.h"
#include "cActionMove.h"

void cPlayerIdleState::Enter(cPlayer * pOwner)
{
	if (!pOwner) return;

	m_fLastHittedTime = g_pTimeManager->GetTotalSec();
	pOwner->GetMesh()->SetAnimationIndex("idle");
}

void cPlayerIdleState::Execute(cPlayer * pOwner)
{
	if (g_pTimeManager->GetTotalSec() - m_fLastHittedTime > 3.0f)
	{
		pOwner->GetStat().fHp += 5;
	}
}

void cPlayerIdleState::Exit(cPlayer * pOwner)
{
	m_fLastHittedTime = 0.0f;
}

bool cPlayerIdleState::OnMessage(cPlayer * pOwner, const Telegram & msg)
{
	if (!pOwner) return false;
	
	switch (msg.emMessageType)
	{
	case MSG_IDLE:
		//����Ÿ���� �ʱ�ȭ �Ѵ�.
		pOwner->SetTarget(NULL);
		//
		pOwner->m_pSateMachnie->ChangeState(cPlayerIdleState::Instance());
		return true;
		break;
	case MSG_RUN:
	{
		ST_RUN_EXTRAINFO MSG;

		MSG = *(ST_RUN_EXTRAINFO*)msg.ExtraInfo;

		if (!msg.ExtraInfo) return false;

		if (pOwner->GetID() != MSG.nTarget)
		{
			pOwner->SetTarget(g_pAIManager->GetAIBaseFromID(MSG.nTarget));
		}
		else
		{
			pOwner->SetTarget(NULL);
		}

		cActionMove* pAction = new cActionMove;

		//pOwner->SetIsMove(true);
		pAction->SetTo(MSG.vDest);
		pAction->SetFrom(pOwner->GetPosition());
		pAction->SetTarget(pOwner);
		pAction->SetDelegate(pOwner);
		pAction->SetSpeed(0.05f);
		pAction->SetOBB(MSG.vecBox);
		pAction->Start();
		pOwner->SetAction(pAction);
		pOwner->m_pSateMachnie->ChangeState(cPlayerMoveState::Instance());

		return true;
	}
		
	break;
	case MSG_ATTACK:
		pOwner->m_pSateMachnie->ChangeState(cPlayerAttackState::Instance());
		return true;
		break;
	case MSG_NONE:
		break;
	default:
		break;
	}
	return false;
}