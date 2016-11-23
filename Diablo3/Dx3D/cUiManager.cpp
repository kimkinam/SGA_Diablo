#include "stdafx.h"
#include "cUiManager.h"
#include "cUIImage.h"
#include "cUIText.h"
#include "cUIButton.h"
#include "cPlayerManager.h"

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

	//SAFE_DELETE(m_pPlayerManager);
	SAFE_RELEASE(m_pSprite);
}

void cUiManager::SetUp()
{
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);
	D3DXMATRIXA16 matS;
	
//================= HpBar ===============================================

	cUIImage* pBackGround = new cUIImage;
	pBackGround->SetPosition(200, 440, 0);
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 1.2f, 1.0f, 1);
	pBackGround->SetmatS(matS);
	pBackGround->SetTexture("./Resources/UI/밑ui.png");
	m_pHpBar = pBackGround;

//================= 인벤토리 창 ========================================================================

	cUIImage* pInventory = new cUIImage;
	pInventory->SetPosition(810, 0, 0);
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.54f, 0.44f, 1);
	pInventory->SetmatS(matS);
	pInventory->SetTexture("./Resources/UI/인벤토리.png");
	m_pInven = pInventory;

	cUIButton* Inven_Button = new cUIButton; // 인벤토리 열고닫기 (엑스)
	Inven_Button->SetPosition(300, 0, 0);
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.5f, 0.5f, 1);
	Inven_Button->SetmatS(matS);
	Inven_Button->SetTexture("./Resources/UI/버튼오프.png", "./Resources/UI/버튼오프.png", "./Resources/UI/버튼온.png");
	m_pInven->AddChild(Inven_Button);

	m_pInven->SetDraw(false);

	//cUIImage* Item = new cUIImage;
	//Item->SetPosition(0, 0, 0);
	//D3DXMatrixIdentity(&matS);
	//D3DXMatrixScaling(&matS, 0.54f, 0.44f, 1);
	//Item->SetmatS(matS);
	//Item->SetTexture("./UI/활.png");
	//m_pInven->AddChild(Item);

	/*cUIImage* pTileBar = new cUIImage;
	pTileBar->SetPosition(100, 50, 0);
	D3DXMatrixScaling(&matS, 0.25f, 0.3f, 1);
	pTileBar->SetmatS(matS);
	pTileBar->SetTexture("./UI/title-bar.jpg");
	m_pRoot = pTileBar;

	cUIImage* pBackGround = new cUIImage;
	pBackGround->SetPosition(0, 50, 0);
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.25f, 0.2f, 1);
	pBackGround->SetmatS(matS);
	pBackGround->SetTexture("./UI/background.jpg");
	m_pRoot->AddChild(pBackGround);

	cUIImage* pQuestBox = new cUIImage;
	pQuestBox->SetPosition(
		pBackGround->GetCollider().nWidth / 2 - 125,
		pBackGround->GetCollider().nHeight / 2 - 110);
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.25f, 0.2f, 1);
	pQuestBox->SetmatS(matS);
	pQuestBox->SetTexture("./UI/writing-area.png");
	m_pRoot->AddChild(pQuestBox);

	cUIText* pQuestBoxText = new cUIText;
	pQuestBoxText->SetText("퀘스트를 수행하시겠습니까?");
	pQuestBoxText->SetFont(g_pFontManger->GetFont(cFontManager::E_NORMAL));
	pQuestBoxText->SetPosition(10, 100);
	pQuestBoxText->SetDrawTextFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	pQuestBoxText->SetTextColor(D3DCOLOR_XRGB(0, 0, 0));
	pQuestBoxText->SetTag(2);
	m_pRoot->AddChild(pQuestBoxText);
	SAFE_RELEASE(pQuestBoxText);

	cUIButton* pButton1 = new cUIButton;
	pButton1->SetPosition(20, 350);
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.2f, 0.2f, 1);
	pButton1->SetmatS(matS);
	pButton1->SetTexture("./UI/bt1_none.png", "./UI/bt1_hover.png", "./UI/bt1_pressed.png");
	pButton1->SetDelegate(this);
	pButton1->SetTag(3);
	m_pRoot->AddChild(pButton1);

	cUIButton* pButton2 = new cUIButton;
	pButton2->SetPosition(160, 350);
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.2f, 0.2f, 1);
	pButton2->SetmatS(matS);
	pButton2->SetTexture("./UI/bt2_none.png", "./UI/bt2_hover.png", "./UI/bt2_pressed.png");
	pButton2->SetDelegate(this);
	pButton2->SetTag(4);
	m_pRoot->AddChild(pButton2);*/

	
}

void cUiManager::Update()
{
	if (m_pInven)
		m_pInven->Update();

	if (m_pHpBar)
		m_pHpBar->Update();

	if (g_pKeyManager->isOnceKeyDown('I'))
	{
		if (!m_pInven->GetIsDraw()) m_pInven->SetDraw(true);
		else m_pInven->SetDraw(false);
	}
}

void cUiManager::Render()
{


	if (m_pInven)
		m_pInven->Render(m_pSprite);

	if (m_pHpBar)
		m_pHpBar->Render(m_pSprite);

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
	//cUIText* pTextUI = (cUIText*)m_pInven->FindChildByTag(2);
	//pTextUI->SetDrawTextFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	//
	//if (pSender->GetTag() == 3)
	//{
	//	pTextUI->SetText("확인");
	//	m_pInven->SetDraw(false);
	//}
	//else if (pSender->GetTag() == 4)
	//{
	//	pTextUI->SetText("취소");
	//	//pTextView->SetText("취소");
	//}

}
