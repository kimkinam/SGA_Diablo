#include "stdafx.h"
#include "cDiabloLightingBreath.h"
#include "cMonsterDetecting.h"
#include "cMonster.h"
#include "cBoss.h"

void cDiabloLightingBreath::Enter(cMonster * pOwner)
{
	if (!pOwner->GetTarget()) return;

	m_fSkillTime = 5.0f;
	m_bIsBreath = false;

	pOwner->SetAnimation("breathStart");
}

void cDiabloLightingBreath::Execute(cMonster * pOwner)
{
	D3DXVECTOR3 vDir = pOwner->GetTarget()->GetPosition() - pOwner->GetPosition();
	float distance = D3DXVec3Length(&vDir);
	D3DXVec3Normalize(&vDir, &vDir);

	pOwner->SetNewDirection(vDir);

	string name = pOwner->GetCurAnimation()->GetName();

	if (pOwner->IsDoneCurAni() && name == "breathStart")
	{
		pOwner->SetAnimation("breathLoop");
	}

	name = pOwner->GetCurAnimation()->GetName();

	if (name == "breathLoop")
	{
		if (m_fSkillTime >= 0)
		{
			m_fSkillTime -= g_pTimeManager->GetDeltaTime();

			//น฿ป็
			if (!m_bIsBreath)
			{
				SOUNDMANAGER->play("DiabloLightBreath", 1.0f);
				dynamic_cast<cBoss*>(pOwner)->BreathSkillStart(5.0f);
				m_bIsBreath = true;
			}

			
		}
		else
		{
			pOwner->SetAnimation("breathEnd");
			SOUNDMANAGER->stop("DiabloLightBreath");
		}
	}

	if (name == "breathEnd" && pOwner->IsDoneCurAni())
	{
		pOwner->m_pSateMachnie->ChangeState(cMonsterDetecting::Instance());
		//this->Exit(pOwner);
	}
}

void cDiabloLightingBreath::Exit(cMonster * pOwner)
{
	pOwner->SetAnimation("idle");
	//pOwner->m_pSateMachnie->ChangeState(cMonsterDetecting::Instance());
}

bool cDiabloLightingBreath::OnMessage(cMonster * pOwner, const Telegram & msg)
{
	return false;
}
