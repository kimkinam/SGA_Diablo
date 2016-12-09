#include "stdafx.h"
#include "cPlayerMove.h"
#include "cPlayer.h"
#include "cActionMove.h"
#include "cPlayerIdleState.h"

void cPlayerMove::Enter(cPlayer * pOwner)
{
	if (!pOwner) return;

	pOwner->GetMesh()->SetAnimationIndex("run");

}

void cPlayerMove::Execute(cPlayer * pOwner)
{
	//충돌처리
	if (!pOwner->GetAction())
	{
		g_pMessageManager->MessageSend(0.0f, pOwner->GetID(), pOwner->GetID(), MESSAGE_TYPE::MSG_IDLE);
	}
		
}

void cPlayerMove::Exit(cPlayer * pOwner)
{

}

bool cPlayerMove::OnMessage(cPlayer * pOwner, const Telegram & msg)
{
	switch (msg.Msg)
	{
		case MSG_RUN:
		{
			if (!msg.ExtraInfo) return false;

			struct extramsg
			{
				UINT				nBox;
				std::vector<cOBB*>	bound;
				D3DXVECTOR3			vPickPos;
			};

			extramsg MSG = *(extramsg*)msg.ExtraInfo;

			cActionMove* pAction = new cActionMove;

			pOwner->SetIsMove(true);
			pAction->SetTo(MSG.vPickPos);
			pAction->SetFrom(pOwner->GetPosition());
			pAction->SetTarget(pOwner);
			pAction->SetDelegate(pOwner);
			pAction->SetSpeed(0.05f);
			pAction->SetOBB(MSG.bound);
			pAction->Start();
			pOwner->SetAction(pAction);
		}
		break;
		case MSG_IDLE:
			pOwner->m_pSateMachnie->ChangeState(cPlayerIdleState::Instance());
		break;
		case MSG_NONE:
		break;
		default:
		break;
	}

	return false;
}
