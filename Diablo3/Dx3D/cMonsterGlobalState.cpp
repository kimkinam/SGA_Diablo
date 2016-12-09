#include "stdafx.h"
#include "cMonsterGlobalState.h"
#include "cMonster.h"

void cMonsterGlobalState::Enter(cMonster * pOwner)
{
}

void cMonsterGlobalState::Execute(cMonster * pOwner)
{
	assert(pOwner->GetStat().fHp > 0 && "죽었습니다.");

	
}

void cMonsterGlobalState::Exit(cMonster * pOwner)
{
}

bool cMonsterGlobalState::OnMessage(cMonster* pOwner, const Telegram& msg)
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