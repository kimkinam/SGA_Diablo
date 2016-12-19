#include "StdAfx.h"
#include "cCamera.h"


cCamera::cCamera(void)
	: m_vEye(0, 0, -5)
	, m_vLookAt(0, 0, 0)
	, m_vUp(0, 1, 0)
	, m_isLButtonDown(false)
	, m_fAngleX(0.0f)
	, m_fAngleY(0.0f)
	, m_fDistance(10.0f)
	, m_vForward(0, 0, 1)
	, m_vRight(1, 0, 0)
	, m_pTarget(NULL)
{
	m_ptPrevMouse.x = 0;
	m_ptPrevMouse.y = 0;

	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);

	//ClientToScreen(g_hWnd, &m_ptPrevMouse);
	//SetCursorPos(m_ptPrevMouse.x, m_ptPrevMouse.y);
}


cCamera::~cCamera(void)
{
	SAFE_DELETE(m_pTarget);
}

void cCamera::Setup()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);


	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vLookAt, &m_vUp);

	g_pD3DDevice->SetTransform(D3DTS_VIEW, &m_matView);

	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4.0f, rc.right / (float)rc.bottom, 0.1f, 1000.0f);

	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);

}

void cCamera::Reset()
{
	
}

void cCamera::Render()
{
	LPD3DXFONT font;
	font = g_pFontManger->GetFont(cFontManager::E_NORMAL);

	char temp[128];
	sprintf_s(temp, "CameraPos : %.2f, %.2f, %.2f",
		m_vEye.x,
		m_vEye.y,
		m_vEye.z, 128);
	RECT rc;
	SetRect(&rc, DEBUG_STARTX, DEBUG_STARTY, DEBUG_STARTX + 250, DEBUG_STARTY + 15);
	font->DrawText(NULL,
		temp,
		128,
		&rc,
		DT_LEFT,
		D3DCOLOR_XRGB(255, 255, 255));

	SetRect(&rc, DEBUG_STARTX, DEBUG_STARTY + 15 + DEBUG_INTERVAL, DEBUG_STARTX + 250, DEBUG_STARTY + 15 + DEBUG_INTERVAL + 15);
	sprintf_s(temp, "MousePos : %d, %d", g_ptMouse.x, g_ptMouse.y);
	font->DrawText(NULL,
		temp,
		128,
		&rc,
		DT_LEFT,
		D3DCOLOR_XRGB(255, 255, 255));

}

void cCamera::Update(D3DXVECTOR3* pTarget)
{
	//D3DXMATRIXA16 matView;
	//g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	
	if (pTarget)
	{
		m_vEye = *pTarget -m_vForward * 9.0f;
		//m_vEye = *pTarget - m_vForward * 9.0f;
		//m_vEye = *pTarget - vDir * distance;
		
		//D3DXVECTOR3 camDir(0.98,- 0.12,- 0.11);
		//SetNewDirection(camDir);
		D3DXMATRIXA16 matView;
		m_vLookAt = m_vEye + m_vForward;
		D3DXMatrixLookAtLH(&matView, &m_vEye, &(m_vLookAt), &m_vUp);
		g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);
	}
	else
	{
		//if (g_pKeyManager->isStayKeyDown('W'))
		//	m_vEye += m_vForward * 5.0f * g_pTimeManager->GetDeltaTime();
		//if (g_pKeyManager->isStayKeyDown('S'))
		//	m_vEye -= m_vForward * 5.0f * g_pTimeManager->GetDeltaTime();

		if (g_pKeyManager->isStayKeyDown('D'))
			m_vEye += m_vRight * 20.0f * g_pTimeManager->GetDeltaTime();
		if (g_pKeyManager->isStayKeyDown('A'))
			m_vEye -= m_vRight * 20.0f * g_pTimeManager->GetDeltaTime();

		if (g_pKeyManager->isStayKeyDown('W'))
			m_vEye += m_vUp * 20.0f * g_pTimeManager->GetDeltaTime();
		if (g_pKeyManager->isStayKeyDown('S'))
			m_vEye -= m_vUp * 20.0f * g_pTimeManager->GetDeltaTime();

		if (g_pKeyManager->isOnceKeyDown(VK_LBUTTON))
		{
			m_ptPrevMouse = g_ptMouse;
		}
		if (g_pKeyManager->isStayKeyDown(VK_LBUTTON))
		{
			MouseRotate();
		}

		//m_vLookAt = m_vEye + m_vForward *30.0;

		//D3DXMATRIXA16 matView;
		//D3DXMatrixLookAtLH(&matView, &m_vEye, &m_vLookAt, &m_vUp);
		//g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);
	}
	D3DXMATRIXA16 matView;
	m_vLookAt = m_vEye + m_vForward;
	D3DXMatrixLookAtLH(&matView, &m_vEye, &(m_vLookAt), &m_vUp);
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);

}

void cCamera::SetNewDirection(D3DXVECTOR3 vDirection)
{
	m_vForward = vDirection;

	D3DXVECTOR3 vRight;
	D3DXVECTOR3 vUp;

	D3DXVec3Cross(&vRight, &D3DXVECTOR3(0, 1, 0), &m_vForward);
	D3DXVec3Normalize(&m_vRight, &vRight);

	D3DXVec3Cross(&vUp, &m_vForward, &m_vRight);
	D3DXVec3Normalize(&m_vUp, &vUp);

	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &m_vEye, &(m_vEye+m_vForward), &m_vUp);
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);

}


void cCamera::MoveForward(float fSpeed, float fY)
{
	D3DXVECTOR3 vZ(m_matView._13, m_matView._23 * fY, m_matView._33);
	D3DXVec3Normalize(&vZ, &vZ);

	m_vEye += vZ * fSpeed;
	m_vLookAt += vZ * fSpeed;

}

void cCamera::MoveSideward(float fSpeed)
{
	D3DXVECTOR3 vX(m_matView._11, 0, m_matView._31);
	D3DXVec3Normalize(&vX, &vX);

	m_vEye += vX * fSpeed;
	m_vLookAt += vX * fSpeed;
}

void cCamera::MouseRotate()
{
	float deltaY = (float)g_ptMouse.y - m_ptPrevMouse.y;
	float deltaX = (float)g_ptMouse.x - m_ptPrevMouse.x;

	m_fAngleX = deltaX / 5;
	m_fAngleY = deltaY / 5;

	D3DXMATRIXA16 matR;
	D3DXMatrixRotationAxis(&matR, &m_vRight, D3DXToRadian(m_fAngleY));

	D3DXVec3TransformCoord(&m_vForward, &m_vForward, &matR);
	D3DXVec3Normalize(&m_vForward, &m_vForward);

	//-------------------------------------------------------------------
	D3DXMatrixRotationAxis(&matR, &m_vUp, D3DXToRadian(m_fAngleX));

	D3DXVec3TransformCoord(&m_vForward, &m_vForward, &matR);
	D3DXVec3Normalize(&m_vForward, &m_vForward);


	D3DXVec3Cross(&m_vRight, &D3DXVECTOR3(0, 1, 0), &m_vForward);
	D3DXVec3Normalize(&m_vRight, &m_vRight);

	D3DXVec3Cross(&m_vUp, &m_vForward, &m_vRight);
	D3DXVec3Normalize(&m_vUp, &m_vUp);

	m_ptPrevMouse = g_ptMouse;

}


void cCamera::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_MOUSEWHEEL:
	{
		m_vEye += m_vForward * GET_WHEEL_DELTA_WPARAM(wParam) / 2.0F * g_pTimeManager->GetDeltaTime();
	}

	break;
	case WM_MOUSEMOVE:
		g_ptMouse.x = LOWORD(lParam);
		g_ptMouse.y = HIWORD(lParam);
		break;
	}
}

