#include "stdafx.h"
#include "cGameObjectGlobalState.h"
#include "cMonster.h"

void cGameObjectGlobalState::Enter(cGameObject * pOwner)
{
	m_dDeadTime = 0.0f;
	pOwner->SetAnimation("idle");
}

void cGameObjectGlobalState::Execute(cGameObject * pOwner)
{
	if (pOwner->GetStat().fHp <= 0)
	{
		string name = pOwner->GetCurAnimation()->GetName();

		if (name == "stunned")
		{
			m_dDeadTime -= g_pTimeManager->GetDeltaTime();
			if(m_dDeadTime < 0)
			{
				g_pAIManager->RemoveAIBase(pOwner);
				this->Exit(pOwner);
			}
		}
		else
		{
			pOwner->SetAnimation("stunned");

			double totalTime = pOwner->GetCurAniTime();
			g_pMessageManager->MessageSend(0.0f, pOwner->GetID(), pOwner->GetID(),
				MESSAGE_TYPE::MSG_DEAD, &(double)totalTime);
		}
	}

}

void cGameObjectGlobalState::Exit(cGameObject * pOwner)
{
}

bool cGameObjectGlobalState::OnMessage(cGameObject* pOwner, const Telegram& msg)
{
	//if (pOwner->GetStat().fHp <= 0) return false;
	switch (msg.emMessageType)
	{
		case MSG_RUN:
		break;
		case MSG_IDLE:
		break;
		case MSG_ATTACK:
		break;
		case MSG_HITTED:
		{
			float hp = pOwner->GetStat().fHp;
			float atk = g_pAIManager->GetAIBaseFromID(msg.nSender)->GetStat().fAtk;
			pOwner->GetStat().fHp -= g_pAIManager->GetAIBaseFromID(msg.nSender)->GetStat().fAtk;
		}
		return true;
		break;
		case MSG_DEAD:
			m_dDeadTime = *(double*)msg.ExtraInfo;
			return true;
			break;
		case MSG_NONE:
		break;
	default:
		break;
	}

	return false;
}