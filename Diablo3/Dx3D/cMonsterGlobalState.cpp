#include "stdafx.h"
#include "cMonsterGlobalState.h"
#include "cMonster.h"

void cMonsterGlobalState::Enter(cGameObject * pOwner)
{
}

void cMonsterGlobalState::Execute(cGameObject * pOwner)
{
	//assert(pOwner->GetStat().fHp > 0 && "�׾����ϴ�.");

	
}

void cMonsterGlobalState::Exit(cGameObject * pOwner)
{
}

bool cMonsterGlobalState::OnMessage(cGameObject* pOwner, const Telegram& msg)
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