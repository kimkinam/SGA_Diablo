#include "stdafx.h"
#include "cMonsterTrace.h"
#include "cMonster.h"
#include "cActionMove.h"
#include "cMonsterDetecting.h"
#include "cMonsterAttack.h"

void cMonsterTrace::Enter(cMonster * pOwner)
{
	if (pOwner)
	{
		cActionMove* pAction = new cActionMove;

		pAction->SetTo(pOwner->GetTarget()->GetPosition());
		pAction->SetFrom(pOwner->GetPosition());
		pAction->SetTarget(pOwner);
		pAction->SetDelegate(pOwner);
		pAction->SetSpeed(0.02f);
		pAction->SetOBB(pOwner->GetBoundBox());
		pAction->Start();
		pOwner->SetAction(pAction);
	}
	pOwner->SetAnimation("run");
}

void cMonsterTrace::Execute(cMonster * pOwner)
{
	D3DXVECTOR3 vLength = pOwner->GetTarget()->GetPosition()
		- pOwner->GetPosition();

	float distance = D3DXVec3Length(&vLength);

	//���� ��Ÿ� �ȿ� ������ ���ݻ��·� �ٲ��ش�.
	if (distance < pOwner->GetStat().fAttackRange)
	{
		pOwner->m_pSateMachnie->ChangeState(cMonsterAttack::Instance());
	}

	//���� ��Ÿ� ���̸� �˻���·� �ٲ��ش�.
	if (distance > pOwner->GetStat().fTraceRange)
	{
		pOwner->m_pSateMachnie->ChangeState(cMonsterDetecting::Instance());
	}
}

void cMonsterTrace::Exit(cMonster * pOwner)
{
	//pOwner->SetAction(NULL);
}

bool cMonsterTrace::OnMessage(cMonster* pOwner, const Telegram& msg)
{
	if (pOwner->GetStat().fHp <= 0) return false;

	switch (msg.emMessageType)
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
