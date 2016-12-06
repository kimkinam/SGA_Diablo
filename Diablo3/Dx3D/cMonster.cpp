#include "stdafx.h"
#include "cMonster.h"
#include "cSkinnedMesh.h"
#include "cActionTrace.h"
#include "cActionAtk.h"
#include "cPlayer.h"
#include "cMonsterDetecting.h"
#include "cMonsterGlobalState.h"
#include "cMonsterTrace.h"

cMonster::cMonster()
//: m_emState(MONSTER_IDLE)
	: m_pTarget(NULL)
	, m_pAttackSphere(NULL)
	, m_pTraceSphere(NULL)
	, m_fAttackRange(0.0f)
	, m_fTraceRange(0.0f)
	, m_fSpeed(0.0f)
	, m_pSateMachnie(NULL)
{
	m_pSateMachnie = new cStateMachine<cMonster>(this);

	m_pSateMachnie->SetCurState(cMonsterDetecting::Instance());
	m_pSateMachnie->SetGlobalState(cMonsterGlobalState::Instance());

	m_sSumNailName = m_sObjName = m_sFolderName = "";
}


cMonster::~cMonster()
{
	SAFE_RELEASE(m_pAttackSphere);
	SAFE_RELEASE(m_pTraceSphere);
	SAFE_RELEASE(m_pTarget);
}


void cMonster::Setup(char * szMonsterName, D3DXVECTOR3* vLookAt)
{
	cGameObject::Setup(vLookAt);

	string MonsterName = string(szMonsterName);
	string fileName = MonsterName + ".x";
	m_pMesh = new cSkinnedMesh("./Resources/Monster/", StringToChar(fileName));
	m_pMesh->SetAnimationIndex("idle");

	D3DXCreateSphere(g_pD3DDevice, m_fAttackRange, 20, 20, &m_pAttackSphere, NULL);
	D3DXCreateSphere(g_pD3DDevice, m_fTraceRange, 20, 20, &m_pTraceSphere, NULL);

	
	
	
}

void cMonster::Update()
{
	if (m_pSateMachnie)
		m_pSateMachnie->Update();
	cGameObject::Update();
	//
	//
	////몬스터가 기본적으로 해야할 짓들
	//switch (m_emState)
	//{
	//
	//case cGameObject::IDLE:
	//	Trace();
	//	break;
	//case cGameObject::TRACE:
	//	break;
	//case cGameObject::MOVE:
	//	break;
	//case cGameObject::ATTACK_START:
	//{
	//	//m_pAni->Play("attack");
	//	m_pMesh->SetAnimationIndex("attack");
	//
	//	cActionAtk* atk = new cActionAtk;
	//	LPD3DXANIMATIONSET pAtk;
	//	m_pMesh->GetAnimController()->GetAnimationSetByName("attack", &pAtk);
	//
	//	atk->SetActionTime(pAtk->GetPeriod());
	//	atk->SetTarget(this);
	//	atk->SetDelegate(this);
	//	atk->SetAtkRange(m_fAttackRange);
	//	atk->SetAttackTarget(m_pTarget);
	//	atk->Start();
	//	this->SetAction(atk);
	//
	//	m_bIsAtk = true;
	//
	//	SAFE_RELEASE(atk);
	//
	//	m_emState = ATTACK;
	//}
	//	break;
	//case cGameObject::ATTACK:
	//	break;
	//case cGameObject::HITTED:
	//	break;
	//case cGameObject::KNOCKBACK:
	//	break;
	//case cGameObject::STUNNED:
	//	break;
	//case cGameObject::DEAD:
	//	break;
	//default:
	//	break;
	//}
}

void cMonster::Render()
{
	//몬스터 렌더
	//기본적으로 그냥 몸뚱아리는 이거 쓸거고
	//이펙트, 파티클같은것들은 각자 몬스터 랜더에서 처리

	//D3DXMATRIXA16 matR, matT;
	//D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	//D3DXMatrixRotationY(&matR, m_fAngle);
	//
	//m_matWorld = matR * matT;


	if (m_pMesh)
		m_pMesh->UpdateAndRender(&m_matWorld);

	//공격사거리 그리는 부분
	if (g_pKeyManager->isToggleKey('1'))
	{
		//D3DXMATRIXA16 mat;
		//D3DXMatrixTranslation(&mat, 0, 1, 0);
		//m_matWorld *= mat;
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		if (m_pAttackSphere)
			m_pAttackSphere->DrawSubset(0);

		if (m_pTraceSphere)
			m_pTraceSphere->DrawSubset(0);
	}
	


	//디버그 정보
	LPD3DXFONT font;
	font = g_pFontManger->GetFont(cFontManager::E_NORMAL);
	
	char temp[128];
	sprintf_s(temp, "CurAnimation : %f", m_pMesh->GetCurAnimationName().c_str(), 128);
	RECT rc = { DEBUG_STARTX, DEBUG_STARTY + 200, DEBUG_STARTX + 250, DEBUG_STARTY + 315 };
	font->DrawText(NULL,
		temp,
		128,
		&rc,
		DT_LEFT,
		D3DCOLOR_XRGB(255, 255, 255));
	
	if (m_pAction)
	{
		sprintf_s(temp, "CurAnimation : %f", m_pAction->GetDistance(), 128);//m_pMesh->GetCurAnimationName().c_str(), 128);
		rc = { DEBUG_STARTX, DEBUG_STARTY + 300, DEBUG_STARTX + 250, DEBUG_STARTY + 415 };
		font->DrawText(NULL,
			temp,
			128,
			&rc,
			DT_LEFT,
			D3DCOLOR_XRGB(255, 255, 255));
	}
	
	
	

}

void cMonster::Trace()
{
	D3DXVECTOR3 vLength = m_pTarget->GetPosition() - this->GetPosition();

	float d = D3DXVec3Length(&vLength);
	if (D3DXVec3Length(&vLength) < m_fTraceRange)
	{
		cActionTrace* trace = new cActionTrace;

		trace->SetTo(m_pTarget->GetPtPosition());
		trace->SetFrom(this->GetPtPosition());
		trace->SetTarget(this);
		trace->SetDelegate(this);

		trace->SetTraceRange(m_fTraceRange);
		trace->SetAttackRange(m_fAttackRange);
		trace->SetSpeed(m_fSpeed);

		trace->Start();
		this->SetAction(trace);
		this->SetState(TRACE_START);

		SAFE_RELEASE(trace);

	}

	

}

void cMonster::Attack()
{
	this->GetMesh()->SetAnimationIndex("attack");
}

void cMonster::OnActionFinish(cAction * pSender)
{
	if (m_bIsAtk)
		m_emState = ATTACK_START;
	else
		m_emState = IDLE_START;

	SAFE_RELEASE(pSender);
	m_pAction = NULL;

}
