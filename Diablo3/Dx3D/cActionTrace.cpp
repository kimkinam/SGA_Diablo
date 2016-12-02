#include "stdafx.h"
#include "cActionTrace.h"


cActionTrace::cActionTrace()
	: m_tFrom(NULL)
	, m_tTo(NULL)
	, m_vPosition(NULL)
	, m_fSpeed(0.05f)
	, m_fDistance(0.0f)
	, m_fTraceTime(0.0f)
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
	m_fDistance = 0.0f;

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

	//D3DXVECTOR3 dir1 = m_pTarget->GetPosition() - (*m_tTo);

	//if (D3DXVec3Dot(&dir1, &dir2) <= 0)
	//{
	//	m_pTarget->SetPosition(*m_tTo);
	//	m_pDelegate->OnActionFinish(this);
	//}
	//D3DXVECTOR3 dir2 = (*m_tFrom) - (*m_tTo);
	//
	//if (D3DXVec3Dot(&dir1, &dir2) <= 0)
	//{
	//	m_pTarget->SetPosition(*m_tTo);
	//	m_pDelegate->OnActionFinish(this);
	//}

	m_fPassedTime += g_pTimeManager->GetDeltaTime();
	m_fDistance = D3DXVec3Length(&((*m_tFrom) - (*m_tTo)));

	if (m_fDistance < 2.0f)
	{
		m_pTarget->SetPosition(*this->GetPosition());
		m_pDelegate->OnActionFinish(this);
	}
	else if (m_fPassedTime > m_fTraceTime)
	{
		m_pTarget->SetPosition(*this->GetPosition());
		m_pDelegate->OnActionFinish(this);
		
	}

	//Distance();
}