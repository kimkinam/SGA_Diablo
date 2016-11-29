#include "stdafx.h"
#include "cMonster.h"
#include "cSkinnedMesh.h"
#include "cAction.h"

cMonster::cMonster()
	: m_emState(MONSTER_IDLE)
	, m_pMesh(NULL)
{
	D3DXMatrixIdentity(&m_matWorld);

	m_vPosition.x = 1.0f;
	m_vPosition.z = 1.0f;
}


cMonster::~cMonster()
{
	SAFE_DELETE(m_pAction);
	SAFE_DELETE(m_pMesh);
}

void cMonster::Setup(char* szMonsterName)
{
	string MonsterName = string(szMonsterName);
	string fileName = MonsterName + ".x";
	m_pMesh = new cSkinnedMesh("./Resources/Monster/", StringToChar(fileName));
	m_pMesh->SetAnimationIndex("attack");
}

void cMonster::Update()
{
	
}

void cMonster::Render()
{
	D3DXMATRIXA16 matR, matT;
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	D3DXMatrixRotationY(&matR, m_fAngle);

	m_matWorld = matR * matT;

	if (m_pMesh)
		m_pMesh->UpdateAndRender(&m_matWorld);


	LPD3DXFONT font;
	font = g_pFontManger->GetFont(cFontManager::E_NORMAL);

	char temp[128];
	sprintf_s(temp, "CurAnimation : %s", m_pMesh->GetCurAnimationName(), 128);
	RECT rc = { DEBUG_STARTX, DEBUG_STARTY+500, DEBUG_STARTX + 250, DEBUG_STARTY + 515 };
	font->DrawText(NULL,
		temp,
		128,
		&rc,
		DT_LEFT,
		D3DCOLOR_XRGB(255, 255, 255));
}