#include "stdafx.h"
#include "cActionMove.h"


cActionMove::cActionMove()
	: m_vFrom(0, 0, 0)
	, m_vTo(0, 0, 0)
	, m_fSpeed(0.0f)
{
}


cActionMove::~cActionMove()
{
}


void cActionMove::Start()
{
	m_fPassedTime = 0;

	m_vTo.y = 0;
	
}

void cActionMove::Update()
{
	cAction::Update();

	//m_vDirection = m_vPosition - m_vPrevPosition;
	//
	//D3DXMATRIXA16 matR, matT;
	//D3DXMatrixLookAtLH(&matR,
	//	&D3DXVECTOR3(0, 0, 0),
	//	&m_vDirection,
	//	&D3DXVECTOR3(0, 1, 0));
	//D3DXMatrixTranspose(&matR, &matR);
	//D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	//
	//m_matWorld = matR * matT;
	//
	//D3DXMATRIXA16 mat;
	//D3DXMatrixRotationY(&mat, D3DX_PI);
	//m_matWorld = mat * m_matWorld;
	//
	//m_vPrevPosition = m_vPosition;

	D3DXVECTOR3 position = m_pTarget->GetPosition();

	D3DXVECTOR3 dir = m_vTo - m_vFrom;
	D3DXVec3Normalize(&dir, &dir);

	position = position + dir * m_fSpeed;

	D3DXMATRIXA16 matR, matT, matW;
	D3DXMatrixLookAtLH(&matR,
		&D3DXVECTOR3(0, 0, 0),
		&dir,
		&D3DXVECTOR3(0, 1, 0));
	D3DXMatrixTranspose(&matR, &matR);
	D3DXMatrixTranslation(&matT, position.x, position.y, position.z);

	matW = matR * matT;

	D3DXMATRIXA16 localmatR;
	D3DXMatrixRotationY(&localmatR, m_pTarget->GetAngle());

	matW = localmatR * matW;

	m_pTarget->SetPosition(position);
	m_pTarget->SetDirection(dir);
	m_pTarget->SetWorldTM(matW);


	D3DXVECTOR3 dir1 = m_pTarget->GetPosition() - m_vTo;
	D3DXVECTOR3 dir2 = m_vFrom - m_vTo;

	if (D3DXVec3Dot(&dir1, &dir2) <= 0)
	{
		m_pTarget->SetPosition(m_vTo);
		m_pDelegate->OnActionFinish(this);
	}
	//D3DXVECTOR3 dir = m_vTo - m_vFrom;
	//D3DXVec3Normalize(&dir, &dir);
	//
	//D3DXVECTOR3 position = m_pTarget->GetPosition();
	//	
	//position = position + dir * 0.1f;
	//
	//m_pTarget->SetPosition(position);
	//
	//D3DXVECTOR3 dir1 = m_pTarget->GetPosition() - m_vTo;
	//D3DXVECTOR3 dir2 = m_vFrom - m_vTo;
	//
	//if (D3DXVec3Dot(&dir1, &dir2) <= 0)
	//{
	//	m_pTarget->SetPosition(m_vTo);
	//	m_pDelegate->OnActionFinish(this);
	//}
}


cAction * cActionMove::Create()
{
	return nullptr;
}