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
	m_rcDraw.right = stImageInfo.Width; // 이미지의 넓이
	m_rcDraw.bottom = stImageInfo.Height; // 이미지 높이

	m_stCollider.nWidth *= m_matS._11;
	m_stCollider.nHeight *= m_matS._22;

}

void cUIImage::Render(LPD3DXSPRITE pSprite)
{
	if (!m_bIsDraw) return;

	assert(pSprite != NULL && "");
	assert(m_pTexture != NULL && "");

	pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	m_matW = m_matS * m_matW;
	pSprite->SetTransform(&m_matW);

	//SetRect(&rc_image, 0, 0, m_rcDraw.right, m_rcDraw.bottom);
	pSprite->Draw(m_pTexture,
		&m_rcDraw,
		&D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 0, 0),
		D3DCOLOR_ARGB(255, 255, 255, 255));

	pSprite->End();

	cUIObject::Render(pSprite);
}

void cUIImage::Render()
{

}

