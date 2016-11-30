#include "stdafx.h"
#include "cInventory.h"
#include "cUIImage.h"
#include "cUIButton.h"
#include "cUIText.h"
#include "cItem.h"



cInventory::cInventory()
	: m_pInvenInfo(NULL)
	, m_pInvenItem(NULL)
	, m_pInvenStat(NULL)
	, m_pSprite(NULL)
	, test_item(NULL)
	, m_pCurItem(NULL)
	, Creat_item(true)
	, m_isClick(false)
{
	m_ptClickPoint.x = 0;
	m_ptClickPoint.y = 0;
}


cInventory::~cInventory()
{
	if (m_pInvenItem)
		m_pInvenItem->Destroy();

	if (m_pInvenInfo)
		m_pInvenInfo->Destroy();

	if (m_pInvenStat)
		m_pInvenStat->Destroy();

	SAFE_RELEASE(m_pCurItem);
	SAFE_RELEASE(m_pSprite);
	SAFE_RELEASE(test_item);
	SAFE_RELEASE(test_item_1);

	for (int i = 0; i < m_vecItem.size(); i++)
	{
		SAFE_DELETE(m_vecItem[i]);
	}

	for (size_t i = 0; i < rc_inven[i].size(); ++i)
	{
		for (size_t j = 0; j < rc_inven[j].size(); ++j)
		{
			SAFE_DELETE(rc_inven[i][j]);
		}
	}
	for (size_t i = 0; i < m_Itemimg.size(); i++)
	{
		SAFE_DELETE(m_Itemimg[i]);
	}
}	

void cInventory::Setup()
{
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

	SetupInven_Item();
	SetupInven_Info();
	SetupInven_Stat();

	m_Itemimg.resize(2);

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

	for (size_t i = 0; i < m_Itemimg.size(); i++)
	{
		m_Itemimg[i]->Update();
	}

//	test_item->Update();
//	test_item_1->Update();
	m_pCurItem->Update();

	if (g_pKeyManager->isStayKeyDown(VK_LBUTTON))
	{	
		for (size_t i = 0; i < m_Itemimg.size(); i++)
		{
			if (isPicked(m_Itemimg[i]))
			{
			m_ptClickPoint = g_ptMouse;
			m_isClick = true;
			}
				
		}
	}
	else
	{
		m_isClick = false;
	}

	//if (m_isClick)
	//{
	//	int deltaX = g_ptMouse.x - m_ptClickPoint.x;
	//	int deltaY = g_ptMouse.y - m_ptClickPoint.y;
	//	m_ptClickPoint = g_ptMouse;
	//	m_Itemimg[i]->SetPosition(m_Itemimg[i]->GetPosition().x += deltaX, m_Itemimg[i]->GetPosition().y += deltaY, 0);
	//}
	
	

	//invenPicked(test_item_1);


}

void cInventory::Render()
{
	if (m_pInvenInfo)
		m_pInvenInfo->Render(m_pSprite);
	for (int i = 0; i < m_vecItem.size(); i++)
	{
		m_vecItem[i]->Render(m_pSprite);
	}

	for (int i = 0; i < m_Itemimg.size(); i++)
	{
		m_Itemimg[i]->Render(m_pSprite);
	}
	//test_item->Render(m_pSprite);
	//test_item_1->Render(m_pSprite);
}

void cInventory::SetupInven_Item() // 인벤토리 격자 공간 
{
	RECT rc_win; // 윈도우 렉트
	GetClientRect(g_hWnd, &rc_win);
	D3DXMATRIXA16 matS;
	
	cUIImage* pInventory = new cUIImage;
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.54f, 0.44f, 1);
	pInventory->SetmatS(matS);
	pInventory->SetTexture("./Resources/UI/빈인벤.png");
	pInventory->SetPosition(rc_win.right - pInventory->GetCollider().nWidth, 0, 0);
	m_pInvenInfo = pInventory;

	cUIButton* Inven_Button = new cUIButton; // 인벤토리 열고닫기 (엑스)
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.5f, 0.5f, 1);
	Inven_Button->SetmatS(matS);
	Inven_Button->SetTexture("./Resources/UI/버튼오프.png", "./Resources/UI/버튼오프.png", "./Resources/UI/버튼온.png");
	Inven_Button->SetPosition(300, 0, 0);
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
}  //인벤토리 격자공간

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
	D3DXMatrixScaling(&matS, 0.54f, 0+.44f, 1);
	Inven_sword->SetmatS(matS);
	Inven_sword->SetTexture("./Resources/UI/인벤칼.png");
	Inven_sword->SetTag(cUIObject::Inven_sword);
	m_pInvenInfo->AddChild(Inven_sword);


	test_item = new cUIImage;
	test_item->SetPosition(500, 100, 0);
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.53f, 0.48f, 1);
	test_item->SetmatS(matS);
	test_item->SetTexture("./Resources/UI/활.png");
	m_Itemimg.push_back(test_item);

	test_item_1 = new cUIImage;
	test_item_1->SetPosition(600, 100, 0);
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.57f, 0.7f, 1);
	test_item_1->SetmatS(matS);
	test_item_1->SetTexture("./Resources/UI/바지1.png");
	m_Itemimg.push_back(test_item_1);


} // 




void cInventory::SetupInven_Stat()
{
	if (Creat_item == true)
	{
		m_pCurItem = new cItem;
		D3DXMATRIXA16 matS;
		
		m_pCurItem->SetItemType(ITEM_SWORD);
		switch (m_pCurItem->GetItemType())
		{
		case  ITEM_HEAD :
			
			break;
		case ITEM_ARM :
			break;
		case ITEM_CHEST :
			break;
		case ITEM_PANTS :
			break;
		case ITEM_BOOTS :
			break;
		case ITEM_SHOULDER :
			break;
		case ITEM_SWORD :
			D3DXMatrixIdentity(&matS);
			D3DXMatrixScaling(&matS, 0.3f, 0.3f, 1);
			m_pCurItem->SetmatS(matS);
			m_pCurItem->SetPosition(0, 0, 0);
			m_pCurItem->Setup("./Resources/UI/활.png", NULL, 100, 100, 100, 100, ITEM_SWORD);		
			m_vecItem.push_back(m_pCurItem);
			Creat_item = false;
			break;
		case ITEM_ACCESS :
			break;
		default:
			break;
		}
	}

}


void cInventory::OnClick(cUIButton * pSender)
{
	//cUIButton* pTextUI = (cUIButton*)m_pInven->FindChildByTag(cUIObject::Button_1);

	if (pSender->GetTag() == cUIObject::Button_1)
	{
		m_pInvenInfo->SetDraw(false);
	}
}



bool cInventory::isPicked(cUIImage* item)
{
	RECT rc;
	float deltaX = item->GetPosition().x;
	float deltaY = item->GetPosition().y;

	SetRect(&rc, item->GetCollider().nStartX + deltaX,
		item->GetCollider().nStartY + deltaY,
		item->GetCollider().nWidth + deltaX,
		item->GetCollider().nHeight + deltaY);

	if (PtInRect(&rc, g_ptMouse))
	{
		return true;
	}
		return false;
}


void cInventory::invenPicked(cUIImage* item)
{
	RECT temp;
	RECT rc[8][5];
	if ((g_pKeyManager->isOnceKeyUp(VK_LBUTTON)))
	{
		for (size_t i = 0; i < rc_inven[i].size(); i++)
		{
			for (size_t j = 0; i < rc_inven[j].size(); j++)
			{
					if (rc_inven[i][j]->GetIsPicked())
					{

					}
					else
					{
						rc_inven[i][j]->SetIsPicked(true);
						test_item_1->SetPosition(rc_inven[i][j]->GetCollider().nStartX, rc_inven[i][j]->GetCollider().nStartY, 0);
					}				
			}
		}
	}
}