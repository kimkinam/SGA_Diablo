#include "stdafx.h"
#include "cUiManager.h"
#include "cUIImage.h"
#include "cUIText.h"
#include "cUIButton.h"
#include "cPlayerManager.h"
#include "cUISkill.h"

cUiManager::cUiManager()
	: m_pHpBar(NULL)
	, m_pInven(NULL)
	, m_pSprite(NULL)
	, m_IsClick(false)
	, m_pPlayerManager(NULL)
{
	m_ptClickPoint.x = m_ptClickPoint.y = 0;
}


cUiManager::~cUiManager()
{
	if (m_pHpBar)
		m_pHpBar->Destroy();

	if (m_pInven)
		m_pInven->Destroy();

	//if (HP_sphere)
	//	HP_sphere->Destroy();
	//
	//if (MP_sphere)
	//	MP_sphere->Destroy();

	if (pBaba_skill_1)
		pBaba_skill_1->Destroy();


	//SAFE_DELETE(m_pPlayerManager);
	SAFE_RELEASE(m_pSprite);
}

void cUiManager::SetUp()
{
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);
	D3DXMATRIXA16 matS;

	RECT rc_win; // 윈도우 렉트
	GetClientRect(g_hWnd, &rc_win);
	
//================= HpBar =================================================================

	SetUpHpBar(rc_win);
	
//================================스킬 ============================================
	pBaba_skill_1 = new cUISkill;
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.5f, 0.5f, 1);
	pBaba_skill_1->SetmatS(matS);
	pBaba_skill_1->SetSkillTexture("./Resources/UI/휠윈드대기.png","./Resources/UI/힐윈드선택.png" );
	pBaba_skill_1->SetPosition(rc_win.right / 1.87, rc_win.bottom / 1.08, 0);
	pBaba_skill_1->SetTag(cUIObject::Skill_1); // 스킬 테그 만들어 준다 
	pBaba_skill_1->SetCoolTime(3.0f);
	
//================= 인벤토리 창 ===================================================

	cUIImage* pInventory = new cUIImage;
	pInventory->SetPosition(rc_win.right-325, 0, 0);
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.54f, 0.44f, 1);
	pInventory->SetmatS(matS);
	pInventory->SetTexture("./Resources/UI/빈인벤.png");
	pInventory->AddRef();
	m_pInven = pInventory;

	cUIButton* Inven_Button = new cUIButton; // 인벤토리 열고닫기 (엑스)
	Inven_Button->SetPosition(300, 0, 0);
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.5f, 0.5f, 1);
	Inven_Button->SetmatS(matS);
	Inven_Button->SetTexture("./Resources/UI/버튼오프.png", "./Resources/UI/버튼오프.png", "./Resources/UI/버튼온.png");
	Inven_Button->SetDelegate(this);
	Inven_Button->SetTag(cUIObject::Button_1);
	m_pInven->AddChild(Inven_Button);
	m_pInven->SetDraw(false);

		/*Inven_head,
		Inven_shoulder,
		Inven_chest,
		Inven_boots,
		Inven_pants,
		Inven_belt,
		Inven_arm,
		Inven_wrist
		Inven_sword*/


//====================인벤토리, 상단 아이템 ===================================

	Image_vec.resize(10);


	Inven_head = new cUIImage;
	Inven_head->SetPosition(194, 85, 0);
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.54f, 0.44f, 1);
	Inven_head->SetmatS(matS);
	Inven_head->SetTexture("./Resources/UI/인벤머리.png");
	Inven_head->SetTag(cUIObject::Inven_head);
	m_pInven->AddChild(Inven_head);
	Image_vec.push_back(Inven_head);

	Inven_shoulder = new cUIImage;
	Inven_shoulder->SetPosition(150, 95, 0);
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.54f, 0.44f, 1);
	Inven_shoulder->SetmatS(matS);
	Inven_shoulder->SetTexture("./Resources/UI/인벤어깨.png");
	Inven_shoulder->SetTag(cUIObject::Inven_shoulder);
	m_pInven->AddChild(Inven_shoulder);
	Image_vec.push_back(Inven_shoulder);


	Inven_chest = new cUIImage;
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.54f, 0.44f, 1);
	Inven_chest->SetPosition(190, 115, 0);
	Inven_chest->SetmatS(matS);
	Inven_chest->SetTexture("./Resources/UI/인벤갑옷.png");
	Inven_chest->SetTag(cUIObject::Inven_chest);
	m_pInven->AddChild(Inven_chest);
	Image_vec.push_back(Inven_chest);

	Inven_boots = new cUIImage;
	Inven_boots->SetPosition(194, 225, 0);
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.54f, 0.44f, 1);
	Inven_boots->SetmatS(matS);
	Inven_boots->SetTexture("./Resources/UI/인벤부츠.png");
	Inven_boots->SetTag(cUIObject::Inven_boots);
	m_pInven->AddChild(Inven_boots);
	Image_vec.push_back(Inven_boots);

	Inven_pants = new cUIImage;
	Inven_pants->SetPosition(194, 184, 0);
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.54f, 0.44f, 1);
	Inven_pants->SetmatS(matS);
	Inven_pants->SetTexture("./Resources/UI/인벤바지.png");
	Inven_pants->SetTag(cUIObject::Inven_pants);
	m_pInven->AddChild(Inven_pants);
	Image_vec.push_back(Inven_pants);

	Inven_belt = new cUIImage;
	Inven_belt->SetPosition(190, 167, 0);
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.54f, 0.44f, 1);
	Inven_belt->SetmatS(matS);
	Inven_belt->SetTexture("./Resources/UI/인벤벨트.png");
	Inven_belt->SetTag(cUIObject::Inven_belt);
	m_pInven->AddChild(Inven_belt);
	Image_vec.push_back(Inven_belt);

	Inven_arm = new cUIImage;
	Inven_arm->SetPosition(136, 139, 0);
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.54f, 0.44f, 1);
	Inven_arm->SetmatS(matS);
	Inven_arm->SetTexture("./Resources/UI/인벤팔.png");
	Inven_arm->SetTag(cUIObject::Inven_arm);
	m_pInven->AddChild(Inven_arm);
	Image_vec.push_back(Inven_arm);

	Inven_wrist = new cUIImage;
	Inven_wrist->SetPosition(253, 138, 0);
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.54f, 0.44f, 1);
	Inven_wrist->SetmatS(matS);
	Inven_wrist->SetTexture("./Resources/UI/인벤손목.png");
	Inven_wrist->SetTag(cUIObject::Inven_wrist);
	m_pInven->AddChild(Inven_wrist);
	Image_vec.push_back(Inven_wrist);

	Inven_sword = new cUIImage;
	Inven_sword->SetPosition(136, 207, 0);
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.54f, 0.44f, 1);
	Inven_sword->SetmatS(matS);
	Inven_sword->SetTexture("./Resources/UI/인벤칼.png");
	Inven_sword->SetTag(cUIObject::Inven_sword);
	m_pInven->AddChild(Inven_sword);
	Image_vec.push_back(Inven_sword);

}

void cUiManager::Update()
{
	if (m_pInven)
		m_pInven->Update();

	if (m_pHpBar)
		m_pHpBar->Update(); // UI 틀
	
	if (HP_sphere)
		HP_sphere->Update(); // HP 구
	
	if (MP_sphere)
		MP_sphere->Update(); // HP 구

	if (pBaba_skill_1)
		pBaba_skill_1->Update();


	for (int i = 0; i < Image_vec.size(); i++)
	{
		
	}


	if (g_pKeyManager->isOnceKeyDown('I')) // 키보드  i 로 열기 
	{
		if (!m_pInven->GetIsDraw()) m_pInven->SetDraw(true);
		else m_pInven->SetDraw(false);
	}

	//if (g_pKeyManager->isStayKeyDown(VK_RBUTTON))
	//{
	//	cUIButton* skillBtn = (cUIButton*)m_pHpBar->FindChildByTag(cUIObject::Skill_1);
	//	skillBtn->SetIsClick(true);
	//	m_bIsClick = true;
	//	if (m_emButtonState == E_OVER)
	//		m_emButtonState = E_CLICKED;
	//}
	//else
	//{
	//	m_bIsClick = false;
	//	if (m_emButtonState == E_CLICKED)
	//	{
	//		if (m_pDelegate)
	//			m_pDelegate->OnClick(this);
	//	}
	//	m_emButtonState = E_OVER;
	//}
	
}

void cUiManager::Render()
{

	if (m_pInven)
		m_pInven->Render(m_pSprite);

	if (HP_sphere)
		HP_sphere->Render(m_pSprite); // 
	
	if (MP_sphere)
		MP_sphere->Render(m_pSprite);
	
	if (m_pHpBar)
		m_pHpBar->Render(m_pSprite);
	
	if (pBaba_skill_1)
		pBaba_skill_1->Render(m_pSprite);
	
	
}
//
//void cUiManager::UITranslation(cUIObject* pRoot)
//{
//	if (g_pKeyManager->isOnceKeyDown('I') && !m_pRoot->GetIsDraw())
//	{
//		cUIText* pTextUI = (cUIText*)m_pRoot->FindChildByTag(2);
//		pTextUI->SetText("퀘스트를 수행하시겠습니까?");
//		m_pRoot->SetDraw(true);
//	}
//
//	if (g_pKeyManager->isOnceKeyDown(VK_LBUTTON))
//	{
//		if (InCollider(m_pRoot))
//		{
//			m_IsClick = true;
//			m_ptClickPoint = g_ptMouse;
//		}
//
//	}
//
//	if (g_pKeyManager->isOnceKeyUp(VK_LBUTTON))
//	{
//		m_IsClick = false;
//	}
//
//	if (m_IsClick)
//	{
//		int deltaX = g_ptMouse.x - m_ptClickPoint.x;
//		int deltaY = g_ptMouse.y - m_ptClickPoint.y;
//		
//		m_pRoot->GetPosition().x += deltaX;
//		m_pRoot->GetPosition().y += deltaY;
//
//		m_ptClickPoint = g_ptMouse;
//	}
//}

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
	
	if (pSender->GetTag() == cUIObject::Button_1)
	{
		m_pInven->SetDraw(false);
	}

	if (pSender->GetTag() == cUIObject::Skill_1)
	{
		Skill_FileName = "./Resources/UI/휠윈드선택.png";
	}
	
}

void cUiManager::Itemswap(cUIImage* Findimage)
{

	switch (Findimage->GetTag())
	{
	case cUIObject::Inven_arm:
		//Findimage->SetTexture();
		break;
	case cUIObject::Inven_head:
		break;
	case cUIObject::Inven_chest:
		break;
	case cUIObject::Inven_boots:
		break;
	case cUIObject::Inven_pants:
		break;
	case cUIObject::Inven_belt:
		break;
	case cUIObject::Inven_wrist:
		break;
	case cUIObject::Inven_sword:
		break;
	case cUIObject::Inven_shoulder:
		break;
	default:
		break;
	}

}

void cUiManager::SetUpHpBar(RECT rc)
{
	D3DXMATRIXA16 matS;

	cUIImage* pBackGround = new cUIImage;
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.5f, 0.5f, 1);
	pBackGround->SetmatS(matS);
	pBackGround->SetTexture("./Resources/UI/유아이바.png");
	pBackGround->SetPosition(rc.right / 5.5, rc.bottom - pBackGround->GetCollider().nHeight, 0);
	m_pHpBar = pBackGround;


	cUIImage* Hp_C = new cUIImage; // 체력 구 (빨간색)
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.485f, 0.485f, 1);
	Hp_C->SetmatS(matS);
	Hp_C->SetTexture("./Resources/UI/HP_C.png");
	Hp_C->SetPosition(rc.right / 3.94, rc.bottom / 1.2, 0);
	HP_sphere = Hp_C;


	cUIImage* Hp_bar = new cUIImage; // 체력 글라스
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.48f, 0.48f, 1);
	Hp_bar->SetmatS(matS);
	Hp_bar->SetTexture("./Resources/UI/HP_G.png");
	Hp_bar->SetPosition(84, 3, 0);
	m_pHpBar->AddChild(Hp_bar);

	cUIImage* Mp_C = new cUIImage; // 마나 구 (파란색)
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.48f, 0.48f, 1);
	Mp_C->SetmatS(matS);
	Mp_C->SetTexture("./Resources/UI/MANA_C.png");
	Mp_C->SetPosition(rc.right / 1.5, rc.bottom / 1.2, 0);
	MP_sphere = Mp_C;



	cUIImage* Mp_bar = new cUIImage; // 마나 글라스
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.48f, 0.48f, 1);
	Mp_bar->SetmatS(matS);
	Mp_bar->SetTexture("./Resources/UI/MANA_G.png");
	Mp_bar->SetPosition(550, 3, 0);
	m_pHpBar->AddChild(Mp_bar);

}

