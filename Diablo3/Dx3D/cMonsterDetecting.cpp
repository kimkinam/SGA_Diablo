#include "stdafx.h"
#include "cMonsterDetecting.h"
#include "cMonster.h"
#include "cMonsterTrace.h"
#include "cActionMove.h"
#include "cDiabloLightingBreath.h"
#include "cDiabloStom.h"
#include "cBoss.h"



void cMonsterDetecting::Enter(cMonster * pOwner)
{
	m_fTraceTime = 2.0f;
	m_fPatternSelecTime = 2.0f;

	pOwner->GetMesh()->SetAnimationIndex("idle");
	

	
	

}

void cMonsterDetecting::Execute(cMonster * pOwner)
{
	if (!pOwner->GetTarget()) return;
	
	m_fPatternSelecTime -= g_pTimeManager->GetDeltaTime();

	if (m_fPatternSelecTime <= 0)
	{
		if (pOwner->GetStat().chType == CHARACTER_TYPE::CHARACTER_DIABLO)
		{
			m_fIdleTime = 2.0f;

			int nRandom[100] =
			{ 0,0,1,1,1,2,2,2,3,3,
				0,0,1,1,1,2,2,2,3,3,
				0,0,1,1,1,2,2,2,3,3,
				0,0,1,1,1,2,2,2,3,3,
				0,0,1,1,1,2,2,2,3,3,
				0,0,1,1,1,2,2,2,3,3,
				0,0,1,1,1,2,2,2,3,3,
				0,0,1,1,1,2,2,2,3,3,
				0,0,1,1,1,2,2,2,3,3,
				0,0,1,1,1,2,2,2,3,3
			};
			// 20	//0
			// 30	//1
			// 30	//2
			// 20	//3

			//idle		//				// 20	//0
			//breath	//				// 30	//1
			//attack	//기본공격		// 30	//2
			//stom		//8방향 확장	// 20	//3
			///idle	//바바닥판			// 20	//0
			for (size_t i = 0; i < 333; ++i)
			{
				int nRandom1 = rand() % 100;

				int nRandom2 = rand() % 100;

				while (nRandom1 == nRandom2)
				{
					nRandom2 = rand() % 100;
				}


				int nTemp = nRandom[nRandom1];
				nRandom[nRandom1] = nRandom[nRandom2];
				nRandom[nRandom2] = nTemp;
			}

			int nIndex = rand() % 100;

			m_nState = nRandom[nIndex];
		}

		m_fPatternSelecTime = 999.9f;
	}
	

	if (pOwner->GetStat().chType == CHARACTER_TYPE::CHARACTER_DIABLO)
	{
		
		switch (m_nState)
		{
		case 0:
			m_fIdleTime -= g_pTimeManager->GetDeltaTime();

			if (m_fIdleTime <= 0)
			{
				pOwner->m_pSateMachnie->ChangeState(cMonsterDetecting::Instance());
			}

			break;
		case 1:
			pOwner->m_pSateMachnie->ChangeState(cDiabloLightingBreath::Instance());
			break;
		case 2:
			g_pMessageManager->MessageSend(0.0f, pOwner->GetID(), pOwner->GetID(),
				MESSAGE_TYPE::MSG_RUN, NULL);
			break;
		case 3:
			if (((cBoss*)pOwner)->GetIsActive())
				pOwner->m_pSateMachnie->ChangeState(cMonsterDetecting::Instance());
			else
				pOwner->m_pSateMachnie->ChangeState(cDiabloStom::Instance());
			break;
		}
	}
	else
	{
		D3DXVECTOR3 vLength = pOwner->GetTarget()->GetPosition()
			- pOwner->GetPosition();
		float distance = D3DXVec3Length(&vLength);

		if (distance < pOwner->GetStat().fTraceRange)
		{
			g_pMessageManager->MessageSend(0.0f, pOwner->GetID(), pOwner->GetID(),
				MESSAGE_TYPE::MSG_RUN, NULL);
			//this->Exit(pOwner);
		}
	}
}

void cMonsterDetecting::Exit(cMonster * pOwner)
{
	if (!pOwner->GetTarget()) return;
	D3DXVECTOR3 vLength = pOwner->GetTarget()->GetPosition()
		- pOwner->GetPosition();

	D3DXVECTOR3 vDir;
	D3DXVec3Normalize(&vDir, &vLength);
	pOwner->SetNewDirection(vDir);
}

bool cMonsterDetecting::OnMessage(cMonster* pOwner, const Telegram& msg)
{
	if (pOwner->GetStat().fHp <= 0) return false;

	switch (msg.emMessageType)
	{
	case MSG_RUN:
	{
		pOwner->m_pSateMachnie->ChangeState(cMonsterTrace::Instance());
		//SOUNDMANAGER->play("SkeletonAttack", 0.5f);
		return true;
	}
		break;
	case MSG_ATTACK:
		break;
	case MSG_HITTED:
		return false;
		break;
	case MSG_NONE:
		break;
	default:
		break;
	}

	return false;
}