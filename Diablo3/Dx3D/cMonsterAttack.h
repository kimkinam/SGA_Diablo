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
		if (pOwner->GetStat().fHp <= 0) return;

		if (pOwner->GetStat().chType == CHARACTER_STITCH)
		{
			if (pOwner)
			{
				pOwner->GetMesh()->SetAnimationIndex("attackPunch");
			}
		}
		else
		{
			if (pOwner)
			{
				pOwner->GetMesh()->SetAnimationIndex("attack");
				
			}
		}

		if(pOwner->GetStat().chType == CHARACTER_DIABLO)
			SOUNDMANAGER->play("DiabloAttack", 0.6f);
		
		ST_HIT_EXTRAINFO msgHit;
		msgHit.dwHitType = 1;
		msgHit.fDamage = pOwner->GetStat().fAtk;

		double totalTime = pOwner->GetCurAniTime();
		g_pMessageManager->MessageSend(totalTime * 2 / 3, pOwner->GetID(), pOwner->GetTarget()->GetID(),
			MESSAGE_TYPE::MSG_HITTED, &msgHit);

		
	}

	//상태에 진입해서 갱신
	virtual void Execute(cMonster* pOwner)
	{
		if (pOwner->GetStat().fHp <= 0) return;

		D3DXVECTOR3 vDir = pOwner->GetTarget()->GetPosition() - pOwner->GetPosition();
		float distance = D3DXVec3Length(&vDir);
		D3DXVec3Normalize(&vDir, &vDir);

		if (pOwner->IsDoneCurAni())
		{
			pOwner->SetNewDirection(vDir);
			this->Enter(pOwner);
		}
			
		

		//공격사거리 밖으로 나가면
		if (distance > pOwner->GetStat().fAttackRange)
		{
			//LPD3DXANIMATIONSET pCurAS = NULL;
			//pOwner->GetMesh()->GetAnimController()->GetTrackAnimationSet(0, &pCurAS);
			//if (pCurAS)
			//{
				//현재 애니메이션이 돌아가는 트랙정보를 가져온다.
				D3DXTRACK_DESC td;
				pOwner->GetMesh()->GetAnimController()->GetTrackDesc(0, &td);

				//현재 애니메이션의 전체 길이를 실행하고
				//if(pOwner->IsDoneCurAni())
				//if (td.Position > pCurAS->GetPeriod() - EPSILON - 0.2f)
				//{
					//상태를 변화시켜준다.
					//pOwner->SetIsAtk(false);

					pOwner->SetAction(NULL);
					//pOwner->OnActionFinish(pOwner->GetAction());
					pOwner->m_pSateMachnie->ChangeState(cMonsterDetecting::Instance());
				//}
			//}

				if(SOUNDMANAGER->isPlaySound("DiabloAttack"))
					SOUNDMANAGER->stop("DiabloAttack");
		}
	}

	//상태에서 퇴장
	virtual void Exit(cMonster* pOwner)
	{

	}

	//GameObject의 HandleMessage로부터 메시지를 접수하면 이 부분이 실행
	virtual bool OnMessage(cMonster* pOwner, const Telegram& msg)
	{
		if (pOwner->GetStat().fHp <= 0) return false;
		switch (msg.emMessageType)
		{
		case MSG_ATTACK:
			pOwner->m_pSateMachnie->ChangeState(cMonsterAttack::Instance());
			return true;
		case MSG_RUN:
			// 행동 처리
			return true;
			break;
		case MSG_NONE:
			// 행동 처리
			return true;
		case MSG_HITTED:
			return false;
			break;
		}

		return false;
	}
};
