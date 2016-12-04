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
}

cAction * cActionTrace::Create()
{
	return nullptr;
}

void cActionTrace::Start()
{
	m_fPassedTime = 0.0f;

	D3DXVECTOR3 Dir = (*m_tFrom) - (*m_tTo);
	D3DXVec3Normalize(&Dir, &Dir);
	m_pTarget->SetDirection(Dir);
}


void cActionTrace::Update()
{
	cAction::Update();

	D3DXVECTOR3 Dir = (*m_tFrom) - (*m_tTo);
	D3DXVec3Normalize(&Dir, &Dir);
	m_pTarget->SetDirection(Dir);


	if (m_pTarget)
	{
		float angle;
		D3DXVECTOR3	v3 = (*m_tFrom) - (*m_tTo);
		angle = acosf(((*m_tTo).x - (*m_tFrom).x) / D3DXVec3Length(&v3));

		if ((*m_tFrom).z > (*m_tTo).z)
			angle = D3DX_PI + D3DX_PI / 2 + angle;
		else
			angle = D3DX_PI + D3DX_PI / 2 - angle;

		D3DXVECTOR3 position = m_pTarget->GetPosition();

		position = position - m_pTarget->GetDirection() * m_fSpeed;
		m_vPosition = &position;
		m_pTarget->SetPosition(*m_vPosition);
		m_pTarget->SetAngle(angle);

	}


	//타겟과의 거리
	m_fDistance = D3DXVec3Length(&((*m_tFrom) - (*m_tTo)));

	//공격할 상황
	if (m_fAttackRange > m_fDistance)
	{
		m_pTarget->SetPosition(*this->GetPosition());
		m_bIsAtk = true;
		m_pDelegate->OnActionFinish(this);

	}
	//추적포기 상황
	if (m_fTraceRange < m_fDistance)
	{
		m_pTarget->SetPosition(*this->GetPosition());
		m_bIsAtk = false;
		m_pDelegate->OnActionFinish(this);
	}

}