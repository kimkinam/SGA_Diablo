#include "stdafx.h"
#include "cUiManager.h"
#include "cUIImage.h"
#include "cUIText.h"
#include "cUIButton.h"
#include "cPlayerManager.h"
#include "cUISkill.h"
#include "cInventory.h"

cUiManager::cUiManager()
	: m_pHpBar(NULL)
	, m_pInven(NULL)
	, m_pSprite(NULL)
	, m_IsClick(false)
	, m_pPlayerManager(NULL)
	, Inventory(NULL)
{
	m_ptClickPoint.x = m_ptClickPoint.y = 0;
}


cUiManager::~cUiManager()
{
	SAFE_RELEASE(Inventory);

	if (m_pHpBar)
		m_pHpBar->Destroy();

	if (m_pInven)
		m_pInven->Destroy();
	
	if (HP_sphere)
		HP_sphere->Destroy();
	
	if (MP_sphere)
		MP_sphere->Destroy();

	if (pBaba_skill_1)
		pBaba_skill_1->Destroy();


	//SAFE_DELETE(m_pPlayerManager);
	SAFE_RELEASE(m_pSprite);
}

void cUiManager::SetUp()
{
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);
	D3DXMATRIXA16 matS;

	RECT rc_win; // ������ ��Ʈ
	GetClientRect(g_hWnd, &rc_win);
	
//================= HpBar =================================================================

	SetUpHpBar(rc_win);
	
//================================��ų ============================================
	pBaba_skill_1 = new cUISkill;
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.5f, 0.5f, 1);
	pBaba_skill_1->SetmatS(matS);
	pBaba_skill_1->SetSkillTexture("./Resources/UI/��������.png","./Resources/UI/�����弱��.png" );
	pBaba_skill_1->SetPosition(rc_win.right / 1.85, rc_win.bottom / 1.08, 0);
	pBaba_skill_1->SetTag(cUIObject::Skill_1); // ��ų �ױ� ����� �ش� 
	pBaba_skill_1->SetCoolTime(3.0f);
	
//================= �κ��丮 â ===================================================

Inventory = new cInventory;
Inventory->Setup();

}

void cUiManager::Update()
{
	if (m_pInven)
		m_pInven->Update();

	if (m_pHpBar)
		m_pHpBar->Update(); // UI Ʋ
	
	if (HP_sphere)
		HP_sphere->Update(); // HP ��
	
	if (MP_sphere)
		MP_sphere->Update(); // HP ��

	if (pBaba_skill_1)
		pBaba_skill_1->Update();

	if (Inventory)
		Inventory->Update();

	for (int i = 0; i < Image_vec.size(); i++)
	{
		
	}
	
}

void cUiManager::Render()
{
	if (Inventory)
		Inventory->Render();

	if (HP_sphere)
		HP_sphere->Render(m_pSprite); // 
	
	if (MP_sphere)
		MP_sphere->Render(m_pSprite);
	
	if (m_pHpBar)
		m_pHpBar->Render(m_pSprite);
	
	if (pBaba_skill_1)
		pBaba_skill_1->Render(m_pSprite);	
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
		Skill_FileName = "./Resources/UI/�����弱��.png";
	}
	
}

void cUiManager::SetUpHpBar(RECT rc)
{
	D3DXMATRIXA16 matS;

	cUIImage* pBackGround = new cUIImage;
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.5f, 0.5f, 1);
	pBackGround->SetmatS(matS);
	pBackGround->SetTexture("./Resources/UI/�����̹�.png");
	pBackGround->SetPosition(rc.right / 2 - pBackGround->GetCollider().nWidth/2, rc.bottom - pBackGround->GetCollider().nHeight, 0);
	m_pHpBar = pBackGround;



	cUIImage* Hp_bar = new cUIImage; // ü�� �۶�
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.48f, 0.48f, 1);
	Hp_bar->SetmatS(matS);
	Hp_bar->SetTexture("./Resources/UI/HP_G.png");
	Hp_bar->SetPosition(84, 3, 0);
	m_pHpBar->AddChild(Hp_bar);

	cUIImage* Hp_C = new cUIImage; // ü�� �� (������)
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.485f, 0.485f, 1);
	Hp_C->SetmatS(matS);
	Hp_C->SetTexture("./Resources/UI/HP_C.png");
	D3DXVECTOR3* HPSpear_Position;
	D3DXVECTOR3* back;
	back = &pBackGround->GetPosition();
	HPSpear_Position = &Hp_bar->GetPosition();
	Hp_C->SetPosition(*HPSpear_Position + *back);
	HP_sphere = Hp_C;

	cUIImage* Mp_bar = new cUIImage; // ���� �۶�
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.48f, 0.48f, 1);
	Mp_bar->SetmatS(matS);
	Mp_bar->SetTexture("./Resources/UI/MANA_G.png");
	Mp_bar->SetPosition(550, 3, 0);
	m_pHpBar->AddChild(Mp_bar);

	cUIImage* Mp_C = new cUIImage; // ���� �� (�Ķ���)
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.48f, 0.48f, 1);
	Mp_C->SetmatS(matS);
	Mp_C->SetTexture("./Resources/UI/MANA_C.png");
	D3DXVECTOR3* MPSpear_Position;
	MPSpear_Position = &Mp_bar->GetPosition();
	Mp_C->SetPosition(*MPSpear_Position+*back);
	MP_sphere = Mp_C;

}

