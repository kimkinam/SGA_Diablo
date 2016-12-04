#include "stdafx.h"
#include "cActionTrace.h"


cActionTrace::cActionTrace()
	: m_tFrom(NULL)
	, m_tTo(NULL)
	, m_vPosition(NULL)
	, m_fSpeed(0.0f)
	, m_fTraceRange(0.0f)
	, m_fAttackRange(0.0f)
	, m_fDistance(0.0f)
	, m_bIsAtk(false)
{
}


cActionTrace::~cActionTrace()
{
	//SAFE_DELETE(m_tFrom);
	//SAFE_DELETE(m_tTo);
	//SAFE_DELETE(m_vPosition);
}

cAction * cActionTrace::Create()
{
	return nullptr;
}

void cActionTrace::Start()
{
	cAction::Start();

	//D3DXVECTOR3 Dir = (*m_tFrom) - (*m_tTo);
	//D3DXVec3Normalize(&Dir, &Dir);
	//m_pTarget->SetDirection(Dir);
}


void cActionTrace::Update()
{
	cAction::Update();

	D3DXVECTOR3 dir = (*m_tTo) - (*m_tFrom);
	D3DXVec3Normalize(&dir, &dir);

	D3DXVECTOR3 position = m_pTarget->GetPosition();

	position = position + dir * m_fSpeed;

	D3DXMATRIXA16 matR, matT, matW;
	D3DXMatrixLookAtLH(&matR,
		&D3DXVECTOR3(0, 0, 0),
		&-dir,
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

	//타겟과의 거리
	m_fDistance = D3DXVec3Length(&((*m_tFrom) - (*m_tTo)));

	//공격할 상황
	if (m_fAttackRange > m_fDistance)
	{
		m_pTarget->SetPosition(m_pTarget->GetPosition());
		m_pTarget->SetIsAtk(true);
		m_pDelegate->OnActionFinish(this);

	}
	//추적포기 상황
	if (m_fTraceRange < m_fDistance)
	{
		m_pTarget->SetPosition(m_pTarget->GetPosition());
		m_pTarget->SetIsAtk(false);
		m_pDelegate->OnActionFinish(this);
	}

}