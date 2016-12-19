#include "stdafx.h"
#include "cDiabloStom.h"
#include "cMonster.h"
#include "cBoss.h"
#include "cMonsterDetecting.h"

void cDiabloStom::Enter(cMonster * pOwner)
{
	if (!pOwner->GetTarget()) return;

	string name = pOwner->GetCurAnimation()->GetName();

	if(name != "stom")
		pOwner->SetAnimation("stom");
}

void cDiabloStom::Execute(cMonster * pOwner)
{
	//파티클을 생성해서 디아블로에 넣어줌


	if (pOwner->IsPassedTime(0.3f))
	{
		((cBoss*)pOwner)->StomSkillStart(5.0);
	}

	if (pOwner->IsPassedTime(0.9f))
		pOwner->m_pSateMachnie->ChangeState(cMonsterDetecting::Instance());
}

void cDiabloStom::Exit(cMonster * pOwner)
{
}

bool cDiabloStom::OnMessage(cMonster * pOwner, const Telegram & msg)
{
	return false;
}
