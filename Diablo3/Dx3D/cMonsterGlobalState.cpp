#include "stdafx.h"
#include "cMonsterGlobalState.h"
#include "cMonster.h"

void cMonsterGlobalState::Enter(cGameObject * pOwner)
{
}

void cMonsterGlobalState::Execute(cGameObject * pOwner)
{
	//assert(pOwner->GetStat().fHp > 0 && "죽었습니다.");

	
}

void cMonsterGlobalState::Exit(cGameObject * pOwner)
{
}

bool cMonsterGlobalState::OnMessage(cGameObject* pOwner, const Telegram& msg)
{
	switch (msg.Msg)
	{
	case MSG_RUN:
		// 행동 처리
		return true;
		break;
	case MSG_NONE:
		// 행동 처리
		return true;
		break;
	}

	return false;
}