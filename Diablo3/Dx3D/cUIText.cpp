#include "stdafx.h"
#include "cUIText.h"


cUIText::cUIText()
	: m_dwDrawTextFormat(DT_LEFT | DT_TOP | DT_NOCLIP)
	, m_dwTextColor(D3DCOLOR_XRGB(0, 0, 0))
	, m_sText("")
	, m_pFont(NULL)
{
}


cUIText::~cUIText()
{
	SAFE_RELEASE(m_pFont);
}

void cUIText::Render(LPD3DXSPRITE pSprite)
{
	if (!m_bIsDraw)
		return;

	RECT rc;
	SetRect(&rc,
		m_matW._41,
		m_matW._42,
		m_matW._41 + 250,
		m_matW._42 + 100);

	m_pFont->DrawText(NULL,
		m_sText.c_str(),
		m_sText.length(),
		&rc,
		m_dwDrawTextFormat,
		m_dwTextColor);


	//g_pFontManger->PrintOut(m_sText.c_str(), rc, m_dwDrawTextFormat, m_dwTextColor);
}
