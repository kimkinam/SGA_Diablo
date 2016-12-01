#include "stdafx.h"
#include "cItem.h"
#include "cUIImage.h"

cItem::cItem()
	: m_eItemType(E_ITEM::ITEM_COUNT)
	, m_pSumNail(NULL)
	, m_pImage(NULL)
{
	fPlusHp = fPlusMp = fPlusAtk = fPlusSpeed = 0.0f;
	m_ImageName = m_SumNailName = { '\0' };
	m_ptArea.x = m_ptArea.y = 0;

	m_rcDraw = { 0, 0, 0, 0 };
	D3DXMatrixIdentity(&m_matS);
}


cItem::~cItem()
{
	SAFE_RELEASE(m_pSumNail);
	SAFE_RELEASE(m_pImage);
}

void cItem::Setup(char* szImageName, char* sumNailName, float fhp, float fmp, float fAtk, float fSpeed, E_ITEM Item_type)
{
	D3DXIMAGE_INFO stImageInfo;
	//string path = string(szFolder);
	//string name = string(szImageName);
	//string fullPath = path + name;
	m_pImage = g_pTextureManager->GetTexture(szImageName, &stImageInfo);
	
	
	m_stCollider.nWidth = stImageInfo.Width;
	m_stCollider.nHeight = stImageInfo.Height;
	m_rcDraw.right = stImageInfo.Width; // 이미지의 넓이
	m_rcDraw.bottom = stImageInfo.Height; // 이미지 높이

	m_stCollider.nWidth *= m_matS._11;
	m_stCollider.nHeight *= m_matS._22;
	
	m_ptArea.x = m_stCollider.nStartX;
	m_ptArea.y = m_stCollider.nStartY;

	fPlusHp = fhp;
	fPlusMp = fhp;
	fPlusAtk = fAtk;
	fPlusSpeed = fSpeed;

	St_item.stPlusStat.fHp = fPlusHp;
	St_item.stPlusStat.fMp = fPlusMp;
	St_item.stPlusStat.fAtk = fPlusAtk;
	St_item.stPlusStat.fSpeed = fPlusSpeed;
}

void cItem::Update()
{

}

void cItem::Render(LPD3DXSPRITE pSprite)
{
	if (!m_bIsDraw) return;

	pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	m_matW = m_matS * m_matW;
	pSprite->SetTransform(&m_matW);

	//SetRect(&rc_image, 0, 0, m_rcDraw.right, m_rcDraw.bottom);
	pSprite->Draw(m_pImage,
		&m_rcDraw,
		&D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 0, 0),
		D3DCOLOR_ARGB(255, 255, 255, 255));

	pSprite->End();
	cUIObject::Render(pSprite);
}
