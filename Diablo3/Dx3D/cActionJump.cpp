#include "stdafx.h"
#include "cActionJump.h"
#include "iMap.h"


cActionJump::cActionJump()
	: m_fGravity(-0.03f)
	, m_fJumpPower(0.0f)
	, m_fTargetX(0.0f)
	, m_fTargetZ(0.0f)
	, m_fTargetY(0.0f)
	, m_fPrevMapY(0.0f)
	, m_pMap(NULL)
	, m_vStartPosition(0, 0, 0)
	, m_vPrevPosition(0, 0, 0)
	, m_vEndPosition(0, 0, 0)
	, m_vDirection(0, 0, 0)
{
}


cActionJump::~cActionJump()
{
}

void cActionJump::Start()
{
	m_vStartPosition = m_pTarget->GetPosition();
	m_vPrevPosition = m_pTarget->GetPosition();
	m_fJumpPower = 0.5f;

	if (m_vEndPosition)
		m_vDirection = (m_vEndPosition) - m_vStartPosition;
	else
		m_vDirection = D3DXVECTOR3(0, 0, 0);
	
}

void cActionJump::Update()
{
	cAction::Update();
	m_fTargetX = m_pTarget->GetPosition().x;
	m_fTargetZ = m_pTarget->GetPosition().z;
	m_fTargetY = m_pTarget->GetPosition().y;

	m_fJumpPower += m_fGravity;

	m_fTargetX += m_vDirection.x * 0.1f;
	m_fTargetZ += m_vDirection.z * 0.1f;
	m_fTargetY += m_fJumpPower;


	if (m_pMap)
	{
		float fMapY = m_fTargetY;

		if (m_pMap->GetHeight(m_fTargetX, fMapY, m_fTargetZ))
		{
			m_fPrevMapY = fMapY;

			if (m_fJumpPower > 0)
			{
				if (m_fTargetY < fMapY)
					m_pTarget->SetPosition(D3DXVECTOR3(m_vPrevPosition.x, m_fTargetY, m_vPrevPosition.z));
				else
					m_pTarget->SetPosition(D3DXVECTOR3(m_fTargetX, m_fTargetY, m_fTargetZ));
			}
			else if(m_fJumpPower < 0)
			{
				if (m_fTargetY < fMapY)
				{
					m_pTarget->SetPosition(D3DXVECTOR3(m_fTargetX, fMapY, m_fTargetZ));
					m_pDelegate->OnActionFinish(this);
				}
				else
					m_pTarget->SetPosition(D3DXVECTOR3(m_fTargetX, m_fTargetY, m_fTargetZ));
			}
		}
		else
		{
			if (m_fTargetY < m_fPrevMapY)
			{
				m_pTarget->SetPosition(D3DXVECTOR3(m_vPrevPosition.x, m_fPrevMapY, m_vPrevPosition.z));
				m_pDelegate->OnActionFinish(this);
			}
			else
			{
				m_pTarget->SetPosition(D3DXVECTOR3(m_vPrevPosition.x, m_fTargetY, m_vPrevPosition.z));
			}
		}
	}

	m_vPrevPosition = m_pTarget->GetPosition();
}

