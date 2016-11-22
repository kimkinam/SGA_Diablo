#include "StdAfx.h"
#include "cCamera.h"
#include "cRay.h"


cCamera::cCamera(void)
	: m_vEye(0, 0, -5)
	, m_vLookAt(0, 0, 0)
	, m_vUp(0, 1, 0)
	, m_isLButtonDown(false)
	, m_fAngleX(0.0f)
	, m_fAngleY(0.0f)
	, m_fDistance(5.0f)
	, m_vDir(0, 0, 1)
{
	m_ptPrevMouse.x = 0;
	m_ptPrevMouse.y = 0;
}


cCamera::~cCamera(void)
{
	//SAFE_DELETE(m_pRay);
}


void cCamera::Setup()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	D3DXMATRIXA16 matView, matProj;
	
	D3DXMatrixLookAtLH(&matView, &m_vEye, &m_vLookAt, &m_vUp);
	
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);
	
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0f, rc.right / (float)rc.bottom, 1.0f, 1000.0f);
	
	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}



void cCamera::Update(D3DXVECTOR3* pTarget)
{
	m_vEye = -m_vDir * m_fDistance;
	
	//m_vLookAt = D3DXVECTOR3(0, 0, 0);

	D3DXMATRIXA16 matRotX, matRotY;

	D3DXMatrixRotationX(&matRotX, m_fAngleX);
	D3DXMatrixRotationY(&matRotY, m_fAngleY);

	D3DXMATRIXA16 matRot = matRotX * matRotY;

	D3DXVec3TransformCoord(&m_vEye, &m_vEye, &matRot);

	if(pTarget)
	{
		m_vEye = m_vEye + *pTarget;
		m_vLookAt = m_vLookAt + *pTarget;
	}
	
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &m_vEye, &m_vLookAt, &m_vUp);
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);
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
	RECT rc = { DEBUG_STARTX, DEBUG_STARTY, DEBUG_STARTX + 250, DEBUG_STARTY + 15 };
	font->DrawText(NULL,
		temp,
		128,
		&rc,
		DT_LEFT,
		D3DCOLOR_XRGB(255, 255, 255));

	rc = { DEBUG_STARTX, DEBUG_STARTY + 15 + DEBUG_INTERVAL, DEBUG_STARTX + 250, DEBUG_STARTY + 15 + DEBUG_INTERVAL + 15 };
	sprintf_s(temp, "MousePos : %d, %d", g_ptMouse.x, g_ptMouse.y);
	font->DrawText(NULL,
		temp,
		128,
		&rc,
		DT_LEFT,
		D3DCOLOR_XRGB(255, 255, 255));

	/*cRay ray;

	ray = cRay::RayAtWorldSpace(g_ptMouse.x, g_ptMouse.y);
	rc = { DEBUG_STARTX, DEBUG_STARTY + 35 + DEBUG_INTERVAL, DEBUG_STARTX + 250, DEBUG_STARTY + 35 + DEBUG_INTERVAL + 15 };
	sprintf_s(temp, "MousePosWorld : %.2f, %.2f, %.2f",
		ray.GetOrg().x, ray.GetOrg().y, ray.GetOrg().z);
	font->DrawText(NULL,
		temp,
		128,
		&rc,
		DT_LEFT,
		D3DCOLOR_XRGB(255, 255, 255));*/

}

void cCamera::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_MOUSEWHEEL:
		m_fDistance -= GET_WHEEL_DELTA_WPARAM(wParam) / 100.f;
		break;
	case WM_LBUTTONDOWN:
		{
			m_ptPrevMouse.x = LOWORD(lParam);
			m_ptPrevMouse.y = HIWORD(lParam);
	
			m_isLButtonDown = true;
		}
		
		break;
	case WM_LBUTTONUP:
		m_isLButtonDown = false;
		break;
	case WM_MOUSEMOVE:
		{
			if (m_isLButtonDown)
			{
				POINT ptCurrMouse;
				ptCurrMouse.x = LOWORD(lParam);
				ptCurrMouse.y = HIWORD(lParam);
	
				int nDeltaX = ptCurrMouse.x - m_ptPrevMouse.x;
				int nDeltaY = ptCurrMouse.y - m_ptPrevMouse.y;
	
				m_fAngleX += nDeltaY / 100.f;
				
				if(m_fAngleX <= -D3DX_PI / 2 + EPSILON)
					m_fAngleX = -D3DX_PI / 2 + EPSILON;
	
				if(m_fAngleX >=  D3DX_PI / 2 - EPSILON)
					m_fAngleX =  D3DX_PI / 2 - EPSILON;
	
				m_fAngleY += nDeltaX / 100.f;
	
				m_ptPrevMouse = ptCurrMouse;
			}
		}
		break;
	}
}
