#include "stdafx.h"
#include "cActionAtk.h"
#include "cPlayer.h"
#include "cGameObject.h"

cActionAtk::cActionAtk()
	: m_pAttakTarget(NULL)
	, m_fAtkRange(0.0f)
	, m_fAtkTime(0.0f)
{
	m_fDistance = 0.0f;
}


cActionAtk::~cActionAtk()
{
	SAFE_RELEASE(m_pAttakTarget);
}

cAction * cActionAtk::Create()
{
	return nullptr;
}

void cActionAtk::Start()
{
	cAction::Start();
}

void cActionAtk::Update()
{
	cAction::Update();

	m_pTarget->SetPosition(m_pTarget->GetPosition());
	D3DXVECTOR3 v = m_pTarget->GetPosition() - m_pAttakTarget->GetPosition();
	m_fDistance = D3DXVec3Length(&v);

	D3DXTRACK_DESC td;
	m_pTarget->GetMesh()->GetAnimController()->GetTrackDesc(0, &td);

	double p = m_pTarget->GetAni()->GetNowPlayAnimationSet()->GetPeriodicPosition(td.Position);

	if (p > 0.8f && m_fDistance < m_fAtkRange)
	{
		if(m_pAttakTarget->GetState() == cGameObject::IDLE)
			m_pAttakTarget->SetState(cGameObject::HITTED_START);
	}

	if (p > m_pTarget->GetAni()->GetNowPlayAnimationSet()->GetPeriod() - 0.2f)
	{
		D3DXVECTOR3 vDir = m_pTarget->GetPosition() - m_pAttakTarget->GetPosition();
		D3DXVec3Normalize(&vDir, &vDir);

		D3DXMATRIXA16 matR, matT, matW;
		//	D3DXMatrixRotationY(&matR, m_fAngle);
		D3DXMatrixLookAtLH(&matR,
			&D3DXVECTOR3(0, 0, 0),
			&vDir,
			&D3DXVECTOR3(0, 1, 0));
		D3DXMatrixTranspose(&matR, &matR);
		D3DXMatrixTranslation(&matT, m_pTarget->GetPosition().x, 
			m_pTarget->GetPosition().y, m_pTarget->GetPosition().z);

		matW = matR * matT;

		m_pTarget->SetWorldTM(matW);
	}
	
	if (m_fDistance > m_fAtkRange)
	{
		LPD3DXANIMATIONSET pCurAS = NULL;
		m_pTarget->GetMesh()->GetAnimController()->GetAnimationSetByName("attack", &pCurAS);
		if (pCurAS)
		{
			//현재 애니메이션이 돌아가는 트랙정보를 가져온다.
			D3DXTRACK_DESC td;
			m_pTarget->GetMesh()->GetAnimController()->GetTrackDesc(0, &td);

			//현재 애니메이션의 전체 길이를 실행하고
			if (td.Position > pCurAS->GetPeriod() - EPSILON - 0.2f)
			{
				//상태를 변화시켜준다.
				m_pTarget->SetState(cGameObject::IDLE_START);
				m_pTarget->SetIsAtk(false);
				m_pDelegate->OnActionFinish(this);
				//this->SetState(MONSTER_IDLE_START);
			}
		}
	}

}
