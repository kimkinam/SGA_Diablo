#include "stdafx.h"
#include "cMonsterGlobalState.h"
#include "cMonster.h"

void cMonsterGlobalState::Enter(cMonster * pOwner)
{
}

void cMonsterGlobalState::Execute(cMonster * pOwner)
{
	assert(pOwner->GetStat().fHp > 0 && "ав╬З╫ю╢о╢ы.");

	
}

void cMonsterGlobalState::Exit(cMonster * pOwner)
{
}
