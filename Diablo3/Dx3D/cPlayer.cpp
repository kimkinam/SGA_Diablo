#include "stdafx.h"
#include "cPlayer.h"
#include "cSkinnedMesh.h"
#include "cObj.h"
#include "cAction.h"
#include "cOBB.h"
#include "cTrailRenderer.h"
#include <time.h>
#include "cUIImage.h"
#include "cGameObjectGlobalState.h"
#include "cPlayerIdleState.h"
#include "cPlayerWarCryState.h"

cPlayer::cPlayer()
	//: m_emState(PLAYER_IDLE)
	: m_dAttackStartTime(0.0f)
	, m_dAttackTermTime(0.0f)
	, m_pSword(NULL)
	, m_nCurMap(0)
	, m_pSphere(NULL)
	, m_bIsWhirl(false)
{
	m_pSateMachnie = new cStateMachine<cPlayer>(this);

	m_pSateMachnie->SetCurState(cPlayerIdleState::Instance());
	m_pSateMachnie->SetGlobalState(cGameObjectGlobalState::Instance());


}


cPlayer::~cPlayer()
{
	
	SAFE_RELEASE(m_pSphere);
	SAFE_RELEASE(m_pSword);
	SAFE_RELEASE(m_pTrailRenderer);
}

void cPlayer::Setup(D3DXVECTOR3* vLook)
{
	//바바
	m_pMesh = new cSkinnedMesh("./Resources/Player/", "babarian.X");

	m_pMesh->GetBoundingSphere()->vCenter.y = 0.5f;

	m_pOBB = new cOBB;
	D3DXVECTOR3 vMin, vMax;
	
	vMin = m_pMesh->GetMin();
	vMax = m_pMesh->GetMax();
	
	vMin.x += 0.6f;
	vMax.x -= 0.6f;
	vMax.y -= 0.2f;

	m_pOBB->Setup(vMin, vMax);

	this->GetStat().chType = CHARACTER_BARBARIAN;

	//칼
	m_pSword = new cObj;
	m_pSword->SetUp("twohandsword.objobj", "./Resources/Object/");

	m_pSword->SetWorldTM(m_pMesh->AttachItem("right_weapon"));

	cGameObject::Setup(vLook);

	m_stStat.fHp = 1000.0f;
	m_stStat.fMaxHp = 1000.0f;
	m_stStat.fMaxMp = 1000.0f;
	
	m_stStat.fAtk = 10.0f;
	m_stStat.fAttackRange = this->GetMesh()->GetBoundingSphere()->fRadius * 4.0f;
	m_stStat.fSpeed = 0.1f;

	this->TrailTexSetUp("./Resources/Images/Trail/Trail_angelic.dds");
	this->TrailTexSetUp("./Resources/Images/Trail/Trail_arcane.dds");
	this->TrailTexSetUp("./Resources/Images/Trail/Trail_attack.dds");
	this->TrailTexSetUp("./Resources/Images/Trail/Trail_basic_sharp.dds");
	this->TrailTexSetUp("./Resources/Images/Trail/Trail_blood.dds");
	this->TrailTexSetUp("./Resources/Images/Trail/Trail_cold.dds");
	this->TrailTexSetUp("./Resources/Images/Trail/Trail_fire.dds");
	this->TrailTexSetUp("./Resources/Images/Trail/Trail_gold_01.dds");
	this->TrailTexSetUp("./Resources/Images/Trail/trail_holy.dds");
	this->TrailTexSetUp("./Resources/Images/Trail/Trail_lightning.dds");
	this->TrailTexSetUp("./Resources/Images/Trail/Trail_magicFind.dds");
	this->TrailTexSetUp("./Resources/Images/Trail/Trail_mana.dds");
	this->TrailTexSetUp("./Resources/Images/Trail/Trail_phys.dds");
	this->TrailTexSetUp("./Resources/Images/Trail/Trail_phys_sharp.dds");
	this->TrailTexSetUp("./Resources/Images/Trail/Trail_poison.dds");
	this->TrailTexSetUp("./Resources/Images/Trail/Trail_rainbow.dds");
	this->TrailTexSetUp("./Resources/Images/Trail/Trail_spellDamage.dds");
	this->TrailTexSetUp("./Resources/Images/Trail/TrailTest.png");

	this->m_pTrailRenderer = new cTrailRenderer();
	m_pTrailRenderer->SetParent((D3DXMATRIXA16*)m_pSword->GetWorldTM()); //m_pMesh->AttachItem16("right_weapon"));
	m_nTrailIndex = 0;


	this->m_pTrailRenderer->SetUp(
		0.4f,					//꼬리 라이브 타임 ( 이게 크면 환영큐 사이즈가 커지고 꼬리가 오랬동안 남아있다 )
		0.8f,					//폭
		g_pTextureManager->GetTexture("./Resources/Images/Trail/Trail_basic_sharp.dds"),	//메인 Texture
		D3DXCOLOR(1, 1, 1, 1),												//메인 Texture 로 그릴때 컬러
		g_pTextureManager->GetTexture("./Resources/Images/Trail/TrailTest.png")	//외곡 그릴때 외곡 노말
	);
	//m_pTrailRenderer->SetTrailTexture(m_vecTrailTex[3]);



	

	m_pSateMachnie->ChangeState(cPlayerIdleState::Instance());
}

void cPlayer::Update()
{
	cGameObject::Update();

	TrailUpdate();

	//m_pMesh->GetBoundingSphere()->vCenter.y += 1.5f;
	if (m_pSateMachnie)
		m_pSateMachnie->Update();

	//if (g_pKeyManager->isOnceKeyDown(VK_UP))
	//{
	//	//g_pAIManager->GetAIBaseFromID(0)->GetStat().fHp -= 100.0f;
	//	
	//	m_stStat.fHp -= 100.0f;
	//	g_pMessageManager->MessageSend(0.0f, this->GetID(), this->GetID(),
	//		MESSAGE_TYPE::MSG_ATTACK, NULL);
	//}
	

	



	//if (m_vPosition.x < 0 && m_vPosition.z < 0)	//왼쪽아래
	//	m_nCurMap = 0;
	//if (m_vPosition.x < 0 && m_vPosition.z > 0)	//왼쪽위
	//	m_nCurMap = 1;
	//if (m_vPosition.x > 0 && m_vPosition.z > 0)	//오른쪽위
	//	m_nCurMap = 2;
	//if (m_vPosition.x > 0 && m_vPosition.z < 0)	//오른쪽아래
	//	m_nCurMap = 3;

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
	
	int animMaxCount = m_pMesh->GetAnimController()->GetNumAnimationSets();

	if (g_pKeyManager->isToggleKey(VK_OEM_1))
	{
		for (int i = 0; i < animMaxCount; i++)
		{
			//LPD3DXFONT
			RECT rcTextArea = { WINSIZE_X / 2, i * 30, 0, 0 };

			LPD3DXANIMATIONSET animSet = NULL;
			m_pMesh->GetAnimController()->GetAnimationSet(i, &animSet);
			if (!animSet) continue;
			string str = animSet->GetName();
			g_pFontManger->GetFont(cFontManager::E_NORMAL)->DrawTextA(
				NULL,
				str.c_str(),		//string 의 내용을 char* 로 전달
				-1,
				&rcTextArea,
				DT_LEFT | DT_NOCLIP,		// DT_NOCLIP 영영은 무시됨
				0xffffff00);
		}
	}
	

	
}

void cPlayer::PlayerPosition()
{
	//m_pPosition = &cGameObject::GetPosition();
}

void cPlayer::TrailRender()
{
	this->m_pTrailRenderer->Render();
}

void cPlayer::TrailUpdate()
{
	if (g_pKeyManager->isOnceKeyDown(VK_OEM_4))
	{
		m_nTrailIndex--;
		if (m_nTrailIndex < 0)
		{

			m_nTrailIndex = m_vecTrailTex.size() - 1;
		}
		m_pTrailRenderer->SetTrailTexture(m_vecTrailTex[m_nTrailIndex]);
	}
	if (g_pKeyManager->isOnceKeyDown(VK_OEM_6))
	{
		m_nTrailIndex++;
		if (m_nTrailIndex >= m_vecTrailTex.size())
		{
			m_nTrailIndex = 0;
		}
		m_pTrailRenderer->SetTrailTexture(m_vecTrailTex[m_nTrailIndex]);
	}

	this->m_pTrailRenderer->Update(g_pTimeManager->GetDeltaTime());

}

void cPlayer::TrailTexSetUp(const char * texFileName)
{
	LPDIRECT3DTEXTURE9 tex = NULL;
	tex = g_pTextureManager->GetTexture(texFileName);

	if (tex)
		m_vecTrailTex.push_back(tex);
}



bool cPlayer::HandleMessage(const Telegram & msg)
{
	return m_pSateMachnie->HandleMessage(msg);
}

