#include "stdafx.h"
#include "cMonsterAttack.h"
#include "cMonster.h"	
#include "cActionAtk.h"
#include "cMonsterDetecting.h"

void cMonsterAttack::Enter(cMonster * pOwner)
{
	
	if (pOwner)
	{
		pOwner->GetMesh()->SetAnimationIndex("attack");

		cActionAtk* atk = new cActionAtk;
		LPD3DXANIMATIONSET pAtk;
		pOwner->GetMesh()->GetAnimController()->GetAnimationSetByName("attack", &pAtk);
		
		atk->SetActionTime(pAtk->GetPeriod());
		atk->SetTarget(pOwner);
		atk->SetDelegate(pOwner);
		atk->SetAtkRange(pOwner->GetStat().fAttackRange);
		atk->SetAttackTarget(pOwner->GetTarget());
		atk->Start();
		pOwner->SetAction(atk);
		
		pOwner->SetIsAtk(true);
		
		SAFE_RELEASE(atk);
		
	}
}

void cMonsterAttack::Execute(cMonster * pOwner)
{
	D3DXVECTOR3 v = pOwner->GetPosition() - pOwner->GetTarget()->GetPosition();
	float distance = D3DXVec3Length(&v);

	if (distance > pOwner->GetStat().fAttackRange)
	{
		LPD3DXANIMATIONSET pCurAS = NULL;
		pOwner->GetMesh()->GetAnimController()->GetTrackAnimationSet(0, &pCurAS);
		if (pCurAS)
		{
			//���� �ִϸ��̼��� ���ư��� Ʈ�������� �����´�.
			D3DXTRACK_DESC td;
			pOwner->GetMesh()->GetAnimController()->GetTrackDesc(0, &td);

			
			//���� �ִϸ��̼��� ��ü ���̸� �����ϰ�
			if (td.Position > pCurAS->GetPeriod() - EPSILON - 0.2f)
			{
				//���¸� ��ȭ�����ش�.
				pOwner->SetIsAtk(false);
				pOwner->OnActionFinish(pOwner->GetAction());
				pOwner->m_pSateMachnie->ChangeState(cMonsterDetecting::Instance());
			}
		}
	}
}

void cMonsterAttack::Exit(cMonster * pOwner)
{
}

bool cMonsterAttack::OnMessage(cMonster* pOwner, const Telegram& msg)
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