#include "stdafx.h"
#include "cMonsterGlobalState.h"
#include "cMonster.h"

void cMonsterGlobalState::Enter(cMonster * pOwner)
{
}

void cMonsterGlobalState::Execute(cMonster * pOwner)
{
	assert(pOwner->GetStat().fHp > 0 && "�׾����ϴ�.");

	
}

void cMonsterGlobalState::Exit(cMonster * pOwner)
{
}

bool cMonsterGlobalState::OnMessage(cMonster* pOwner, const Telegram& msg)
{
	switch (msg.Msg)
	{
	case MSG_RUN:
		// �ൿ ó��
		return true;
		break;
	case MSG_NONE:
		// �ൿ ó��
		return true;
		break;
	}

	return false;
}