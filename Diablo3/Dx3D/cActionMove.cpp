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

	D3DXVECTOR3 position = m_pTarget->GetPosition();

	m_vDirection = m_vTo - m_vFrom;
	D3DXVec3Normalize(&m_vDirection, &m_vDirection);

	position = position + m_vDirection * m_fSpeed;
	

	if (!m_vecOBB.empty())
		{
			for (size_t i = 0; i < m_vecOBB.size(); ++i)
			{
				cOBB& temp = *(m_pTarget->GetOBB());

				temp.GetCenterPos() = position + m_vDirection * m_fSpeed;

			if (cOBB::IsCollision(&temp, m_vecOBB[i]))
				{
					m_pTarget->SetPosition(m_pTarget->GetPosition());
					m_pDelegate->OnActionFinish(this);
					g_pMessageManager->MessageSend(0.0f, m_pTarget->GetID(), m_pTarget->GetID(),
						MESSAGE_TYPE::MSG_IDLE, NULL);
					return;
				}
			}
		}


	m_pTarget->SetPosition(position);
	m_pTarget->SetNewDirection(m_vDirection);

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
	return new cActionMove;
}