#include "stdafx.h"
#include "cGameObject.h"
#include "cStateMachine.h"


cGameObject::cGameObject()
	: m_vPosition(0, 0, 0)
	, m_vDirection(0, 0, 1)
	, m_vUp(0, 1, 0)
	, m_vRight(1, 0, 0)
	, m_vScale(1, 1, 1)
	, m_pAction(NULL)
	, m_pMesh(NULL)
	, m_pOBB(NULL)
	, m_pTarget(NULL)
	, m_pSphere(NULL)
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matLocal);

	m_ID = m_INextID;
	m_INextID++;
}


cGameObject::~cGameObject()
{
	SAFE_RELEASE(m_pAction);
	SAFE_RELEASE(m_pSphere);
	//SAFE_DELETE(m_pTarget);
	SAFE_DELETE(m_pMesh);
	SAFE_DELETE(m_pOBB);

	m_INextID--;
}

void cGameObject::Setup(D3DXVECTOR3* vLook)
{
	if (vLook)
	{
		m_vDirection = *vLook;

		D3DXVECTOR3 vRight;
		D3DXVECTOR3 vUp;

		D3DXVec3Cross(&vRight, &D3DXVECTOR3(0, 1, 0), &m_vDirection);
		D3DXVec3Normalize(&m_vRight, &vRight);

		D3DXVec3Cross(&vUp, &m_vDirection, &m_vRight);
		D3DXVec3Normalize(&m_vUp, &vUp);
	}
		

	//D3DXMATRIXA16 matR, matT;

	D3DXVECTOR3 scaledRight		= m_vScale.x * m_vRight;
	D3DXVECTOR3 scaledUp		= m_vScale.y * m_vUp;
	D3DXVECTOR3 scaledForward	= m_vScale.z * m_vDirection;

	m_matLocal._11 = scaledRight.x;		m_matLocal._12 = scaledRight.y;		m_matLocal._13 = scaledRight.z;
	m_matLocal._21 = scaledUp.x;		m_matLocal._22 = scaledUp.y;		m_matLocal._23 = scaledUp.z;
	m_matLocal._31 = scaledForward.x;	m_matLocal._32 = scaledForward.y;	m_matLocal._33 = scaledForward.z;
	m_matLocal._41 = 0;					m_matLocal._42 = 0;					m_matLocal._43 = 0;

	m_matWorld = m_matLocal;

	m_pMesh->GetBoundingSphere()->vCenter = m_vPosition;
	D3DXCreateSphere(g_pD3DDevice, m_pMesh->GetBoundingSphere()->fRadius, 20, 20, &m_pSphere, NULL);
}

void cGameObject::Update()
{
	if (m_pAction)
	{
 		m_pAction->Update();
	}

	m_pMesh->GetBoundingSphere()->vCenter = m_vPosition;
}

void cGameObject::Render()
{
	if (m_pOBB)
		m_pOBB->Update(&m_matWorld);

	D3DXMATRIXA16 matT;
	//D3DXMatrixIdentity(&matT);
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matT);
	//if (m_pSphere)
	//	m_pSphere->DrawSubset(0);
	//if (m_pOBB)
	//	m_pOBB->DebugRender(D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}

void cGameObject::OnActionFinish(cAction * pSender)
{
	m_pAction = NULL;
	
	//g_pMessageManager->MessageSend(0.0f, this->GetID(), this->GetID(), MESSAGE_TYPE::MSG_IDLE, NULL);
	m_pMesh->SetAnimationIndex("idle");
}

void cGameObject::SetNewDirection(D3DXVECTOR3 vDirection)
{
	m_vDirection = vDirection;

	D3DXVECTOR3 vRight;
	D3DXVECTOR3 vUp;

	D3DXVec3Cross(&vRight, &D3DXVECTOR3(0, 1, 0), &m_vDirection);
	D3DXVec3Normalize(&m_vRight, &vRight);

	D3DXVec3Cross(&vUp, &m_vDirection, &m_vRight);
	D3DXVec3Normalize(&m_vUp, &vUp);

	//m_matWorld._11 = m_vRight.x;		m_matWorld._12 = m_vRight.y;		m_matWorld._13 = m_vRight.z;
	//m_matWorld._21 = m_vUp.x;			m_matWorld._22 = m_vUp.y;			m_matWorld._23 = m_vUp.z;
	//m_matWorld._31 = m_vDirection.x;	m_matWorld._32 = m_vDirection.y;	m_matWorld._33 = m_vDirection.z;
	//m_matWorld._41 = 0;					m_matWorld._42 = 0;					m_matWorld._43 = 0;

	m_matLocal._11 = m_vRight.x;		m_matLocal._12 = m_vRight.y;		m_matLocal._13 = m_vRight.z;
	m_matLocal._21 = m_vUp.x;			m_matLocal._22 = m_vUp.y;			m_matLocal._23 = m_vUp.z;
	m_matLocal._31 = m_vDirection.x;	m_matLocal._32 = m_vDirection.y;	m_matLocal._33 = m_vDirection.z;
	m_matLocal._41 = 0;					m_matLocal._42 = 0;					m_matLocal._43 = 0;

	
}

LPD3DXANIMATIONSET cGameObject::GetCurAnimation()
{
	LPD3DXANIMATIONSET pCurAS = NULL;
	m_pMesh->GetAnimController()->GetTrackAnimationSet(0, &pCurAS);

	return pCurAS;
}

void cGameObject::SetAnimation(int nIndex)
{
	m_pMesh->SetAnimationIndex(nIndex);
}
void cGameObject::SetAnimation(string szAniName)
{
	m_pMesh->SetAnimationIndex(StringToChar(szAniName));
}

bool cGameObject::IsDoneCurAni()
{
	LPD3DXANIMATIONSET pCurAS = this->GetCurAnimation();

	D3DXTRACK_DESC td;
	this->m_pMesh->GetAnimController()->GetTrackDesc(0, &td);
	
	double dCurTime = pCurAS->GetPeriodicPosition(td.Position);
	double dTotalTime = pCurAS->GetPeriod();
	double dPercent = dCurTime / dTotalTime;

	if (dPercent > dTotalTime - dTotalTime  * 0.1f) return true;

	return false;
}

double cGameObject::GetCurAniTime()
{
	LPD3DXANIMATIONSET pCurAS = this->GetCurAnimation();

	return pCurAS->GetPeriod();
}

double cGameObject::GetAniTimeWithName(string szAniName)
{
	LPD3DXANIMATIONSET pCurAS = NULL;
	m_pMesh->GetAnimController()->GetAnimationSetByName(StringToChar(szAniName), &pCurAS);

	return pCurAS->GetPeriod();
}

bool cGameObject::HandleMessage(const Telegram& msg)
{
	return false;
}
