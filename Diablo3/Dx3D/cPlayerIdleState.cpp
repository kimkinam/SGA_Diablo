#include "stdafx.h"
#include "cPlayerIdleState.h"
#include "cPlayer.h"
#include "cPlayerMove.h"
#include "cActionMove.h"

void cPlayerIdleState::Enter(cPlayer * pOwner)
{
	if (!pOwner) return;

	pOwner->GetMesh()->SetAnimationIndex("idle");
}

void cPlayerIdleState::Execute(cPlayer * pOwner)
{
}

void cPlayerIdleState::Exit(cPlayer * pOwner)
{
}

bool cPlayerIdleState::OnMessage(cPlayer * pOwner, const Telegram & msg)
{
	if (!pOwner) return false;

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
		pOwner->m_pSateMachnie->ChangeState(cPlayerMove::Instance());
	}
		
		break;
	case MSG_NONE:
		break;
	default:
		break;
	}
	return false;
}
