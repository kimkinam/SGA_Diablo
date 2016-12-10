#include "StdAfx.h"
#include "cCharController.h"
#include "iMap.h"
#include "cActionJump.h"
#include "cActionFalling.h"


cCharController::cCharController(void)
	: m_fFalling(false)
	, m_fAngle(0.0f)
	, m_fSpeed(0.1f)
	, m_pAction(NULL)
	, m_pMap(NULL)
{
	D3DXMatrixIdentity(&m_matWorld);
}

cCharController::~cCharController(void)
{
	SAFE_RELEASE(m_pAction);
}

void cCharController::Update(iMap* map)
{
	if (GetKeyState('A') & 0x8000)
	{
		m_fAngle -= 0.07f;
	}
	if (GetKeyState('D') & 0x8000)
	{
		m_fAngle += 0.07f;
	}

	m_vDirection = D3DXVECTOR3(0, 0, 1);
	D3DXMATRIXA16 matRotY;
	D3DXMatrixRotationY(&matRotY, D3DXToRadian(180));
	D3DXMatrixRotationY(&matRotY, m_fAngle);
	D3DXVec3TransformNormal(&m_vDirection, &m_vDirection, &matRotY);

	D3DXVECTOR3 vPosition = m_vPosition;
	
	
	if (m_pAction)
		m_pAction->Update();

	if(map && !m_pAction)
	{
		if(map->GetHeight(vPosition.x, vPosition.y, vPosition.z))
		{
			if (vPosition.y < m_vPosition.y - 1.0f)
			{
				m_fFalling = true;
			}
			else
			{
				if (vPosition.y - m_vPosition.y < 0.7f)
					m_vPosition = vPosition;
			}
		}
	}

	if (m_fFalling)
	{
		cActionFalling* pAction = new cActionFalling;
		pAction->SetTarget(this);
		pAction->SetDelegate(this);
		pAction->SetMap(map);
		pAction->SetEndPosition(vPosition - (m_vDirection * 0.1f));
		pAction->Start();
		m_pAction = pAction;
		m_fFalling = false;
	}

	D3DXMATRIXA16 matT;
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	m_matWorld = matRotY * matT;

}

void cCharController::OnActionFinish(cAction * pSender)
{
	m_pAction = NULL;
}

