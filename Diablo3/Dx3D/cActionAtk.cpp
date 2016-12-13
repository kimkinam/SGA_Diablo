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
	SAFE_DELETE(m_pAttakTarget);
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
	
	LPD3DXANIMATIONSET pCurAS = NULL;
	m_pTarget->GetMesh()->GetAnimController()->GetTrackAnimationSet(0, &pCurAS);

	D3DXTRACK_DESC td;
	m_pTarget->GetMesh()->GetAnimController()->GetTrackDesc(0, &td);

	double p = pCurAS->GetPeriodicPosition(td.Position);

	
	if (p > pCurAS->GetPeriod() - 0.2f)
	{
		D3DXVECTOR3 vDir = m_pAttakTarget->GetPosition() - m_pTarget->GetPosition();
		D3DXVec3Normalize(&vDir, &vDir);
		
		m_pTarget->SetNewDirection(vDir);
		//D3DXMATRIXA16 matR, matT, matW;
		//D3DXMatrixLookAtLH(&matR,
		//	&D3DXVECTOR3(0, 0, 0),
		//	&vDir,
		//	&D3DXVECTOR3(0, 1, 0));
		//D3DXMatrixTranspose(&matR, &matR);
		//D3DXMatrixTranslation(&matT, m_pTarget->GetPosition().x, 
		//	m_pTarget->GetPosition().y, m_pTarget->GetPosition().z);
		//
		//matW = matR * matT;
		//
		//m_pTarget->SetWorldTM(matW);
	}
	

}
