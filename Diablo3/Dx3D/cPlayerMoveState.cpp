#include "stdafx.h"
#include "cPlayerMoveState.h"
#include "cPlayer.h"
#include "cActionMove.h"
#include "cPlayerIdleState.h"
#include "cPlayerWarCryState.h"
#include "cWhirlwindState.h"
#include "cMonster.h"

void cPlayerMoveState::Enter(cPlayer * pOwner)
{
	if (!pOwner) return;
	
	//if (pOwner->m_bIsWhirl)
	//{
	//	pOwner->GetMesh()->SetAnimationIndex("whirlwinding");
	//}
	//else
	//{
		pOwner->GetMesh()->SetAnimationIndex("run");
		SOUNDMANAGER->play("FootStep", 0.4f);
	//}

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
		{
			//SOUNDMANAGER->stop("FootStep");
			g_pMessageManager->MessageSend(0.0f, pOwner->GetID(), pOwner->GetID(),
				MESSAGE_TYPE::MSG_IDLE, NULL);
		}
	}
}

void cPlayerMoveState::Exit(cPlayer * pOwner)
{
	SOUNDMANAGER->stop("FootStep");
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
			{
				//SAFE_ADDREF(g_pAIManager->GetAIBaseFromID(MSG.nTarget));
				pOwner->SetTarget(g_pAIManager->GetAIBaseFromID(MSG.nTarget));
			}
				
			else
				pOwner->SetTarget(NULL);

			cActionMove* pAction = new cActionMove;
			//SOUNDMANAGER->stop("FootStep");
			if (!SOUNDMANAGER->isPlaySound("FootStep"))
				SOUNDMANAGER->play("FootStep", 0.4f);
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
			return true;
		break;
		case MSG_ATTACK:
			pOwner->m_pSateMachnie->ChangeState(cPlayerAttackState::Instance());
			return true;
		break;
		case MSG_WARCRY:
			pOwner->m_pSateMachnie->ChangeState(cPlayerWarCryState::Instance());
			return true;
		break;
		case MSG_WHIRLWIND:
		{
			ST_RUN_EXTRAINFO message;
			message = *(ST_RUN_EXTRAINFO*)msg.ExtraInfo;
			//
			//message.fSpeed = pOwner->GetStat().fSpeed;
			//
			//std::map < int, cGameObject* > m_mapMonster;
			//std::map<int, cGameObject*>::iterator m_mapMonsterIter;
			//std::vector<cOBB*> vecMonsterOBB;
			//
			//for (m_mapMonsterIter = m_mapMonster.begin(); m_mapMonsterIter != m_mapMonster.end(); ++m_mapMonsterIter)
			//{
			//	cMonster* monster = (cMonster*)(m_mapMonsterIter)->second;
			//	vecMonsterOBB.push_back(monster->GetOBB());
			//}
			//
			//message.nBoxCount = vecMonsterOBB.size();
			//message.vecBox = vecMonsterOBB;

			cActionMove* pAction = new cActionMove;
		
			//SOUNDMANAGER->stop("FootStep");
			//SOUNDMANAGER->play("FootStep", 0.2f);
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
		default:
		break;
	}
	return true;
}
