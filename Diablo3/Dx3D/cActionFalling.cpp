#include "stdafx.h"
#include "cActionFalling.h"
#include "iMap.h"


cActionFalling::cActionFalling()
	: m_fGravity(-0.01f)
	, m_fVelY(0.0f)
	, m_pMap(NULL)
	, m_vStartPosition(0, 0, 0)
	, m_vEndPosition(0, 0, 0)
	, m_vPrevPosition(0, 0, 0)
	, m_fPrevMapY(0.0f)
	, m_fDeltaX(0.0f)
	, m_fDeltaZ(0.0f)
{
}


cActionFalling::~cActionFalling()
{
}

void cActionFalling::Start()
{
	m_vStartPosition = m_pTarget->GetPosition();
	m_vPrevPosition = m_pTarget->GetPosition();
	m_fVelY = -0.2f;

}

void cActionFalling::Update()
{
	cAction::Update();

	m_fVelY += m_fGravity;
	float fTargetY = m_pTarget->GetPosition().y + m_fVelY;

	if (m_pMap)
	{
		float fMapY = m_vEndPosition.y;
		if (m_pMap->GetHeight(m_vPrevPosition.x, m_vPrevPosition.y, m_vPrevPosition.z))
		{
			if (fTargetY > fMapY)
			{
				m_pTarget->SetPosition(D3DXVECTOR3(m_vPrevPosition.x, fTargetY, m_vPrevPosition.z));
			}
			else
			{
				m_pTarget->SetPosition(D3DXVECTOR3(m_vPrevPosition.x, fMapY, m_vPrevPosition.z));
				m_pDelegate->OnActionFinish(this);
			}
		}
	}

	m_vPrevPosition = m_pTarget->GetPosition();
}
