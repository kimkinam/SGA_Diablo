#pragma once
#include "cMonsterAttack.h"
#include "cMonster.h"	
#include "cActionAtk.h"
#include "cMonsterDetecting.h"

class cMonster;

class cMonsterAttack
	: public iState<cMonster>
{
private:
	float	m_fAttackRange;
	float	m_fDelay;
	float	m_fRotationDelay;

	float	m_fAttackTime;
public:
	cMonsterAttack() { }
	virtual ~cMonsterAttack() { }

public:
	static cMonsterAttack* Instance()
	{
		static cMonsterAttack instance;
		return &instance;
	}

public:
	//���¿� ����
	virtual void Enter(cMonster* pOwner)
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

	//���¿� �����ؼ� ����
	virtual void Execute(cMonster* pOwner)
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

	//���¿��� ����
	virtual void Exit(cMonster* pOwner)
	{
		
	}

	//GameObject�� HandleMessage�κ��� �޽����� �����ϸ� �� �κ��� ����
	virtual bool OnMessage(cMonster* pOwner, const Telegram& msg)
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
};
