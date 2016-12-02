#include "stdafx.h"
#include "cActionMove.h"


cActionMove::cActionMove()
	: m_vFrom(0, 0, 0)
	, m_vTo(0, 0, 0)
{
}


cActionMove::~cActionMove()
{
}


void cActionMove::Start()
{
	m_fPassedTime = 0;

	m_vTo.y = 0;
	D3DXVECTOR3 dir = m_vFrom - m_vTo;
	D3DXVec3Normalize(&dir, &dir);
	m_pTarget->SetDirection(dir);
}

void cActionMove::Update()
{
	if (m_pTarget && m_pTarget->GetIsMove())
	{
		float angle;
		D3DXVECTOR3	v3 = m_vFrom - m_vTo;
		angle = acosf(((m_vTo.x - m_vFrom.x) / D3DXVec3Length(&v3)));
		
		if (m_vFrom.z > m_vTo.z)
			angle = D3DX_PI + D3DX_PI / 2 + angle;
		else
			angle = D3DX_PI + D3DX_PI / 2 - angle;
	
		D3DXVECTOR3 position = m_pTarget->GetPosition();
		
		position = position - m_pTarget->GetDirection() * 0.1f;

		m_pTarget->SetPosition(position);
		m_pTarget->SetAngle(angle);

	}

	D3DXVECTOR3 dir1 = m_pTarget->GetPosition() - m_vTo;
	D3DXVECTOR3 dir2 = m_vFrom - m_vTo;

	if (D3DXVec3Dot(&dir1, &dir2) <= 0)
	{
		m_pTarget->SetPosition(m_vTo);
		m_pDelegate->OnActionFinish(this);
	}
}


cAction * cActionMove::Create()
{
	return nullptr;
}