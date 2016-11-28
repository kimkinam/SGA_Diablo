#include "stdafx.h"
#include "cPlayer.h"
#include "cSkinnedMesh.h"
#include "cObj.h"
#include "cAction.h"

cPlayer::cPlayer()
	: m_emState(PLAYER_IDLE)
	, m_pMesh(NULL)
	, m_bIsAtk(false)
	, m_dAttackStartTime(0.0f)
	, m_dAttackTermTime(0.0f)
	, m_pSword(NULL)
{
	D3DXMatrixIdentity(&m_matWorld);
	
}


cPlayer::~cPlayer()
{
	SAFE_DELETE(m_pAction);
	SAFE_DELETE(m_pMesh);

	SAFE_DELETE(m_pSword);
}

void cPlayer::SetUp()
{
	//¹Ù¹Ù
	m_pMesh = new cSkinnedMesh("./Resources/Player/", "Bab1.X");
	m_pMesh->SetAnimationIndex(5);

	//Ä®
	m_pSword = new cObj;
	m_pSword->SetUp("twohandsword.objobj", "./Resources/Object/");

	m_pSword->SetWorldTM(m_pMesh->AttachItem("right_weapon"));

}

void cPlayer::Update()
{
	if (m_pAction)
		m_pAction->Update();

	AniControl();

	
}

void cPlayer::Render()
{
	D3DXMATRIXA16 matR, matT;
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	D3DXMatrixRotationY(&matR, m_fAngle);

	m_matWorld = matR * matT;

	if (m_pMesh)
		m_pMesh->UpdateAndRender(&m_matWorld);

	if (m_pSword)
		m_pSword->Render();
}

void cPlayer::AniControl()
{
}

void cPlayer::Picking()
{
}

void cPlayer::OnActionFinish(cAction * pSender)
{
	m_pAction = NULL;

	m_pMesh->SetAnimationIndex(5);
}
