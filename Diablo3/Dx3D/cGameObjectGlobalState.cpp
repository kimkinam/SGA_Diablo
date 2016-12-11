#include "stdafx.h"
#include "cGameObjectGlobalState.h"
#include "cMonster.h"

void cGameObjectGlobalState::Enter(cGameObject * pOwner)
{
	pOwner->GetMesh()->SetAnimationIndex("idle");
}

void cGameObjectGlobalState::Execute(cGameObject * pOwner)
{
	//assert(pOwner->GetStat().fHp > 0 && "ав╬З╫ю╢о╢ы.");

	if (pOwner->GetStat().fHp < 0)
	{
		//SAFE_RELEASE(pOwner);
		g_pAIManager->RemoveAIBase(pOwner);
		
	}
}

void cGameObjectGlobalState::Exit(cGameObject * pOwner)
{
}

bool cGameObjectGlobalState::OnMessage(cGameObject* pOwner, const Telegram& msg)
{
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
		break;
		case MSG_NONE:
		break;
	default:
		break;
	}

	return false;
}