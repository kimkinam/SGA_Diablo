#include "stdafx.h"
#include "cGameObject.h"


cGameObject::cGameObject()
	: m_vPrevPosition(0, 0, 0)
	, m_vPosition(0, 0, 0)
	, m_vDirection(0, 0, 1)
	, m_vUp(0, 1, 0)
	, m_pAction(NULL)
	, m_pMesh(NULL)
	, m_fAngle(0.0f)
	, m_bIsMove(false)
	, m_bIsAtk(false)
	, m_emState(IDLE)
	, m_pAni(NULL)
	, m_pOBB(NULL)
{
	time = 0.0f;
	D3DXMatrixIdentity(&m_matWorld);
}


cGameObject::~cGameObject()
{
	SAFE_RELEASE(m_pAction);
	SAFE_DELETE(m_pMesh);
	SAFE_DELETE(m_pAni);
	SAFE_DELETE(m_pOBB);

}

void cGameObject::Setup(D3DXVECTOR3* vLook)
{
	D3DXMATRIXA16 matR, matT;

	if (vLook)
	{
		float m_fAngle = acosf(D3DXVec3Dot(vLook, &m_vDirection));

		D3DXMatrixRotationY(&matR, m_fAngle);

		m_vDirection = *vLook;
	}
	else
	{
		m_fAngle = D3DX_PI;
	}
		
	D3DXMatrixRotationY(&matR, m_fAngle);
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	m_matWorld = matR * matT;	

	//m_pAni = new cAnimation;
	//m_pAni->SetAnimController(m_pMesh->GetAnimController());
	//m_pAni->Setup();
	
	//m_pAni->Play("idle");


}

void cGameObject::Update()
{
	if (m_pAction)
	{
		m_pAction->Update();
	}

	//if (m_pAni)
	//	m_pAni->Update();

	//switch (m_emState)
	//{
	//case cGameObject::IDLE_START:
	//	//m_pAni->Play("idle");
	//	m_pMesh->SetAnimationIndex("idle");
	//	m_emState = IDLE;
	//	break;
	//case cGameObject::TRACE_START:
	//	//m_pAni->Play("run");
	//	m_pMesh->SetAnimationIndex("run");
	//	m_emState = TRACE;
	//	break;
	//case cGameObject::MOVE_START:
	//	break;
	//
	//case cGameObject::HITTED_START:
	//{
	//	//m_pAni->PlayOneShot("hit");
	//	//m_emState = HITTED;
	//}
	//case cGameObject::HITTED:
	//{
	//	//LPD3DXANIMATIONSET pAS = NULL;
	//	//
	//	//m_pMesh->GetAnimController()->GetTrackAnimationSet(0, &pAS);
	//	//
	//	//if (pAS)
	//	//{
	//	//	D3DXTRACK_DESC td;
	//	//	m_pMesh->GetAnimController()->GetTrackDesc(0, &td);
	//	//
	//	//	if (td.Position > pAS->GetPeriod() - EPSILON - 0.1f)
	//	//	{
	//	//		m_pAni->Play("Idle");
	//	//
	//	//		time = g_pTimeManager->GetTotalSec();
	//	//		m_emState = cGameObject::ATTACK;
	//	//	}
	//	//}
	//	//
	//	//SAFE_RELEASE(pAS);
	//	//
	//	//float a = m_pAni->GetNowPlayAnimationSet()->GetPeriod();
	//	//double d = m_pAni->GetNowPlayAnimationSet()->GetPeriodicPosition(m_pAni->m_Track_Desc_0.Position);
	//	//
	//	//if (d > a - 0.1f)
	//	//{
	//	//	m_pAni->Play("Idle", 0.1f);
	//	//	m_emState = cGameObject::IDLE;
	//	//}
	//}
	//	//m_pMesh->SetAnimationIndex("hit");
	//	
	//	break;
	//case cGameObject::KNOCKBACK_START:
	//	break;
	//case cGameObject::STUNNED_START:
	//	break;
	//case cGameObject::DEAD_START:
	//	break;
	//default:
	//	break;
	//}
}

void cGameObject::Render()
{

	if (m_pOBB)
		m_pOBB->Update(&m_matWorld);

	if (m_pOBB)
		m_pOBB->DebugRender(D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}

void cGameObject::OnActionFinish(cAction * pSender)
{
}
