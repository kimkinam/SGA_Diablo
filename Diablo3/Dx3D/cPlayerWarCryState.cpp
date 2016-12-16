#include "stdafx.h"
#include "cPlayerWarCryState.h"
#include "cPlayerMoveState.h"
#include "cPlayer.h"
#include "cActionMove.h"
#include "cPlayerIdleState.h"


void cPlayerWarCryState::Enter(cPlayer * pOwner)
{
	if (!pOwner) return;
	pOwner->GetMesh()->SetAnimationIndex("warcry");
	SOUNDMANAGER->play("WarCay", 0.6f);


}

void cPlayerWarCryState::Execute(cPlayer * pOwner)
{
	LPD3DXANIMATIONSET pCurAS = NULL;
	pOwner->GetMesh()->GetAnimController()->GetTrackAnimationSet(0, &pCurAS);

	if (pCurAS)
	{
		D3DXTRACK_DESC td;
		pOwner->GetMesh()->GetAnimController()->GetTrackDesc(0, &td);

		if (td.Position > pCurAS->GetPeriod() - EPSILON - 0.2f)
		{
			pOwner->SetAction(NULL);
			//SOUNDMANAGER->stop("WarCay");
			pOwner->m_pSateMachnie->ChangeState(cPlayerIdleState::Instance());
		}
	}
}

void cPlayerWarCryState::Exit(cPlayer * pOwner)
{

}

//매세지를 처리한다.
bool cPlayerWarCryState::OnMessage(cPlayer * pOwner, const Telegram & msg)
{
	switch (msg.emMessageType)
	{
	case MSG_WARCRY:
		pOwner->m_pSateMachnie->ChangeState(cPlayerWarCryState::Instance());
		break;
	case MSG_IDLE:
		pOwner->m_pSateMachnie->ChangeState(cPlayerIdleState::Instance());
		break;
	case MSG_ATTACK:
		pOwner->m_pSateMachnie->ChangeState(cPlayerAttackState::Instance());
		break;
	default:
		break;
	}
	return true;
}

