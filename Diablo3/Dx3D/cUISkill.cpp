#include "stdafx.h"
#include "cUISkill.h"


cUISkill::cUISkill()
: m_SkillState(S_normal)
, m_sDelegate(NULL)
, m_skillClick(false)
, m_fUsedTime(0.0f)
, m_fCoolTime(0.0f)
, Skill_CoolTime(62)

{
	D3DXMatrixIdentity(&m_matS);
}


cUISkill::~cUISkill()
{
}

void cUISkill::SetSkillTexture(char* szNor,char* szSel)
{
	m_SvecTextur.resize(2);
	D3DXIMAGE_INFO sImageinfo;
	m_SvecTextur[S_normal] = g_pTextureManager->GetTexture(szNor, &sImageinfo);
	m_stCollider.nWidth = sImageinfo.Width;
	m_stCollider.nHeight = sImageinfo.Height;
	m_rcDraw.right = sImageinfo.Width;
	m_rcDraw.bottom = sImageinfo.Height;

	m_SvecTextur[S_Clicked] = g_pTextureManager->GetTexture(szSel, &sImageinfo);

	m_stCollider.nWidth *= m_matS._11;
	m_stCollider.nHeight *= m_matS._22;

	CoolTime_img = g_pTextureManager->GetTexture("./Resources/UI/ÄðÅ¸ÀÓ¹Ù.png", &CoolTime);

}

void cUISkill::Update() 
{
	cUIObject::Update();

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	RECT rc;
	GetClientRect(g_hWnd,&rc);
	
	SetRect(&rc_cooltime, 0, 0, Skill_CoolTime, -9);


		if (g_pKeyManager->isOnceKeyDown(VK_RBUTTON)&& m_SkillState == S_normal)
		{
			m_SkillState = S_Clicked;
			m_fUsedTime = g_pTimeManager->GetTotalSec();
			Skill_CoolTime = 0;
		}
		if (m_SkillState == S_Clicked 
			&& m_fCoolTime < g_pTimeManager->GetTotalSec() - m_fUsedTime)
		{
			m_SkillState = S_normal;
			m_fUsedTime = 0;	
		}
		if (Skill_CoolTime < 62){Skill_CoolTime = (62 / m_fCoolTime)*(g_pTimeManager->GetTotalSec()-m_fUsedTime);}
		if (Skill_CoolTime > 62){Skill_CoolTime = 62;}
}

void cUISkill::Render(LPD3DXSPRITE pSprite)
{
	if (!m_bIsDraw) return;

	pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	m_matW = m_matS * m_matW;
	pSprite->SetTransform(&m_matW);

	
	RECT rc;
	SetRect(&rc, 0, 0, m_rcDraw.right, m_rcDraw.bottom);


	pSprite->Draw(m_SvecTextur[m_SkillState],
		&rc,
		&D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 0, 0),
		D3DCOLOR_ARGB(255, 255, 255, 255));

	pSprite->Draw(CoolTime_img, &rc_cooltime,
		&D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 0, 0),
		D3DCOLOR_ARGB(255, 255, 255, 255));



	pSprite->End();
	/*LPD3DXFONT font;
	font = g_pFontManger->GetFont(cFontManager::E_NORMAL);

	char temp[128];
	sprintf_s(temp, "UsedTime : %.2f",
		m_fUsedTime, 128);
	rc = { DEBUG_STARTX, DEBUG_STARTY - 50, DEBUG_STARTX + 250, DEBUG_STARTY + 15 };
	font->DrawText(NULL,
		temp,
		128,
		&rc,
		DT_LEFT,
		D3DCOLOR_XRGB(255, 255, 255));*/
}

void cUISkill::OnClick(cUIButton * pSender)
{
}


