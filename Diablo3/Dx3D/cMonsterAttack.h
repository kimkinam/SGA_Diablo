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
	//상태에 진입
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

	//상태에 진입해서 갱신
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
				//현재 애니메이션이 돌아가는 트랙정보를 가져온다.
				D3DXTRACK_DESC td;
				pOwner->GetMesh()->GetAnimController()->GetTrackDesc(0, &td);


				//현재 애니메이션의 전체 길이를 실행하고
				if (td.Position > pCurAS->GetPeriod() - EPSILON - 0.2f)
				{
					//상태를 변화시켜준다.
					pOwner->SetIsAtk(false);
					pOwner->OnActionFinish(pOwner->GetAction());
					pOwner->m_pSateMachnie->ChangeState(cMonsterDetecting::Instance());
				}
			}
		}
	}

	//상태에서 퇴장
	virtual void Exit(cMonster* pOwner)
	{
		
	}

	//GameObject의 HandleMessage로부터 메시지를 접수하면 이 부분이 실행
	virtual bool OnMessage(cMonster* pOwner, const Telegram& msg)
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
};
