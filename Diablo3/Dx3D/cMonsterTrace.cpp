#include "stdafx.h"
#include "cMonsterTrace.h"
#include "cMonster.h"
#include "cActionTrace.h"
#include "cMonsterDetecting.h"
#include "cMonsterAttack.h"

void cMonsterTrace::Enter(cMonster * pOwner)
{
	if (pOwner)
	{
		cActionTrace* trace = new cActionTrace;
		cGameObject* target = pOwner->GetTarget();

		trace->SetTo(target->GetPtPosition());
		trace->SetFrom(pOwner->GetPtPosition());
		trace->SetTarget(pOwner);
		trace->SetDelegate(pOwner);

		trace->SetTraceRange(pOwner->GetStat().fTraceRange);
		trace->SetAttackRange(pOwner->GetStat().fAttackRange);
		trace->SetSpeed(pOwner->GetStat().fSpeed);

		trace->Start();
		pOwner->SetAction(trace);

		SAFE_RELEASE(trace);

		pOwner->GetMesh()->SetAnimationIndex("run");
	}
}

void cMonsterTrace::Execute(cMonster * pOwner)
{
	D3DXVECTOR3 vLength = pOwner->GetTarget()->GetPosition()
		- pOwner->GetPosition();

	float distance = D3DXVec3Length(&vLength);

	//공격 사거리 안에 들어오면 공격상태로 바꿔준다.
	if (distance < pOwner->GetStat().fAttackRange)
	{
		pOwner->m_pSateMachnie->ChangeState(cMonsterAttack::Instance());
	}

	//공격 사거리 밖이면 검사상태로 바꿔준다.
	if (distance > pOwner->GetStat().fAttackRange)
	{
		pOwner->m_pSateMachnie->ChangeState(cMonsterDetecting::Instance());
	}
}

void cMonsterTrace::Exit(cMonster * pOwner)
{
}

bool cMonsterTrace::OnMessage(cMonster* pOwner, const Telegram& msg)
{
	switch (msg.emMessageType)
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
