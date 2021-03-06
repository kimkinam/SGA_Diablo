#include "stdafx.h"
#include "cMonsterTrace.h"
#include "cMonster.h"
#include "cActionMove.h"
#include "cActionTrace.h"
#include "cMonsterDetecting.h"
#include "cMonsterAttack.h"

void cMonsterTrace::Enter(cMonster * pOwner)
{
	if (!pOwner) return;

	if (pOwner->GetStat().chType != CHARACTER_DIABLO)
	{
		cActionTrace* pAction = new cActionTrace;

		pAction->SetTraceTarget(pOwner->GetTarget());
		pAction->SetTarget(pOwner);
		pAction->SetDelegate(pOwner);
		pAction->SetSpeed(0.03f);
		pAction->SetOBB(pOwner->GetBoundBox());
		
		std::map<int, cGameObject*> map;
		std::map<int, cGameObject*>::iterator iter;
		
		map = g_pAIManager->GetAImap();
		
		for (iter = map.begin(); iter != map.end();++iter)
		{
			if (iter->first == 0 || iter->first == pOwner->GetID()) continue;
			pAction->GetMonster().push_back((iter->second));
		}

		//for (iter = map.begin(); iter != map.end(); ++iter)
		//{
		//	if (iter->first == 0 || iter->first == pOwner->GetID()) continue;
		//	pAction->GetOBB().push_back((iter->second->GetOBB()));
		//}
		pAction->Start();
		pOwner->SetAction(pAction);

		if (pOwner->GetStat().chType == CHARACTER_SKELETON)
			SOUNDMANAGER->play("SkeletonAttack", 0.5f);
		else if (pOwner->GetStat().chType == CHARACTER_STITCH)
			SOUNDMANAGER->play("StitchAttack", 0.5f);
		else if (pOwner->GetStat().chType == CHARACTER_FETISH)
			SOUNDMANAGER->play("FetishAttack", 0.5f);
		else if (pOwner->GetStat().chType == CHARACTER_GARHANTUAN)
			SOUNDMANAGER->play("GargantuanAttack", 0.5f);
		else if (pOwner->GetStat().chType == CHARACTER_ZOMBIEDOG)
			SOUNDMANAGER->play("ZombieDogAttack", 0.5f);

	}
	else
	{
		m_fTraceTime = 2.0f;

		cActionTrace* pAction = new cActionTrace;

		pAction->SetTraceTarget(pOwner->GetTarget());
		pAction->SetTarget(pOwner);
		pAction->SetDelegate(pOwner);
		pAction->SetSpeed(pOwner->GetStat().fSpeed);
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

	if (pOwner->GetStat().chType == CHARACTER_DIABLO)
	{
		m_fTraceTime -= g_pTimeManager->GetDeltaTime();

		if (m_fTraceTime <=0)
		{
			pOwner->m_pSateMachnie->ChangeState(cMonsterDetecting::Instance());
		}
		else if (distance < pOwner->GetStat().fAttackRange)
		{
			pOwner->m_pSateMachnie->ChangeState(cMonsterAttack::Instance());
		}

		return;
	}

	

	std::map<int, cGameObject*> map;
	std::map<int, cGameObject*>::iterator iter;

	map = g_pAIManager->GetAImap();

	//for (iter = map.begin(); iter != map.end();)
	//{
	//	if (iter->first == 0 || iter->first == pOwner->GetID())
	//	{
	//		++iter;
	//		continue;
	//	}
	//
	//	ST_SPHERE temp = *(iter->second->GetMesh()->GetBoundingSphere());
	//	ST_SPHERE owner = *(pOwner->GetMesh()->GetBoundingSphere());
	//
	//	D3DXVECTOR3 vDir = iter->second->GetPosition() - pOwner->GetPosition();
	//	float fLength = D3DXVec3Length(&vDir);
	//
	//	if (fLength <= temp.fRadius + owner.fRadius)
	//	{
	//		pOwner->SetAction(NULL);
	//		break;
	//		int a = 0;
	//	}
	//	else
	//		++iter;
	//}
	

	//공격 사거리 안에 들어오면 공격상태로 바꿔준다.
	if (distance < pOwner->GetStat().fAttackRange)
	{
		pOwner->m_pSateMachnie->ChangeState(cMonsterAttack::Instance());
	}

	//공격 사거리 밖이면 검사상태로 바꿔준다.
	if (distance > pOwner->GetStat().fTraceRange)
	{
		pOwner->m_pSateMachnie->ChangeState(cMonsterDetecting::Instance());
	}
}

void cMonsterTrace::Exit(cMonster * pOwner)
{
	pOwner->SetAction(NULL);
}

bool cMonsterTrace::OnMessage(cMonster* pOwner, const Telegram& msg)
{
	if (pOwner->GetStat().fHp <= 0) return false;

	switch (msg.emMessageType)
	{
	case MSG_IDLE:
		pOwner->m_pSateMachnie->ChangeState(cMonsterDetecting::Instance());
		return true;
		break;
	case MSG_RUN:
		// 행동 처리
		return true;
		break;
	case MSG_NONE:
		// 행동 처리
		return true;
		break;
	case MSG_HITTED:
		return false;
		break;
	}

	return false;
}
