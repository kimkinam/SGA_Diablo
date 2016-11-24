#include "stdafx.h"
#include "cUISkill.h"


cUISkill::cUISkill()
: m_SkillState(S_normal)
, m_sDelegate(NULL)
, m_skillClick(false)

{
	D3DXMatrixIdentity(&m_matS);
}


cUISkill::~cUISkill()
{
}

void cUISkill::SetSkillTexture(char* szNor, char* szCtm, char* szSel)
{
	m_SvecTextur.resize(3);
	D3DXIMAGE_INFO sImageinfo;
	m_SvecTextur[S_normal] = g_pTextureManager->GetTexture(szNor, &sImageinfo);
	m_stCollider.nWidth = sImageinfo.Width;
	m_stCollider.nHeight = sImageinfo.Height;
	m_SvecTextur[S_Clicked] = g_pTextureManager->GetTexture(szSel, &sImageinfo);
	m_SvecTextur[S_CoolTime] = g_pTextureManager->GetTexture(szCtm, &sImageinfo);
}

void cUISkill::OnClick(cUIButton * pSender)
{
}

