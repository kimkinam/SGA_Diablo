#include "stdafx.h"
#include "cMonster.h"
#include "cSkinnedMesh.h"
#include "cActionTrace.h"

cMonster::cMonster()
	: m_emState(MONSTER_IDLE)
	, m_pMesh(NULL)
	, m_pTarget(NULL)
	, m_fTraceTime(0.0f)
{
	D3DXMatrixIdentity(&m_matWorld);

	m_vPosition.x = 1.0f;
	m_vPosition.z = 1.0f;

}


cMonster::~cMonster()
{
	SAFE_RELEASE(m_pAction);
	SAFE_DELETE(m_pMesh);
}

void cMonster::Setup(char* szMonsterName)
{
	string MonsterName = string(szMonsterName);
	string fileName = MonsterName + ".x";
	m_pMesh = new cSkinnedMesh("./Resources/Monster/", StringToChar(fileName));
	m_pMesh->SetAnimationIndex("idle");

}

void cMonster::Update()
{
	if (this->GetState() == MONSTER_IDLE)
	{
		D3DXVECTOR3 vLength = m_pTarget->GetPosition() - this->GetPosition();
		
		if (D3DXVec3Length(&vLength) < 5.0f)
		{
			Trace();
			this->SetState(MONSTER_MOVE);
		}
	}
	if (this->GetState() == MONSTER_MOVE)
	{
		if (!this->GetAction())
			this->SetState(MONSTER_IDLE);
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

void cMonster::Trace()
{
	cActionTrace* trace = new cActionTrace;

	trace->SetTo(m_pTarget->GetPtPosition());
	trace->SetFrom(this->GetPtPosition());
	trace->SetTarget(this);
	trace->SetDelegate(this);
	trace->SetTraceTime(3.0f);
	trace->Start();
	this->SetAction(trace);
	this->GetMesh()->SetAnimationIndex("run");

	SAFE_RELEASE(trace);

}