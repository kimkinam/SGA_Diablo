#include "stdafx.h"
#include "cWhirlwindState.h"
#include "cPlayerWarCryState.h"
#include "cPlayer.h"
#include "cActionMove.h"
#include "cMonster.h"

void cWhirlwindState::Enter(cPlayer * pOwner)
{
	if (!pOwner) return;
	//if (!pOwner->GetTarget()) return;

	pOwner->SetAnimation("whirlwinding");

	//double totalTime = pOwner->GetCurAniTime();
	//g_pMessageManager->MessageSend(totalTime / 2, pOwner->GetID(), pOwner->GetTarget()->GetID(),
	//	MESSAGE_TYPE::MSG_HITTED, &(float)pOwner->GetStat().fAtk);

	SOUNDMANAGER->play("WhirlWind", 0.6f);

	m_vecMonster.clear();

	std::map<int, cGameObject*> m_mapMonster;
	std::map<int, cGameObject*>::iterator m_mapMonsterIter;

	m_mapMonster = g_pAIManager->GetAImap();

	m_bIsAtk = false;

	for (m_mapMonsterIter = m_mapMonster.begin(); m_mapMonsterIter != m_mapMonster.end(); ++m_mapMonsterIter)
	{
		m_vecMonster.push_back(m_mapMonsterIter->second);
	}
}


void cWhirlwindState::Execute(cPlayer * pOwner)
{
	if (!pOwner) return;
	
	LPD3DXANIMATIONSET pCurAS = pOwner->GetCurAnimation();

	D3DXTRACK_DESC td;
	pOwner->GetMesh()->GetAnimController()->GetTrackDesc(0, &td);

	double dCurTime = pCurAS->GetPeriodicPosition(td.Position);
	double dTotalTime = pCurAS->GetPeriod();
	double dPercent = dCurTime / dTotalTime;


	if (pOwner->IsDoneCurAni())
	//if (dPercent > dTotalTime)
	{
		SOUNDMANAGER->stop("WhirlWind");
		g_pMessageManager->MessageSend(0.0f, pOwner->GetID(),
			pOwner->GetID(), MESSAGE_TYPE::MSG_IDLE);

	}
	
	for (size_t i = 0; i < m_vecMonster.size(); ++i)
	{
		if (m_vecMonster[i]->GetID() == pOwner->GetID()) continue;
		if (m_vecMonster[i]->GetStat().fHp <= 0) continue;
		cOBB* oBB = pOwner->GetOBB();
		oBB->SetMax(oBB->GetMax() * 2);
		oBB->SetMin(oBB->GetMin() * 2);
		if (cOBB::IsCollision(oBB, m_vecMonster[i]->GetOBB()) && !m_vecMonster[i]->GetIsHit())
		{
			ST_HIT_EXTRAINFO msgHit;
			msgHit.dwHitType = 0;
			msgHit.fDamage = pOwner->GetStat().fAtk;

			m_vecMonster[i]->SetIsHit(true);
			g_pMessageManager->MessageSend(0.0f, pOwner->GetID(), m_vecMonster[i]->GetID(),
				MESSAGE_TYPE::MSG_HITTED, &msgHit);
			//m_vecMonster[i]->GetStat().fHp -= pOwner->GetStat().fAtk;
		}
	}
}

void cWhirlwindState::Exit(cPlayer * pOwner)
{
	SOUNDMANAGER->stop("WhirlWind");
	//pOwner->GetTarget()->Release();
	//pOwner->SetTarget(NULL);
}

bool cWhirlwindState::OnMessage(cPlayer * pOwner, const Telegram & msg)
{
	switch (msg.emMessageType)
	{
	case MSG_RUN:
		//if (pOwner->IsDoneCurAni())
			pOwner->m_pSateMachnie->ChangeState(cPlayerMoveState::Instance());
		return true;
		break;
	case MSG_ATTACK:
		//if (pOwner->IsDoneCurAni())
			pOwner->m_pSateMachnie->ChangeState(cPlayerAttackState::Instance());
		return true;
		break;
	case MSG_IDLE:
		//if (pOwner->IsDoneCurAni())
			pOwner->m_pSateMachnie->ChangeState(cPlayerIdleState::Instance());
		return true;
		break;
	case MSG_WARCRY:
		pOwner->m_pSateMachnie->ChangeState(cPlayerWarCryState::Instance());
		return true;
		break;
	case MSG_NONE:
		break;
	case MSG_WHIRLWIND:
	{

		ST_RUN_EXTRAINFO MSG;

		MSG = *(ST_RUN_EXTRAINFO*)msg.ExtraInfo;

		if (!SOUNDMANAGER->isPlaySound("WhirlWind"))
			SOUNDMANAGER->play("WhirlWind", 0.6f);
		cActionMove* pAction = new cActionMove;
		pAction->SetTo(MSG.vDest);
		pAction->SetFrom(pOwner->GetPosition());
		pAction->SetTarget(pOwner);
		pAction->SetDelegate(pOwner);
		pAction->SetSpeed(0.05f);
		pAction->SetOBB(MSG.vecBox);
		pAction->Start();
		pOwner->SetAction(pAction);
	}

		return true;
		break;
	default:
		break;
	}

	return false;
}
