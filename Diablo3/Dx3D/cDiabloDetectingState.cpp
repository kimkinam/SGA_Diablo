#include "stdafx.h"
#include "cDiabloDetectingState.h"
#include "cMonster.h"

void cDiabloDetectingState::Enter(cMonster * pOwner)
{
	pOwner->GetMesh()->SetAnimationIndex("idle");
	
	m_fIdleTime = 3.0f;
	
	int nRandom[100] =
	{	0,1,1,1,2,2,3,3,4,4,
		0,1,1,1,2,2,3,3,4,4,
		0,1,1,1,2,2,3,3,4,4,
		0,1,1,1,2,2,3,3,4,4,
		0,1,1,1,2,2,3,3,4,4,
		0,1,1,1,2,2,3,3,4,4,
		0,1,1,1,2,2,3,3,4,4,
		0,1,1,1,2,2,3,3,4,4,
		0,1,1,1,2,2,3,3,4,4,
		0,1,1,1,2,2,3,3,4,4
	};
	
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

void cDiabloDetectingState::Execute(cMonster * pOwner)
{
	if (pOwner->GetTarget()) return;

	//idle		//				// 10	//0
	//breath	//				// 30	//1
	//fireball	//8방향 확장	// 20	//2
	//attack	//기본공격		// 20	//3
	//meteor	//바닥자판		// 20	//4

	
	
	//switch (m_nState)
	//{
	//	case 0:
	//		m_fIdleTime -= g_pTimeManager->GetDeltaTime();
	//
	//		if (m_fIdleTime <= 0)
	//		{
	//			pOwner->m_pSateMachnie->ChangeState(cDiabloDetectingState::Instance());
	//		}
	//		
	//	break;
	//	case 1:
	//
	//	break;
	//	case 2:
	//	break;
	//	case 3:
	//
	//	break;
	//	case 4:
	//	break;
	//}


}

void cDiabloDetectingState::Exit(cMonster * pOwner)
{
}

bool cDiabloDetectingState::OnMessage(cMonster * pOwner, const Telegram & msg)
{
	return false;
}
