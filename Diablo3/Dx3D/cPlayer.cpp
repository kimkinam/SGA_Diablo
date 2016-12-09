#include "stdafx.h"
#include "cPlayer.h"
#include "cSkinnedMesh.h"
#include "cObj.h"
#include "cAction.h"
#include "cOBB.h"
#include "cTrailRenderer.h"
#include <time.h>
#include "cUIImage.h"
#include "cMonsterGlobalState.h"
#include "cPlayerIdleState.h"

cPlayer::cPlayer()
	//: m_emState(PLAYER_IDLE)
	: m_bIsAtk(false)
	, m_dAttackStartTime(0.0f)
	, m_dAttackTermTime(0.0f)
	, m_pSword(NULL)
	, m_nCurMap(0)
	, m_pSprite(NULL)
	, m_pSphere(NULL)
	, m_pHpTex(NULL)
	, m_pMpTex(NULL)
{
	m_pSateMachnie = new cStateMachine<cPlayer>(this);

	m_pSateMachnie->SetCurState(cPlayerIdleState::Instance());
	m_pSateMachnie->SetGlobalState(cMonsterGlobalState::Instance());


}


cPlayer::~cPlayer()
{
	SAFE_RELEASE(m_pHpTex);
	SAFE_RELEASE(m_pMpTex);
	SAFE_RELEASE(m_pSphere);
	SAFE_RELEASE(m_pSprite);
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

	//칼
	m_pSword = new cObj;
	m_pSword->SetUp("twohandsword.objobj", "./Resources/Object/");

	m_pSword->SetWorldTM(m_pMesh->AttachItem("right_weapon"));

	cGameObject::Setup(vLook);

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

	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);


	D3DXCreateTextureFromFileEx(g_pD3DDevice,
		"./Resources/UI/Hp_Sphere.png",
		2048 * HP_SPHERE_SCALE, 128 * HP_SPHERE_SCALE, 0, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT,
		D3DCOLOR_XRGB(0, 0, 0), 0, 0, &m_pHpTex);

	D3DXCreateTextureFromFileEx(g_pD3DDevice,
		"./Resources/UI/Mp_Sphere.png",
		2048 * HP_SPHERE_SCALE, 128 * HP_SPHERE_SCALE, 0, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT,
		D3DCOLOR_XRGB(0, 0, 0), 0, 0, &m_pMpTex);
	//m_pSphere = new cUIImage;
	
	//m_pSphere->SetTexture("./Resources/UI/Hp_Shpere.png");

	SetSphere(m_Hp);
	SetSphere(m_Mp);

	m_pSateMachnie->ChangeState(cPlayerIdleState::Instance());
}

void cPlayer::Update()
{
	cGameObject::Update();

	if (m_pSateMachnie)
		m_pSateMachnie->Update();

	DWORD dwCurTime = g_pTimeManager->GetTotalSec();
	if (dwCurTime - m_Hp.dwOldAniTime >= m_Hp.dwAniTime)
	{
		m_Hp.dwOldAniTime = dwCurTime;
		m_Hp.nIndex = ++m_Hp.nIndex % m_Hp.nCount;
	}

	dwCurTime = g_pTimeManager->GetTotalSec();
	if (dwCurTime - m_Mp.dwOldAniTime >= m_Mp.dwAniTime)
	{
		m_Mp.dwOldAniTime = dwCurTime;
		m_Mp.nIndex = ++m_Mp.nIndex % m_Mp.nCount;
	}

	if (g_pKeyManager->isOnceKeyDown('B'))
	{
		for (size_t i = 0; i < 60; ++i)
		{
			m_Hp.pRect[i].top += 10;
			m_Hp.pCenter[i].y -= 10;
		}
	}
	if (g_pKeyManager->isOnceKeyDown('H'))
	{
		for (size_t i = 0; i < 60; ++i)
		{
			m_Hp.pRect[i].top -= 10;
			m_Hp.pCenter[i].y += 10;
		}

	}


	AniControl();

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
	
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	m_pSprite->Draw(m_pHpTex,
		&m_Hp.pRect[m_Hp.nIndex],
		&m_Hp.pCenter[m_Hp.nIndex],
		&D3DXVECTOR3(700, 200, 0), 
		D3DCOLOR_XRGB(255, 255, 255));
	
	m_pSprite->Draw(m_pMpTex,
		&m_Mp.pRect[m_Mp.nIndex],
		&m_Mp.pCenter[m_Mp.nIndex],
		&D3DXVECTOR3(600, 200, 0), 
		D3DCOLOR_XRGB(255, 255, 255));

	m_pSprite->End();

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
	this->m_pTrailRenderer->Render();

	
}

void cPlayer::AniControl()
{


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

void cPlayer::SetSphere(ST_HPSPHERE & sphere)
{

	for (size_t i = 0; i < 2; ++i)
	{
		for (size_t j = 0; j < 42; ++j)
		{
			if (i == 1 && j == 18) break;
			RECT rc;
			SetRect(&rc, j * HP_SPHERE_SIZE_X, i * HP_SPHERE_SIZE_Y,
					(j + 1) * HP_SPHERE_SIZE_X, i * HP_SPHERE_SIZE_Y + HP_SPHERE_SIZE_Y);
			D3DXVECTOR3 vCenter;
			vCenter = D3DXVECTOR3(HP_SPHERE_SIZE_X / 2, HP_SPHERE_SIZE_Y / 2, 0);
			
			sphere.pRect.push_back(rc);
			sphere.pCenter.push_back(vCenter);
			
			
		}
	}

	sphere.nCount = 60;
	sphere.nIndex = 0;
	sphere.dwAniTime = g_pTimeManager->GetDeltaTime();
	sphere.dwOldAniTime = g_pTimeManager->GetTotalSec();

	int a = 0;
}

bool cPlayer::HandleMessage(const Telegram & msg)
{
	return m_pSateMachnie->HandleMessage(msg);
}

