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
		if (pOwner->GetStat().fHp <= 0) return;
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

		}

		double totalTime = pOwner->GetCurAniTime();
		g_pMessageManager->MessageSend(totalTime * 2 / 3, pOwner->GetID(), pOwner->GetTarget()->GetID(),
			MESSAGE_TYPE::MSG_HITTED, &(float)pOwner->GetStat().fAtk);
	}

	//���¿� �����ؼ� ����
	virtual void Execute(cMonster* pOwner)
	{
		if (pOwner->GetStat().fHp <= 0) return;

		D3DXVECTOR3 vDir = pOwner->GetTarget()->GetPosition() - pOwner->GetPosition();
		float distance = D3DXVec3Length(&vDir);
		D3DXVec3Normalize(&vDir, &vDir);

		if(pOwner->IsDoneCurAni())
			pOwner->SetNewDirection(vDir);

		//���ݻ�Ÿ� ������ ������
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
				//if(pOwner->IsDoneCurAni())
				if (td.Position > pCurAS->GetPeriod() - EPSILON - 0.2f)
				{
					//���¸� ��ȭ�����ش�.
					//pOwner->SetIsAtk(false);

					pOwner->SetAction(NULL);
					//pOwner->OnActionFinish(pOwner->GetAction());
					pOwner->m_pSateMachnie->ChangeState(cMonsterDetecting::Instance());
				}
			}
		}

		else
		{
			if (pOwner->IsDoneCurAni())
			{
				g_pMessageManager->MessageSend(0.0f, pOwner->GetID(), pOwner->GetID(),
					MESSAGE_TYPE::MSG_ATTACK, NULL);
				//double totalTime = pOwner->GetCurAniTime();
				//g_pMessageManager->MessageSend(totalTime / 2, pOwner->GetID(), pOwner->GetTarget()->GetID(),
				//	MESSAGE_TYPE::MSG_HITTED, &(float)pOwner->GetStat().fAtk);
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
		if (pOwner->GetStat().fHp <= 0) return false;
		switch (msg.emMessageType)
		{
		case MSG_ATTACK:
			pOwner->m_pSateMachnie->ChangeState(cMonsterAttack::Instance());
			return true;
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
