#include "stdafx.h"
#include "cPlayerMoveState.h"
#include "cPlayer.h"
#include "cActionMove.h"
#include "cPlayerIdleState.h"

void cPlayerMoveState::Enter(cPlayer * pOwner)
{
	if (!pOwner) return;
	pOwner->GetMesh()->SetAnimationIndex("run");
}

void cPlayerMoveState::Execute(cPlayer * pOwner)
{
	//목적지까지 움직였다면 자기자신에게 다시 매세지를 보낸다.
	if (!pOwner->GetAction())
	{
		if (pOwner->GetTarget())
		{
			g_pMessageManager->MessageSend(0.0f, pOwner->GetID(), pOwner->GetID(),
				MESSAGE_TYPE::MSG_ATTACK, NULL);
		}
			
		else
			g_pMessageManager->MessageSend(0.0f, pOwner->GetID(), pOwner->GetID(),
				MESSAGE_TYPE::MSG_IDLE, NULL);
	}
}

void cPlayerMoveState::Exit(cPlayer * pOwner)
{
	//pOwner->SetTarget(NULL);
}

//매세지를 처리한다.
bool cPlayerMoveState::OnMessage(cPlayer * pOwner, const Telegram & msg)
{
	switch (msg.emMessageType)
	{
		case MSG_RUN:
		{
			if (!msg.ExtraInfo) return false;

			ST_RUN_EXTRAINFO MSG;

			MSG = *(ST_RUN_EXTRAINFO*)msg.ExtraInfo;

			if (MSG.nTarget != pOwner->GetID())
				pOwner->SetTarget(g_pAIManager->GetAIBaseFromID(MSG.nTarget));
			else
				pOwner->SetTarget(NULL);

			cActionMove* pAction = new cActionMove;
			pAction->SetTo(MSG.vDest);
			pAction->SetFrom(pOwner->GetPosition());
			pAction->SetTarget(pOwner);
			pAction->SetDelegate(pOwner);
			pAction->SetSpeed(0.05f);
			pAction->SetOBB(MSG.vecBox);
			pAction->Start();
			pOwner->SetAction(pAction);
		}
	
		break;
		case MSG_IDLE:
			pOwner->m_pSateMachnie->ChangeState(cPlayerIdleState::Instance());
		break;
		case MSG_ATTACK:
		{
			pOwner->m_pSateMachnie->ChangeState(cPlayerAttackState::Instance());

		}
		break;
		default:
		break;
	}

	return true;
}
