#include "stdafx.h"
#include "cUIItem.h"


cUIItem::cUIItem()
: item_state(Item_normal)
, m_IDelegate(NULL)
, m_ItemClick(false)
{
	m_stItemInfo = ST_ITEM();
}


cUIItem::~cUIItem()
{
}


void cUIItem::SetItem(char* szNor, float Hp, float Mp, float Atk, float Speed)
{
	Item_img = g_pTextureManager->GetTexture(szNor, &Item_img_info);
	m_stCollider.nWidth = Item_img_info.Width;
	m_stCollider.nHeight = Item_img_info.Height;
	
	m_rcDraw.right = Item_img_info.Width; // 이미지의 렉트 
	m_rcDraw.bottom = Item_img_info.Height;

	m_stCollider.nWidth *= m_matS._11; // 스케일링한 넓이와 높이
	m_stCollider.nHeight *= m_matS._22;

//	m_stItemInfo.eItemType = ItemType;
	m_stItemInfo.szFileName = szNor; // 파일 네임
	m_stItemInfo.stPlusStat.fHp = Hp;
	m_stItemInfo.stPlusStat.fMp = Mp;
	m_stItemInfo.stPlusStat.fAtk = Atk;
	m_stItemInfo.stPlusStat.fSpeed = Speed;

}
void cUIItem::Update()
{
	cUIObject::Update();

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	
	SetRect(&rc_item, 0, 0, m_stCollider.nWidth, m_stCollider.nHeight); // 나중에 이거 포지션 바꾸야 한다.

	if (PtInRect(&rc_item, pt))
	{
		if (g_pKeyManager->isStayKeyDown(VK_LBUTTON))
		{
			item_state = Item_picked;
			m_matS._11 += 0.5;
			m_matS._22 += 0.5;
		}
	}
}
void cUIItem::Render(LPD3DXSPRITE pSprite)
{
	if (!m_bIsDraw) return;

	pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	m_matW = m_matS * m_matW;
	pSprite->SetTransform(&m_matW);


	RECT rc;
	SetRect(&rc, 0, 0, m_rcDraw.right, m_rcDraw.bottom);


	pSprite->Draw(Item_img,
		&rc,
		&D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 0, 0),
		D3DCOLOR_ARGB(255, 255, 255, 255));


	pSprite->End();
}


void cUIItem::OnClick(cUIButton * pSender)
{
}

