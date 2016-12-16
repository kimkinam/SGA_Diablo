#include "stdafx.h"
#include "cMonster.h"
#include "cSkinnedMesh.h"
#include "cActionTrace.h"
#include "cActionAtk.h"
#include "cPlayer.h"
#include "cMonsterDetecting.h"
#include "cGameObjectGlobalState.h"
#include "cMonsterTrace.h"

cMonster::cMonster()
	: //m_pTarget(NULL)
	 m_pAttackSphere(NULL)
	, m_pTraceSphere(NULL)
	
{
	m_pSateMachnie = new cStateMachine<cMonster>(this);

	m_pSateMachnie->SetCurState(cMonsterDetecting::Instance());
	m_pSateMachnie->SetGlobalState(cGameObjectGlobalState::Instance());

	m_sSumNailName = m_sObjName = m_sFolderName = "";

	m_stStat = ST_MONSTER_STAT();
}


cMonster::~cMonster()
{
	SAFE_RELEASE(m_pAttackSphere);
	SAFE_RELEASE(m_pTraceSphere);
}

void cMonster::Setup(char * szMonsterName, D3DXVECTOR3* vLookAt)
{
	m_sObjName = szMonsterName;
	m_pMesh = new cSkinnedMesh("./Resources/Monster/", StringToChar(m_sObjName));
	m_pMesh->SetAnimationIndex("idle");

	//m_stStat.fAttackRange = D3DXVec3Length(&(m_pMesh->GetMax() + m_pMesh->GetMin()));
	//m_pMesh->GetMin();

	D3DXCreateSphere(g_pD3DDevice, m_stStat.fAttackRange, 20, 20, &m_pAttackSphere, NULL);
	D3DXCreateSphere(g_pD3DDevice, m_stStat.fTraceRange, 20, 20, &m_pTraceSphere, NULL);

	cGameObject::Setup(vLookAt);

	m_pSateMachnie->ChangeState(cMonsterDetecting::Instance());
}

void cMonster::Setup(ST_SAVEOBJECT wObj)
{
	m_sObjName = wObj.szfileName;
	m_sFolderName = wObj.szFolderName;

	m_vPosition = wObj.vPosition;
	m_stStat.fMaxHp = m_stStat.fHp;

	m_pMesh = new cSkinnedMesh("./Resources/Monster/", StringToChar(m_sObjName));
	m_pMesh->SetAnimationIndex("idle");

	//m_stStat.fAttackRange = D3DXVec3Length(&(m_pMesh->GetMax() + m_pMesh->GetMin()));
	//m_pMesh->GetMin();
	//m_stStat.fAttackRange = 0.6;
	//m_stStat.fTraceRange = 2.5f;
	//m_stStat.fAtk = 10.0f;
	
	D3DXCreateSphere(g_pD3DDevice, m_stStat.fAttackRange, 20, 20, &m_pAttackSphere, NULL);
	D3DXCreateSphere(g_pD3DDevice, m_stStat.fTraceRange, 20, 20, &m_pTraceSphere, NULL);

	cGameObject::Setup(&wObj.vForward);
}

void cMonster::Update()
{
	if(m_stStat.fHp > 0)
		cGameObject::Update();

	if (m_pSateMachnie)
		m_pSateMachnie->Update();
}

void cMonster::Render()
{
	cGameObject::Render();
	//몬스터 렌더
	//기본적으로 그냥 몸뚱아리는 이거 쓸거고
	//이펙트, 파티클같은것들은 각자 몬스터 랜더에서 처리

	D3DXMATRIXA16 matT;
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	
	m_matWorld = m_matLocal * matT;

	if (m_pMesh)
		m_pMesh->UpdateAndRender(&m_matWorld);

	//공격사거리 그리는 부분
	//if (g_pKeyManager->isToggleKey('1'))
	//{
		D3DXMATRIXA16 mat;
		D3DXMatrixTranslation(&mat, 0, m_stStat.fAttackRange / 2, 0);
		m_matWorld *= mat;
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	
		
		if (m_pAttackSphere)
			m_pAttackSphere->DrawSubset(0);
		
		if (m_pTraceSphere)
			m_pTraceSphere->DrawSubset(0);
	//}
	
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	//디버그 정보
	LPD3DXFONT font;
	font = g_pFontManger->GetFont(cFontManager::E_NORMAL);

	char temp[128];
	RECT rc = { DEBUG_STARTX, DEBUG_STARTY + 200, DEBUG_STARTX + 250, DEBUG_STARTY + 315 };

	if (m_pAction)
	{
		//D3DXVECTOR3 v = ((cActionTrace*)m_pAction)->GetDirToTargetX();
		sprintf_s(temp, "dX : %f, %f, %f", m_vPosition.x, m_vPosition.y, m_vPosition.z
			, 128);//m_pMesh->GetCurAnimationName().c_str(), 128);
		rc = { DEBUG_STARTX, DEBUG_STARTY + 300, DEBUG_STARTX + 250, DEBUG_STARTY + 415 };
		font->DrawText(NULL,
			temp,
			128,
			&rc,
			DT_LEFT,
			D3DCOLOR_XRGB(255, 255, 255));
	}

}




//void cMonster::CloneMonster(cMonster* monster)
//{
//	//this->Setup(StringToChar(monster->GetObjName()), NULL);
//	//this->m_pMesh = new cSkinnedMesh("./Resources/Monster/",
//	//	StringToChar(monster->GetObjName()));
//	//this->m_pMesh->SetAnimationIndex("idle");
//
//}


bool cMonster::HandleMessage(const Telegram& msg)
{
	return m_pSateMachnie->HandleMessage(msg);
}