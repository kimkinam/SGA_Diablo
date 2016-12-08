#include "stdafx.h"
#include "cPlayer.h"
#include "cSkinnedMesh.h"
#include "cObj.h"
#include "cAction.h"
#include "cOBB.h"

cPlayer::cPlayer()
	//: m_emState(PLAYER_IDLE)
	: m_bIsAtk(false)
	, m_dAttackStartTime(0.0f)
	, m_dAttackTermTime(0.0f)
	, m_pSword(NULL)
	, m_nCurMap(0)
	
{
	
}


cPlayer::~cPlayer()
{

	SAFE_RELEASE(m_pSword);

}

void cPlayer::SetUp()
{
	
	//바바
	m_pMesh = new cSkinnedMesh("./Resources/Player/", "babarian.X");
	m_pMesh->SetAnimationIndex("idle");

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

	cGameObject::Setup(&D3DXVECTOR3(-1,0,0));


	//m_pAni->SetPlaySpeed(0.5f);
	
}

void cPlayer::Update()
{
	cGameObject::Update();

	AniControl();

	if (m_vPosition.x < 0 && m_vPosition.z < 0)	//왼쪽아래
		m_nCurMap = 0;
	if (m_vPosition.x < 0 && m_vPosition.z > 0)	//왼쪽위
		m_nCurMap = 1;
	if (m_vPosition.x > 0 && m_vPosition.z > 0)	//오른쪽위
		m_nCurMap = 2;
	if (m_vPosition.x > 0 && m_vPosition.z < 0)	//오른쪽아래
		m_nCurMap = 3;

	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixTranslation(&m_matWorld, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	m_matWorld = m_matLocal * m_matWorld;
	
	
}

void cPlayer::Render()
{
	
	
	cGameObject::Render();


	if (m_pMesh)
		m_pMesh->UpdateAndRender(&m_matWorld);

	if (m_pSword)
		m_pSword->Render();
	
	}

void cPlayer::AniControl()
{
	cGameObject::Update();

	switch (m_emState)
	{
	case cGameObject::IDLE_START:
		break;
	case cGameObject::IDLE:
		break;
	case cGameObject::TRACE_START:
		break;
	case cGameObject::TRACE:
		break;
	case cGameObject::MOVE_START:
		break;
	case cGameObject::MOVE:
		break;
	case cGameObject::ATTACK_START:
		break;
	case cGameObject::ATTACK:
		break;
	case cGameObject::HITTED_START:
		break;
	case cGameObject::HITTED:
	{
		LPD3DXANIMATIONSET pCurAS = NULL;

		m_pMesh->GetAnimController()->GetAnimationSetByName("hit", &pCurAS);

		D3DXTRACK_DESC td;
		m_pMesh->GetAnimController()->GetTrackDesc(0, &td);

		double p = pCurAS->GetPeriodicPosition(td.Position);

		float l = pCurAS->GetPeriod();
		if (p > pCurAS->GetPeriod()/5)
		{
			m_emState = IDLE_START;
		}

		SAFE_RELEASE(pCurAS);

	}
		break;
	case cGameObject::KNOCKBACK_START:
		break;
	case cGameObject::KNOCKBACK:
		break;
	case cGameObject::STUNNED_START:
		break;
	case cGameObject::STUNNED:
		break;
	case cGameObject::DEAD_START:
		break;
	case cGameObject::DEAD:
		break;
	default:
		break;
	}
}

void cPlayer::Picking()
{
}

void cPlayer::OnActionFinish(cAction * pSender)
{
	m_pAction = NULL;
	m_bIsMove = false;
	//m_pAni->Play("idle");
	m_pMesh->SetAnimationIndex("idle");
}

void cPlayer::PlayerPosition()
{
	//m_pPosition = &cGameObject::GetPosition();
}

