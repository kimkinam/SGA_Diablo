#include "stdafx.h"
#include "cUIButton.h"


cUIButton::cUIButton()
	: m_emButtonState(E_NORMAL)
	, m_pDelegate(NULL)
	, m_bIsClick(false)
{
	m_rcDraw = { 0,0,0,0 };
	D3DXMatrixIdentity(&m_matS);
}


cUIButton::~cUIButton()
{
}

void cUIButton::SetTexture(char * szNor, char * szOvr, char * szSel)
{
	m_vecTexture.resize(3);
	D3DXIMAGE_INFO stImageInfo;
	m_vecTexture[E_NORMAL] = g_pTextureManager->GetTexture(szNor, &stImageInfo);

	m_stCollider.nWidth = stImageInfo.Width;
	m_stCollider.nHeight = stImageInfo.Height;
	m_rcDraw.right = stImageInfo.Width;
	m_rcDraw.bottom = stImageInfo.Height;

	m_vecTexture[E_OVER] = g_pTextureManager->GetTexture(szOvr, &stImageInfo);
	assert(m_stCollider.nWidth == stImageInfo.Width &&
		m_stCollider.nHeight == stImageInfo.Height);

	m_vecTexture[E_CLICKED] = g_pTextureManager->GetTexture(szSel, &stImageInfo);
	assert(m_stCollider.nWidth == stImageInfo.Width &&
		m_stCollider.nHeight == stImageInfo.Height);

	m_stCollider.nWidth *= m_matS._11;
	m_stCollider.nHeight *= m_matS._22;
}

void cUIButton::Update()
{
	cUIObject::Update();

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	
	RECT rc;
	SetRect(&rc,
		m_matW._41,
		m_matW._42,
		m_matW._41 + m_stCollider.nWidth,
		m_matW._42 + m_stCollider.nHeight);
	
	if (PtInRect(&rc, pt))
	{
		if (g_pKeyManager->isStayKeyDown(VK_LBUTTON))
		{
			m_bIsClick = true;
			if (m_emButtonState == E_OVER)
				m_emButtonState = E_CLICKED;
		}
		else
		{
			m_bIsClick = false;
			if (m_emButtonState == E_CLICKED)
			{
				if (m_pDelegate)
					m_pDelegate->OnClick(this);
			}
			m_emButtonState = E_OVER;
		}
	}
	else
	{
		if (g_pKeyManager->isStayKeyDown(VK_LBUTTON))
		{

		}
		else
			m_emButtonState = E_NORMAL;
	}
		

	
}


void cUIButton::Render(LPD3DXSPRITE pSprite)
{
	if (!m_bIsDraw) return;

	pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	m_matW = m_matS * m_matW;
	pSprite->SetTransform(&m_matW);

	RECT rc;
	SetRect(&rc, 0, 0, m_rcDraw.right, m_rcDraw.bottom);
	pSprite->Draw(m_vecTexture[m_emButtonState],
		&rc,
		&D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 0, 0),
		D3DCOLOR_ARGB(255, 255, 255, 255));

	pSprite->End();

	cUIObject::Render(pSprite);
}

void cUIButton::OnClick(cUIButton * pSender)
{
}
