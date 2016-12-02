#include "stdafx.h"
#include "cActionTrace.h"


cActionTrace::cActionTrace()
	: m_tFrom(NULL)
	, m_tTo(NULL)
	, m_vPosition(NULL)
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
	m_fPassedTime = 0;
	D3DXVECTOR3 Dir = (*m_tFrom) - (*m_tTo);
	D3DXVec3Normalize(&Dir, &Dir);
	m_pTarget->SetDirection(Dir);
}


void cActionTrace::Update()
{
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

		position = position - m_pTarget->GetDirection() * 0.05f;
		m_vPosition = &position;
		m_pTarget->SetPosition(*m_vPosition);
		m_pTarget->SetAngle(angle);

	}

	D3DXVECTOR3 dir1 = m_pTarget->GetPosition() - (*m_tTo);

<<<<<<< HEAD
	//if (D3DXVec3Dot(&dir1, &dir2) <= 0)
	//{
	//	m_pTarget->SetPosition(*m_tTo);
	//	m_pDelegate->OnActionFinish(this);
	//}
=======
	D3DXVECTOR3 dir2 = (*m_tFrom) - (*m_tTo);
	
	if (D3DXVec3Dot(&dir1, &dir2) <= 0)
	{
		m_pTarget->SetPosition(*m_tTo);
		m_pDelegate->OnActionFinish(this);
	}
>>>>>>> a14e8c3c8cf5045941a2bc098576c46f12c28957
}

bool cActionTrace::Distance()
{
	D3DXVECTOR3	v3 = (*m_tFrom) - (*m_tTo);
	float distance = D3DXVec3Length(&v3);
	
	if (distance < 1.0f)
		return true;
}