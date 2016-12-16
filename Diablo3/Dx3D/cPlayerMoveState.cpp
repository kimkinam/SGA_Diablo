#include "stdafx.h"
#include "cPlayerMoveState.h"
#include "cPlayer.h"
#include "cActionMove.h"
#include "cPlayerIdleState.h"
#include "cPlayerWarCryState.h"
void cPlayerMoveState::Enter(cPlayer * pOwner)
{
	if (!pOwner) return;
	
	pOwner->GetMesh()->SetAnimationIndex("run");
	SOUNDMANAGER->play("FootStep", 0.3f);

}

void cPlayerMoveState::Execute(cPlayer * pOwner)
{
	//���������� �������ٸ� �ڱ��ڽſ��� �ٽ� �ż����� ������.
	if (!pOwner->GetAction())
	{
		if (pOwner->GetTarget())
		{
			SOUNDMANAGER->stop("FootStep");
			g_pMessageManager->MessageSend(0.0f, pOwner->GetID(), pOwner->GetID(),
				MESSAGE_TYPE::MSG_ATTACK, NULL);
		}
			
		else
		{
			SOUNDMANAGER->stop("FootStep");
			g_pMessageManager->MessageSend(0.0f, pOwner->GetID(), pOwner->GetID(),
				MESSAGE_TYPE::MSG_IDLE, NULL);
		}
	}
}

void cPlayerMoveState::Exit(cPlayer * pOwner)
{
}

//�ż����� ó���Ѵ�.
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
			{
				//SAFE_ADDREF(g_pAIManager->GetAIBaseFromID(MSG.nTarget));
				pOwner->SetTarget(g_pAIManager->GetAIBaseFromID(MSG.nTarget));
			}
				
			else
				pOwner->SetTarget(NULL);

			cActionMove* pAction = new cActionMove;
			SOUNDMANAGER->stop("FootStep");
			SOUNDMANAGER->play("FootStep", 0.2f);
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
			pOwner->m_pSateMachnie->ChangeState(cPlayerAttackState::Instance());
		break;
		case MSG_WARCRY:
		{
			pOwner->m_pSateMachnie->ChangeState(cPlayerWarCryState::Instance());
		}
		break;
		default:
		break;
	}
	return true;
}
