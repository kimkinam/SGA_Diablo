#include "stdafx.h"
#include "cUIImage.h"


cUIImage::cUIImage()
	: m_pTexture(NULL)
{
	m_rcDraw = { 0,0,0,0 };
	D3DXMatrixIdentity(&m_matS);

}


cUIImage::~cUIImage()
{
	SAFE_RELEASE(m_pTexture);
}

void cUIImage::SetTexture(char * szFileName)
{
	D3DXIMAGE_INFO stImageInfo;

	m_pTexture = g_pTextureManager->GetTexture(szFileName, &stImageInfo);

	m_stCollider.nWidth = stImageInfo.Width;
	m_stCollider.nHeight = stImageInfo.Height;
	m_rcDraw.right = stImageInfo.Width;
	m_rcDraw.bottom = stImageInfo.Height;

	m_stCollider.nWidth *= m_matS._11;
	m_stCollider.nHeight *= m_matS._22;

}

void cUIImage::Render(LPD3DXSPRITE pSprite)
{
	if (!m_bIsDraw) return;

	pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	m_matW = m_matS * m_matW;
	pSprite->SetTransform(&m_matW);

	RECT rc;
	SetRect(&rc, 0, 0, m_rcDraw.right, m_rcDraw.bottom);
	pSprite->Draw(m_pTexture,
		&rc,
		&D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 0, 0),
		D3DCOLOR_ARGB(255, 255, 255, 255));

	pSprite->End();

	cUIObject::Render(pSprite);
}

