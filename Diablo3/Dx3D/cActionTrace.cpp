#include "stdafx.h"
#include "cActionTrace.h"
#include "cMonster.h"

cActionTrace::cActionTrace()
	: m_pTraceTarget(NULL)
	, m_fSpeed(0.0f)
	, m_fTraceRange(0.0f)
	, m_fAttackRange(0.0f)
	, m_fDistance(0.0f)
	, m_bIsAtk(false)
{
}


cActionTrace::~cActionTrace()
{
	SAFE_DELETE(m_pTraceTarget)
}

cAction * cActionTrace::Create()
{
	return nullptr;
}

void cActionTrace::Start()
{
	cAction::Start();

}


void cActionTrace::Update()
{
	cAction::Update();

	m_vDirection = (*m_pTraceTarget->GetPtPosition()) - (*m_pTarget->GetPtPosition());
	D3DXVec3Normalize(&m_vDirection, &m_vDirection);

	D3DXVECTOR3 position = m_pTarget->GetPosition();

	position = position + m_vDirection * m_fSpeed;

	if (!m_vecMonster.empty())
	{
		for (size_t i = 0; i < m_vecMonster.size(); ++i)
		{
			cOBB& temp = *(m_pTarget->GetOBB());
			
			temp.GetCenterPos() = position + m_vDirection * m_fSpeed;

			if (m_vecMonster[i]->GetOBB())
			{
				if (cOBB::IsCollision(&temp, m_vecMonster[i]->GetOBB()))
				{
					D3DXVECTOR3 vRright = m_vecMonster[i]->GetRight();

					D3DXVECTOR3 vNewDir = m_vecMonster[i]->GetPosition() - m_pTarget->GetPosition();
					D3DXVec3Normalize(&vNewDir, &vNewDir);

					float a = D3DXVec3Dot(&vRright, &m_vecMonster[i]->GetDirection());

					float distance = D3DXVec3Length(&(position - m_pTraceTarget->GetPosition()));
					float distance2 = D3DXVec3Length(&(m_vecMonster[i]->GetPosition() - m_pTraceTarget->GetPosition()));

					if (distance > distance2)
					{
						if (D3DXVec3Dot(&vRright, &vNewDir) > 0)
						{
							m_vDirection = -vRright * m_pTarget->GetStat().fAttackRange * 2;
						}
						else
						{
							m_vDirection = +vRright * m_pTarget->GetStat().fAttackRange * 2;
						}
					}

					D3DXVec3Normalize(&m_vDirection, &m_vDirection);
					position = m_pTarget->GetPosition();
					position += m_vDirection * m_fSpeed;
					m_vDirection = m_pTraceTarget->GetPosition() - m_pTarget->GetPosition();
					D3DXVec3Normalize(&m_vDirection, &m_vDirection);
					m_pTarget->SetNewDirection(m_vDirection);
					m_pTarget->SetPosition(position);
					return;

				}
			}
		

		}
	}

	if (!m_vecOBB.empty())
	{
		for (size_t i = 0; i < m_vecOBB.size(); ++i)
		{
			cOBB& temp = *(m_pTarget->GetOBB());

			temp.GetCenterPos() = position + m_vDirection * m_fSpeed;

			if (cOBB::IsCollision(&temp, m_vecOBB[i]))
			{
				D3DXVECTOR3 vTargetDir = m_pTraceTarget->GetDirection();
				D3DXVec3Normalize(&vTargetDir, &vTargetDir);
				
				D3DXVECTOR3 vPosition = m_pTarget->GetPosition();
				angle = D3DXToDegree(D3DXVec3Dot(&vTargetDir, &m_vDirection));
			
				m_vDirection -= vTargetDir;
				D3DXVec3Normalize(&m_vDirection, &m_vDirection);
				vPosition += m_vDirection * m_fSpeed;
				m_vDirection = m_pTraceTarget->GetPosition() - vPosition;
				
				D3DXVec3Normalize(&m_vDirection, &m_vDirection);

				temp.GetCenterPos() = vPosition;// +m_vDirection * m_fSpeed;
				if (cOBB::IsCollision(&temp, m_vecOBB[i]))
				{
					D3DXVECTOR3 vDir = m_pTraceTarget->GetDirection();

					if (D3DXVec3Dot(&vDir, &m_vDirection) < 0)
					{
						D3DXVECTOR3 vPos = m_pTarget->GetPosition();
						m_vDirection = m_pTraceTarget->GetPosition() - m_pTarget->GetPosition();
						D3DXVec3Normalize(&m_vDirection, &m_vDirection);

						m_vDirection += m_pTraceTarget->GetDirection();
						D3DXVec3Normalize(&m_vDirection, &m_vDirection);

						vPos += m_vDirection * m_fSpeed;
						m_pTarget->SetNewDirection(m_vDirection);
						m_pTarget->SetPosition(vPos);
					}
					else
					{
						D3DXVECTOR3 vPos = m_pTarget->GetPosition();
						D3DXVECTOR3 vDir = m_pTraceTarget->GetPosition() - vPos;
						D3DXVec3Normalize(&vDir, &vDir);
						m_pTarget->SetNewDirection(vDir);
						m_pTarget->SetPosition(vPos);

					}
					return;
				}
				else
				{
					m_pTarget->SetNewDirection(m_vDirection);
					m_pTarget->SetPosition(vPosition);
					return;
				}
			}
			
		}

	}

		m_pTarget->SetPosition(position);
		m_pTarget->SetNewDirection(m_vDirection);
	

}
