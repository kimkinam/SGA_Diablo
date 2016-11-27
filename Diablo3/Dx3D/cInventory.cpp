#include "stdafx.h"
#include "cInventory.h"
#include "cUIImage.h"
#include "cUIItem.h"
#include "cUIButton.h"
#include "cUIText.h"
#include "cItem.h"


cInventory::cInventory()
	: m_pInvenInfo(NULL)
	, m_pInvenItem(NULL)
	, m_pInvenStat(NULL)
	, m_pSprite(NULL)
{
}


cInventory::~cInventory()
{
	if (m_pInvenItem)
		m_pInvenItem->Destroy();

	if (m_pInvenInfo)
		m_pInvenInfo->Destroy();

	if (m_pInvenStat)
		m_pInvenStat->Destroy();

	SAFE_DELETE(m_pCurItem);
	SAFE_RELEASE(Inven_head);
	SAFE_RELEASE(Inven_chest);
	SAFE_RELEASE(Inven_boots);
	SAFE_RELEASE(Inven_pants);
	SAFE_RELEASE(Inven_belt);
	SAFE_RELEASE(Inven_arm);
	SAFE_RELEASE(Inven_wrist);
	SAFE_RELEASE(Inven_sword);
	SAFE_RELEASE(Inven_shoulder);
}	

void cInventory::Setup()
{
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

	SetupInven_Item();
	SetupInven_Info();
	SetupInven_Stat();
}

void cInventory::Update()
{
	if (m_pInvenInfo)
		m_pInvenInfo->Update();

	if (g_pKeyManager->isOnceKeyDown('I')) // 키보드  i 로 열기 
	{
		if (!m_pInvenInfo->GetIsDraw()) m_pInvenInfo->SetDraw(true);
		else m_pInvenInfo->SetDraw(false);
	}
}

void cInventory::Render()
{
	if (m_pInvenInfo)
		m_pInvenInfo->Render(m_pSprite);
}

void cInventory::SetupInven_Item() // 인벤토리 격자 공간 
{
	RECT rc_win; // 윈도우 렉트
	
	D3DXMATRIXA16 matS;

	GetClientRect(g_hWnd, &rc_win);
	cUIImage* pInventory = new cUIImage;
	pInventory->SetPosition(rc_win.right - 325, 0, 0);
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.54f, 0.44f, 1);
	pInventory->SetmatS(matS);
	pInventory->SetTexture("./Resources/UI/빈인벤.png");
	pInventory->AddRef();
	m_pInvenInfo = pInventory;

	cUIButton* Inven_Button = new cUIButton; // 인벤토리 열고닫기 (엑스)
	Inven_Button->SetPosition(300, 0, 0);
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.5f, 0.5f, 1);
	Inven_Button->SetmatS(matS);
	Inven_Button->SetTexture("./Resources/UI/버튼오프.png", "./Resources/UI/버튼오프.png", "./Resources/UI/버튼온.png");
	Inven_Button->SetDelegate(this);
	Inven_Button->SetTag(cUIObject::Button_1);
	m_pInvenInfo->AddChild(Inven_Button);
	m_pInvenInfo->SetDraw(false);

	for (int i = 0; i < 8; i++)
	{
		std::vector<cUIImage*> rc_img;
		rc_img.resize(5);
		rc_inven.push_back(rc_img);
	}

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			cUIImage* rc_Inven = new cUIImage;
			D3DXMatrixIdentity(&matS);
			D3DXMatrixScaling(&matS, 0.53f, 0.45f, 1);
			rc_Inven->SetmatS(matS);
			rc_Inven->SetTexture("./Resources/UI/빈상자.png");
			rc_Inven->SetPosition(8 + i*rc_Inven->GetCollider().nWidth, m_pInvenInfo->GetCollider().nHeight/2+10 + j*rc_Inven->GetCollider().nHeight, 0);
			m_pInvenInfo->AddChild(rc_Inven);
			rc_inven[i][j] = rc_Inven;
		}
	}
}

void cInventory::SetupInven_Info()
{
	D3DXMATRIX matS;

	Inven_head = new cUIImage;
	Inven_head->SetPosition(194, 85, 0);
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.54f, 0.44f, 1);
	Inven_head->SetmatS(matS);
	Inven_head->SetTexture("./Resources/UI/인벤머리.png");
	Inven_head->SetTag(cUIObject::Inven_head);
	m_pInvenInfo->AddChild(Inven_head);

	Inven_shoulder = new cUIImage;
	Inven_shoulder->SetPosition(150, 95, 0);
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.54f, 0.44f, 1);
	Inven_shoulder->SetmatS(matS);
	Inven_shoulder->SetTexture("./Resources/UI/인벤어깨.png");
	Inven_shoulder->SetTag(cUIObject::Inven_shoulder);
	m_pInvenInfo->AddChild(Inven_shoulder);

	Inven_chest = new cUIImage;
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.54f, 0.44f, 1);
	Inven_chest->SetPosition(190, 115, 0);
	Inven_chest->SetmatS(matS);
	Inven_chest->SetTexture("./Resources/UI/인벤갑옷.png");
	Inven_chest->SetTag(cUIObject::Inven_chest);
	m_pInvenInfo->AddChild(Inven_chest);

	Inven_boots = new cUIImage;
	Inven_boots->SetPosition(194, 225, 0);
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.54f, 0.44f, 1);
	Inven_boots->SetmatS(matS);
	Inven_boots->SetTexture("./Resources/UI/인벤부츠.png");
	Inven_boots->SetTag(cUIObject::Inven_boots);
	m_pInvenInfo->AddChild(Inven_boots);


	Inven_pants = new cUIImage;
	Inven_pants->SetPosition(194, 184, 0);
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.54f, 0.44f, 1);
	Inven_pants->SetmatS(matS);
	Inven_pants->SetTexture("./Resources/UI/인벤바지.png");
	Inven_pants->SetTag(cUIObject::Inven_pants);
	m_pInvenInfo->AddChild(Inven_pants);


	Inven_belt = new cUIImage;
	Inven_belt->SetPosition(190, 167, 0);
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.54f, 0.44f, 1);
	Inven_belt->SetmatS(matS);
	Inven_belt->SetTexture("./Resources/UI/인벤벨트.png");
	Inven_belt->SetTag(cUIObject::Inven_belt);
	m_pInvenInfo->AddChild(Inven_belt);


	Inven_arm = new cUIImage;
	Inven_arm->SetPosition(136, 139, 0);
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.54f, 0.44f, 1);
	Inven_arm->SetmatS(matS);
	Inven_arm->SetTexture("./Resources/UI/인벤팔.png");
	Inven_arm->SetTag(cUIObject::Inven_arm);
	m_pInvenInfo->AddChild(Inven_arm);


	Inven_wrist = new cUIImage;
	Inven_wrist->SetPosition(253, 138, 0);
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.54f, 0.44f, 1);
	Inven_wrist->SetmatS(matS);
	Inven_wrist->SetTexture("./Resources/UI/인벤손목.png");
	Inven_wrist->SetTag(cUIObject::Inven_wrist);
	m_pInvenInfo->AddChild(Inven_wrist);


	Inven_sword = new cUIImage;
	Inven_sword->SetPosition(136, 207, 0);
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.54f, 0.44f, 1);
	Inven_sword->SetmatS(matS);
	Inven_sword->SetTexture("./Resources/UI/인벤칼.png");
	Inven_sword->SetTag(cUIObject::Inven_sword);
	m_pInvenInfo->AddChild(Inven_sword);
	
}




void cInventory::SetupInven_Stat()
{
}


void cInventory::OnClick(cUIButton * pSender)
{
	//cUIButton* pTextUI = (cUIButton*)m_pInven->FindChildByTag(cUIObject::Button_1);

	if (pSender->GetTag() == cUIObject::Button_1)
	{
		m_pInvenInfo->SetDraw(false);
	}
}