#include "stdafx.h"
#include "cPlayerIdleState.h"
#include "cPlayer.h"
#include "cActionMove.h"
#include "cPlayerWarCryState.h"
#include "cWhirlwindState.h"

void cPlayerIdleState::Enter(cPlayer * pOwner)
{
	if (!pOwner) return;

	m_fLastHittedTime = g_pTimeManager->GetTotalSec();
	//if(pOwner->IsDoneCurAni())
		pOwner->GetMesh()->SetAnimationIndex("idle");
}

void cPlayerIdleState::Execute(cPlayer * pOwner)
{
	//if (g_pTimeManager->GetTotalSec() - m_fLastHittedTime > 3.0f)
	//{
	//	if (pOwner->GetStat().fHp > pOwner->GetStat().fMaxHp) return;
	//	pOwner->GetStat().fHp += 1;
	//}
}

void cPlayerIdleState::Exit(cPlayer * pOwner)
{
	m_fLastHittedTime = 0.0f;
}

bool cPlayerIdleState::OnMessage(cPlayer * pOwner, const Telegram & msg)
{
	if (!pOwner) return false;
	if (msg.emMessageType == MSG_HITTED) return false;
	switch (msg.emMessageType)
	{
	case MSG_IDLE:
		//공격타겟을 초기화 한다.
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
		//pAction->SetOBB(MSG.vecBox);
		pAction->SetOBB(pOwner->GetBoundBox());
		pAction->Start();
		pOwner->SetAction(pAction);
		pOwner->m_pSateMachnie->ChangeState(cPlayerMoveState::Instance());

		//return true;
	}
	return true;
	break;
	case MSG_WHIRLWIND:
	{
		ST_RUN_EXTRAINFO message;
		message = *(ST_RUN_EXTRAINFO*)msg.ExtraInfo;

		cActionMove* pAction = new cActionMove;
		SOUNDMANAGER->stop("FootStep");
		SOUNDMANAGER->play("FootStep", 0.2f);
		pAction->SetTo(message.vDest);
		pAction->SetFrom(pOwner->GetPosition());
		pAction->SetTarget(pOwner);
		pAction->SetDelegate(pOwner);
		pAction->SetSpeed(0.05f);
		pAction->SetOBB(pOwner->GetBoundBox());
		pAction->Start();
		pOwner->SetAction(pAction);

		pOwner->m_pSateMachnie->ChangeState(cWhirlwindState::Instance());
	}
	return true;
		break;
	case MSG_ATTACK:
		if (!pOwner->GetTarget()) return false;
		pOwner->m_pSateMachnie->ChangeState(cPlayerAttackState::Instance());
		return true;
		break;
	case MSG_NONE:
		break;
	case MSG_WARCRY:
		pOwner->m_pSateMachnie->ChangeState(cPlayerWarCryState::Instance());
		break;
	default:
		break;
	}

	return false;
}
