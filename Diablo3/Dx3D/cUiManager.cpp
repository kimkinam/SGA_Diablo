#include "stdafx.h"
#include "cUiManager.h"
#include "cUIImage.h"
#include "cUIText.h"
#include "cUIButton.h"
#include "cPlayerManager.h"
#include "cUISkill.h"
#include "cInventory.h"
#include "cPlayer.h"
#include "cGamingScene.h"
cUiManager::cUiManager()
	: m_pHpBar(NULL)
	, m_pSprite(NULL)
	, m_IsClick(false)
	, pBaba_skill_1(NULL)
	, HP_sphere(NULL)
	, MP_sphere(NULL)
	, m_pAniSprite(NULL)
	, m_pHpTex(NULL)
	, m_pMpTex(NULL)
	, m_fCurHp(0.0f)
	, m_fMaxHp(0.0f)
	, m_fMinusHp(0.0f)
	, m_pPlayer(NULL)
	, m_pEnemyBar(NULL)
	, m_bIsEnemyBar(NULL)
	, m_pScene(NULL)
	//, m_fEnemyHP(NULL)
{
	m_ptClickPoint.x = m_ptClickPoint.y = 0;
}


cUiManager::~cUiManager()
{

	if (m_pHpBar)
		m_pHpBar->Destroy();

	
	if (HP_sphere)
		HP_sphere->Destroy();
	
	if (MP_sphere)
		MP_sphere->Destroy();

	if (pBaba_skill_1)
		pBaba_skill_1->Destroy();

	if (m_pEnemyBar)
		m_pEnemyBar->Destroy();

	if (m_pEnemyBarBG)
		m_pEnemyBarBG->Destroy();


	//SAFE_DELETE(m_pPlayerManager);
	SAFE_RELEASE(m_pSprite);

	SAFE_RELEASE(m_pAniSprite);
	SAFE_RELEASE(m_pHpTex);
	SAFE_RELEASE(m_pMpTex);
}

void cUiManager::SetUp()
{
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);
	D3DXCreateSprite(g_pD3DDevice, &m_pAniSprite);
	D3DXMATRIXA16 matS;

	RECT rc_win; // 윈도우 렉트
	GetClientRect(g_hWnd, &rc_win);
	
//================= HpBar =================================================================

	SetUpHpBar(rc_win);
	SetUpEnemyBar(rc_win);
	
////================================스킬 ============================================
	pBaba_skill_1 = new cUISkill;
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.5f, 0.5f, 1);
	pBaba_skill_1->SetmatS(matS);
	pBaba_skill_1->SetSkillTexture("./Resources/UI/휠윈드대기.png","./Resources/UI/힐윈드선택.png" );
	pBaba_skill_1->SetPosition(rc_win.right / 1.85, rc_win.bottom / 1.08, 0);
	pBaba_skill_1->SetTag(cUIObject::Skill_1); // 스킬 테그 만들어 준다 
	pBaba_skill_1->SetCoolTime(3.0f);
//	
////================= 인벤토리 창 ===================================================
//
//Inventory = new cInventory;
//Inventory->Setup();


}

void cUiManager::Update()
{

	if (m_pHpBar)
		m_pHpBar->Update(); // UI 틀
	
	if (m_pEnemyBar)
		m_pEnemyBar->Update();

	if (m_pEnemyBarBG)
		m_pEnemyBarBG->Update();

	//
	//if (HP_sphere)
	//	HP_sphere->Update(); // HP 구
	//
	//if (MP_sphere)
	//	MP_sphere->Update(); // HP 구


	//if (pBaba_skill_1)
	//	pBaba_skill_1->Update();

	HpSphereUpdate();

}

void cUiManager::Render()
{
	//if (HP_sphere)
	//	HP_sphere->Render(m_pSprite); // 
	//
	//if (MP_sphere)
	//	MP_sphere->Render(m_pSprite);
	
	HpSphereRender();

	if (m_pHpBar)
		m_pHpBar->Render(m_pSprite);
	
	if (m_bIsEnemyBar)
	{
		if (m_pEnemyBarBG)
			m_pEnemyBarBG->Render(m_pSprite);

		if (m_pEnemyBar)
			m_pEnemyBar->Render(m_pSprite);
	}

	//if (pBaba_skill_1)
	//	pBaba_skill_1->Render(m_pSprite);	
}

void cUiManager::HpSphereUpdate()
{
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

	m_fCurHp = m_pPlayer->GetStat().fHp;

	if (m_fCurHp > 0 && m_fMaxHp - m_fCurHp > EPSILON)
	{
		m_fMinusHp = m_fMaxHp - m_fCurHp;

		for (size_t i = 0; i < 60; ++i)
		{
			m_Hp.pRect[i].top += (m_fMinusHp);
			m_Hp.pCenter[i].y -= (m_fMinusHp);

			m_fMaxHp = m_fCurHp;
		}
	}
	if (g_pKeyManager->isOnceKeyDown('H'))
	{
		if (m_fCurHp < 1000.0f)
		{
			for (size_t i = 0; i < 60; ++i)
			{
				m_Hp.pRect[i].top -= 10;
				m_Hp.pCenter[i].y += 10;
			}

			m_fCurHp += 10;
		}
		
	}
}

void cUiManager::HpSphereRender()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	D3DXMATRIXA16 matS;
	D3DXMatrixScaling(&matS, 0.8f, 0.8f, 1.f);
	m_pAniSprite->SetTransform(&matS);
	m_pAniSprite->Begin(D3DXSPRITE_ALPHABLEND);
	m_pAniSprite->Draw(m_pHpTex,
		&m_Hp.pRect[m_Hp.nIndex],
		&m_Hp.pCenter[m_Hp.nIndex],
		&D3DXVECTOR3(rc.right / 2 - 45 - 85, rc.bottom + (49 + 75), 0),
		D3DCOLOR_XRGB(255, 255, 255));

	m_pAniSprite->Draw(m_pMpTex,
		&m_Mp.pRect[m_Mp.nIndex],
		&m_Mp.pCenter[m_Mp.nIndex],
		&D3DXVECTOR3(rc.right / 2 + 45 + 410, rc.bottom + (49 + 75), 0),
		D3DCOLOR_XRGB(255, 255, 255));

	m_pAniSprite->End();
}

bool cUiManager::InCollider(cUIObject * pUI)
{
	RECT rc;
	float deltaX = pUI->GetPosition().x;
	float deltaY = pUI->GetPosition().y;

	SetRect(&rc, pUI->GetCollider().nStartX + deltaX
		, pUI->GetCollider().nStartY + deltaY,
		pUI->GetCollider().nWidth + deltaX,
		pUI->GetCollider().nHeight + deltaY);

	if (PtInRect(&rc, g_ptMouse))
		return true;

	return false;
}

void cUiManager::OnClick(cUIButton * pSender)
{
	//cUIButton* pTextUI = (cUIButton*)m_pInven->FindChildByTag(cUIObject::Button_1);
	
	if (pSender->GetTag() == cUIObject::Skill_1)
	{
		Skill_FileName = "./Resources/UI/휠윈드선택.png";
	}
	
}

void cUiManager::SetUpEnemyBar(RECT rc)
{
	D3DXMATRIXA16 matS;

	cUIImage* pEnemyBarBG = new cUIImage;
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.7f, 0.7f, 1);
	pEnemyBarBG->SetmatS(matS);
	pEnemyBarBG->SetTexture("./Resources/UI/Enemy_HPbar_BG.png");
	pEnemyBarBG->SetPosition(rc.right / 2 - pEnemyBarBG->GetCollider().nWidth / 2,
		rc.top + pEnemyBarBG->GetCollider().nHeight * 1.5f, 0);
	m_pEnemyBarBG = pEnemyBarBG;

	cUIImage* pEnemyBar = new cUIImage;
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.7f, 0.7f, 1);
	pEnemyBar->SetmatS(matS);
	pEnemyBar->SetTexture("./Resources/UI/Enemy_HPbar_Red.png");
	pEnemyBar->SetPosition(rc.right / 2 - pEnemyBar->GetCollider().nWidth / 2 + 0.5,
		rc.top + pEnemyBar->GetCollider().nHeight * 4.33f, 0);
	m_pEnemyBar = pEnemyBar;
}

void cUiManager::SetUpEnemyBar(RECT rc, char* textureBGName, char* textureName)
{
	D3DXMATRIXA16 matS;

	cUIImage* pEnemyBarBG = new cUIImage;
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.7f, 0.7f, 1);
	pEnemyBarBG->SetmatS(matS);
	pEnemyBarBG->SetTexture(textureBGName);
	pEnemyBarBG->SetPosition(rc.right / 2 - pEnemyBarBG->GetCollider().nWidth / 2,
		rc.top + pEnemyBarBG->GetCollider().nHeight * 1.5f, 0);
	m_pEnemyBarBG = pEnemyBarBG;

	cUIImage* pEnemyBar = new cUIImage;
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.7f, 0.7f, 1);
	pEnemyBar->SetmatS(matS);
	pEnemyBar->SetTexture(textureName);
	pEnemyBar->SetPosition(rc.right / 2 - pEnemyBar->GetCollider().nWidth / 2 + 0.5,
		rc.top + pEnemyBar->GetCollider().nHeight * 4.75f, 0);
	m_pEnemyBar = pEnemyBar;
}

void cUiManager::SetUpHpBar(RECT rc)
{
	D3DXMATRIXA16 matS;

	cUIImage* pBackGround = new cUIImage;
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.5f, 0.5f, 1);
	pBackGround->SetmatS(matS);
	pBackGround->SetTexture("./Resources/UI/유아이바.png");
	pBackGround->SetPosition(rc.right / 2 - pBackGround->GetCollider().nWidth/2, 
		rc.bottom - pBackGround->GetCollider().nHeight, 0);
	m_pHpBar = pBackGround;


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

	//if (m_fCurHp)
	m_fCurHp = m_pPlayer->GetStat().fHp;

	//if (m_fMaxHp)
	m_fMaxHp = m_pPlayer->GetStat().fMaxHp;

}

void cUiManager::SetSphere(ST_HPSPHERE & sphere)
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

}

