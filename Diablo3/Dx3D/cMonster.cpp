#include "stdafx.h"
#include "cMonster.h"
#include "cSkinnedMesh.h"
#include "cActionTrace.h"

cMonster::cMonster()
	: m_emState(MONSTER_IDLE)
	, m_pMesh(NULL)
	, m_pTarget(NULL)
	, m_fAttackRange(0.0f)
	, m_fTraceRange(0.0f)
	, m_fSpeed(0.0f)
{
	D3DXMatrixIdentity(&m_matWorld);
}


cMonster::~cMonster()
{
	SAFE_RELEASE(m_pAction);
	SAFE_DELETE(m_pMesh);
	SAFE_RELEASE(m_pAttackSphere);
	SAFE_RELEASE(m_pTraceSphere);
}

void cMonster::Setup(char* szMonsterName)
{
	string MonsterName = string(szMonsterName);
	string fileName = MonsterName + ".x";
	m_pMesh = new cSkinnedMesh("./Resources/Monster/", StringToChar(fileName));
	m_pMesh->SetAnimationIndex("idle");

	D3DXCreateSphere(g_pD3DDevice, m_fAttackRange, 20, 20, &m_pAttackSphere, NULL);
	D3DXCreateSphere(g_pD3DDevice, m_fTraceRange, 20, 20, &m_pTraceSphere, NULL);
}

void cMonster::Update()
{
	if (m_pAction)
		m_pAction->Update();

	switch (m_emState)
	{
	case MONSTER_IDLE_START:
		this->GetMesh()->SetAnimationIndex("idle");
		this->SetState(MONSTER_IDLE);
		break;
	case MONSTER_IDLE:
	{
		D3DXVECTOR3 vLength = m_pTarget->GetPosition() - this->GetPosition();

		if (D3DXVec3Length(&vLength) < m_fTraceRange)
		{
			Trace();
			this->SetState(MONSTER_TRACE_START);
		}
		break;
	}
	case MONSTER_TRACE_START:
		this->GetMesh()->SetAnimationIndex("run");
		this->SetState(MONSTER_TRACE);
		break;
	case MONSTER_TRACE:
	{
		//D3DXVECTOR3 vLength = m_pTarget->GetPosition() - this->GetPosition();
		//
		//if (D3DXVec3Length(&vLength) > m_fTraceRange)
		//{
		//	this->OnActionFinish(this->GetAction());
		//	this->SetPosition(this->GetPosition());
		//	this->SetState(MONSTER_IDLE_START);
		//}
			
	}
		
		
		break;
	case MONSTER_ATTACK_START:
		this->GetMesh()->SetAnimationIndex("attack");
		this->SetState(MONSTER_ATTACK);
		break;
	case MONSTER_ATTACK:
	{
		float distance = D3DXVec3Length(&(this->GetPosition() - this->GetTarget()->GetPosition()));
		
		LPD3DXANIMATIONSET pCurAS = NULL;

		this->GetMesh()->GetAnimController()->GetTrackAnimationSet(0, &pCurAS);

		//사거리를 벗어낫을 경우
		if (distance > m_fAttackRange)
		{
			if (pCurAS)
			{
				//현재 애니메이션이 돌아가는 트랙정보를 가져온다.
				D3DXTRACK_DESC td;
				this->GetMesh()->GetAnimController()->GetTrackDesc(0, &td);

				//현재 애니메이션의 전체 길이를 실행하고
				if (td.Position > pCurAS->GetPeriod() - EPSILON - 0.2f)
				{
					//상태를 변화시켜준다.
					this->SetState(MONSTER_IDLE_START);
				}
			}
			
		}

		SAFE_RELEASE(pCurAS);
		
	}
		break;
	case MONSTER_HITTED_START:
		break;
	case MONSTER_HITTED:
		break;
	case MONSTER_KNOCKBACK_START:
		break;
	case MONSTER_KNOCKBACK:
		break;
	case MONSTER_STUNNED_START:
		break;
	case MONSTER_STUNNED:
		break;
	case MONSTER_DEAD_START:
		break;
	case MONSTER_DEAD:
		break;
	default:
		break;
	}
	
	//몬스터가 기본적으로 해야할 짓들


	
}

void cMonster::Render()
{
	//몬스터 렌더
	//기본적으로 그냥 몸뚱아리는 이거 쓸거고
	//이펙트, 파티클같은것들은 각자 몬스터 랜더에서 처리

	D3DXMATRIXA16 matR, matT;
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	D3DXMatrixRotationY(&matR, m_fAngle);

	m_matWorld = matR * matT;

	if (m_pMesh)
		m_pMesh->UpdateAndRender(&m_matWorld);

	//공격사거리 그리는 부분
	//D3DXMATRIXA16 mat;
	//D3DXMatrixTranslation(&mat, 0, 1, 0);
	//m_matWorld *= mat;
	//g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	//g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//
	//if (m_pAttackSphere)
	//	m_pAttackSphere->DrawSubset(0);
	//
	//if (m_pTraceSphere)
	//	m_pTraceSphere->DrawSubset(0);
	

	//디버그 정보
	LPD3DXFONT font;
	font = g_pFontManger->GetFont(cFontManager::E_NORMAL);

	char temp[128];
	sprintf_s(temp, "CurAnimation : %s", m_pMesh->GetCurAnimationName().c_str(), 128);
	RECT rc = { DEBUG_STARTX, DEBUG_STARTY+200, DEBUG_STARTX + 250, DEBUG_STARTY + 315 };
	font->DrawText(NULL,
		temp,
		128,
		&rc,
		DT_LEFT,
		D3DCOLOR_XRGB(255, 255, 255));

}

void cMonster::Trace()
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
	this->GetMesh()->SetAnimationIndex("run");

	SAFE_RELEASE(trace);

}

void cMonster::Attack()
{
	this->GetMesh()->SetAnimationIndex("attack");
}
