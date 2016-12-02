#include "stdafx.h"
#include "cPlayer.h"
#include "cSkinnedMesh.h"
#include "cObj.h"
#include "cAction.h"
#include "cOBB.h"

cPlayer::cPlayer()
	: m_emState(PLAYER_IDLE)
	, m_pMesh(NULL)
	, m_bIsAtk(false)
	, m_dAttackStartTime(0.0f)
	, m_dAttackTermTime(0.0f)
	, m_pSword(NULL)
	, m_nCurMap(0)
	, m_pOBB(NULL)
{
	D3DXMatrixIdentity(&m_matWorld);
	
}


cPlayer::~cPlayer()
{
	SAFE_RELEASE(m_pAction);
	SAFE_DELETE(m_pMesh);
	SAFE_DELETE(m_pOBB);

	SAFE_RELEASE(m_pSword);
}

void cPlayer::SetUp()
{
	//바바
	m_pMesh = new cSkinnedMesh("./Resources/Player/", "Bab3.X");
	m_pMesh->SetAnimationIndex(1);

	m_pMesh->GetBoundingSphere()->vCenter.y = 0.5f;

	m_pOBB = new cOBB;
	D3DXVECTOR3 vMin, vMax;
	
	vMin = m_pMesh->GetMin();
	vMax = m_pMesh->GetMax();
	
	vMin.x += 0.6f;
	vMax.x -= 0.6f;
	vMax.y -= 0.2f;

	m_pOBB->Setup(vMin, vMax);

	//칼
	m_pSword = new cObj;
	m_pSword->SetUp("twohandsword.objobj", "./Resources/Object/");

	m_pSword->SetWorldTM(m_pMesh->AttachItem("right_weapon"));

}

void cPlayer::Update()
{
	if (m_pAction)
		m_pAction->Update();

	AniControl();


	if (m_vPosition.x < 0 && m_vPosition.z < 0)	//왼쪽아래
		m_nCurMap = 0;
	if (m_vPosition.x < 0 && m_vPosition.z > 0)	//왼쪽위
		m_nCurMap = 1;
	if (m_vPosition.x > 0 && m_vPosition.z > 0)	//오른쪽위
		m_nCurMap = 2;
	if (m_vPosition.x > 0 && m_vPosition.z < 0)	//오른쪽아래
		m_nCurMap = 3;

	
	
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
	
	if (m_pOBB)
		m_pOBB->Update(&m_matWorld);

	if (m_pOBB)
		m_pOBB->DebugRender(D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
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

	m_pMesh->SetAnimationIndex(1);
}

void cPlayer::PlayerPosition()
{
	m_pPosition = &cGameObject::GetPosition();
}

