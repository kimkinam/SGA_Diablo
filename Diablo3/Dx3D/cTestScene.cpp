#include "stdafx.h"
#include "cTestScene.h"
#include "cGrid.h"
#include "cCamera.h"
#include "cSkinnedMesh.h"
#include "cObj.h"
#include "cObjLoader.h"
#include "cGroup.h"
#include "cMtlTex.h"
#include "cUIImage.h"
#include "cUIButton.h"
#include "cUIText.h"


cTestScene::cTestScene()
	: m_pGrid(NULL)
	, m_pMesh(NULL)
	, m_pSword(NULL)
	, m_pCamera(NULL)
	, m_bIsSetMap(false)
	, m_pSprite(NULL)
	, m_pUIRoot(NULL)
{
	m_pCamera = new cCamera;
	m_pCamera->Setup();

	m_pGrid = new cGrid;
	m_pGrid->Setup(120);

	//¹Ù¹Ù
	m_pMesh = new cSkinnedMesh("./Resources/Player/", "Bab1.X");
	m_pMesh->SetAnimationIndex(5);

	//Ä®
	m_pSword = new cObj;
	m_pSword->SetUp("twohandsword.objobj", "./Resources/Object/");

	m_pSword->SetWorldTM(m_pMesh->AttachItem("right_weapon"));

	//¸Ê
	m_vecObj.reserve(sizeof(cObj) * 3);

	cObj* obj1 = new cObj;
	obj1->SetUp("a1dun_01.objobj", "./Resources/Object/");
	obj1->SetSumNailName("a1Dun_01.jpg");
	m_vecObj.push_back(obj1);

	cObj* obj2 = new cObj;
	obj2->SetUp("a1dun_02.objobj", "./Resources/Object/");
	obj2->SetSumNailName("a1Dun_02.jpg");
	m_vecObj.push_back(obj2);

	cObj* obj3 = new cObj;
	obj3->SetUp("a1dun_03.objobj", "./Resources/Object/");
	obj3->SetSumNailName("a1dun_03.jpg");
	m_vecObj.push_back(obj3);
	

	ST_PC_VERTEX v;
	D3DXCOLOR c;
	c = D3DCOLOR_XRGB(255, 255, 255);
	m_vecTiles.reserve(sizeof(ST_PC_VERTEX) * 6);
	m_vecTiles.push_back(ST_PC_VERTEX(D3DXVECTOR3(-20, 0, 20), c));
	m_vecTiles.push_back(ST_PC_VERTEX(D3DXVECTOR3( 20, 0, 20), c));
	m_vecTiles.push_back(ST_PC_VERTEX(D3DXVECTOR3( 20, 0, -20), c));
	m_vecTiles.push_back(ST_PC_VERTEX(D3DXVECTOR3(20, 0, -20), c));
	m_vecTiles.push_back(ST_PC_VERTEX(D3DXVECTOR3(-20, 0, -20), c));
	m_vecTiles.push_back(ST_PC_VERTEX(D3DXVECTOR3(-20, 0, 20), c));
	

	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

	RECT rcWin;
	GetClientRect(g_hWnd, &rcWin);

	D3DXMATRIX matS;

	cUIImage* pTitleBar = new cUIImage;
	D3DXMatrixScaling(&matS, 0.4f, 0.5f, 1.f);
	pTitleBar->SetmatS(matS);
	pTitleBar->SetTexture("./Resources/MapTool/tool-box.png");
	pTitleBar->SetPosition(rcWin.right - pTitleBar->GetCollider().nWidth - 5, rcWin.top + 5, 0);
	m_pUIRoot = pTitleBar;

	cUIButton* pLightArrow = new cUIButton;
	D3DXMatrixScaling(&matS, 0.5f, 0.5f, 1.f);
	pLightArrow->SetmatS(matS);
	pLightArrow->SetTexture("./Resources/MapTool/right_Arrow_Normal.png",
		"./Resources/MapTool/right_Arrow_Normal.png", "./Resources/MapTool/right_Arrow_Pressed.png");
	pLightArrow->SetPosition(D3DXVECTOR3(m_pUIRoot->GetCollider().nWidth - pLightArrow->GetCollider().nWidth, 6, 0));
	pLightArrow->SetDelegate(this);
	pLightArrow->SetTag((cUIObject::Ui_Tag)2);
	m_pUIRoot->AddChild(pLightArrow);

	cUIButton* pLeftArrow = new cUIButton;
	D3DXMatrixScaling(&matS, 0.5f, 0.5f, 1.f);
	pLeftArrow->SetmatS(matS);
	pLeftArrow->SetTexture("./Resources/MapTool/left_Arrow_Normal.png",
		"./Resources/MapTool/left_Arrow_Normal.png", "./Resources/MapTool/left_Arrow_Pressed.png");
	pLeftArrow->SetPosition(D3DXVECTOR3(2, 6, 0));
	pLightArrow->SetDelegate(this);
	pLeftArrow->SetTag((cUIObject::Ui_Tag)1);
	m_pUIRoot->AddChild(pLeftArrow);

	for (size_t i = 0; i < m_vecObj.size() * 2 + 1; ++i)
	{
		cUIImage* sumNail = new cUIImage;
		D3DXMatrixScaling(&matS, 0.08f, 0.08f, 0.08f);
		sumNail->SetmatS(matS);
		string sPath = "./Resources/MapTool/" + m_vecObj[i % 3]->GetSumNailName();
		sumNail->SetTexture(StringToChar(sPath));
		sumNail->SetPosition(39 + i * 45, 5, 0);
		sumNail->SetTag((cUIObject::Ui_Tag)3);
		m_pUIRoot->AddChild(sumNail);
	}

	
}


cTestScene::~cTestScene()
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);

	SAFE_DELETE(m_pMesh);
	SAFE_DELETE(m_pSword);

	SAFE_RELEASE(m_pSprite);

	if (m_pUIRoot)
		m_pUIRoot->Destroy();

	for each(auto c in m_vecObj)
	{
		SAFE_DELETE(c);
	}

	m_nRefCount--;
}


HRESULT cTestScene::SetUp()
{
	this->AddRef();

	return S_OK;
}

void cTestScene::Release()
{
	m_nRefCount--;
}

void cTestScene::Update()
{
	if (m_pCamera)
	{
		m_pCamera->Update(NULL);
	}

	//if (g_pKeyManager->isOnceKeyDown(VK_LEFT))
	//	m_pMap->SetPosition(m_pMap->GetPosition() + D3DXVECTOR3(-20, 0, 0));
	//if (g_pKeyManager->isOnceKeyDown(VK_RIGHT))
	//	m_pMap->SetPosition(m_pMap->GetPosition() + D3DXVECTOR3(20, 0, 0));
	//if (g_pKeyManager->isOnceKeyDown(VK_UP))
	//	m_pMap->SetPosition(m_pMap->GetPosition() + D3DXVECTOR3(0, 0, 20));
	//if (g_pKeyManager->isOnceKeyDown(VK_DOWN))
	//	m_pMap->SetPosition(m_pMap->GetPosition() + D3DXVECTOR3(0, 0, -20));
	//if (g_pKeyManager->isOnceKeyDown('1'))
	//{
	//	m_pMesh->ChangeItem("Barb_M_MED_Gloves", "./Resources/Player/Barb_M_MED_Norm_Base_A_diff.dds");
	//}
	//if (g_pKeyManager->isOnceKeyDown('2'))
	//{
	//	m_pMesh->ChangeItem("Barb_M_MED_Pants", "./Resources/Player/Barb_M_MED_Norm_Base_A_diff.dds");
	//}
	//if (g_pKeyManager->isOnceKeyDown('3'))
	//{
	//	m_pMesh->ChangeItem("Barb_M_MED_Cloth", "./Resources/Player/Barb_M_MED_Norm_Base_A_diff.dds");
	//}
	//if (g_pKeyManager->isOnceKeyDown('4'))
	//{
	//	m_pMesh->ChangeItem("Barb_M_MED_Armor", "./Resources/Player/Barb_M_MED_Norm_Base_A_diff.dds");
	//}
	//if (g_pKeyManager->isOnceKeyDown('5'))
	//{
	//	m_pMesh->ChangeItem("Barb_M_MED_Boots", "./Resources/Player/Barb_M_MED_Norm_Base_A_diff.dds");
	//}

	if (m_pUIRoot)
		m_pUIRoot->Update();

	/*if (g_pKeyManager->isToggleKey(VK_TAB) && !m_bIsSetMap)
	{
		if (m_pMap)
		{
			cRay r = cRay::RayAtWorldSpace(g_ptMouse.x, g_ptMouse.y);
			D3DXVECTOR3 pickPos;
			for (size_t i = 0; i < m_vecTiles.size(); i += 3)
			{
				if (r.IntersectTri(m_vecTiles[i].p,
					m_vecTiles[i + 1].p,
					m_vecTiles[i + 2].p,
					pickPos))
				{
					
					if (g_pKeyManager->isOnceKeyDown(VK_LBUTTON))
					{
						if (pickPos.x < 0 && pickPos.z > 0)
							m_pMap->SetPosition(D3DXVECTOR3(-10, 0, 10));
						m_bIsSetMap = true;
					}
					else
						m_pMap->SetPosition(pickPos);
					
				}
			}
		}
	}*/
	
	
}

void cTestScene::Render()
{
	if (m_pGrid)
		m_pGrid->Render();
	
	if (m_pCamera)
		m_pCamera->Render();

	if (m_pMesh)
	{
		m_pMesh->UpdateAndRender();
	}
	//
	if (m_pSword)
		m_pSword->Render();

	//for each (auto c in m_vecObj)
	//{
	//	c->Render();
	//}
	//if (m_pMap)
	//	m_pMap->Render();

	g_pD3DDevice->SetTexture(0, NULL);

	if (m_pUIRoot)
		m_pUIRoot->Render(m_pSprite);

}

void cTestScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostMessage(hWnd, WM_DESTROY, NULL, NULL);
			break;
		case VK_LEFT:

			break;
		case VK_RIGHT:

			break;
		case VK_UP:
			
			break;
		case VK_DOWN:

			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
	}
	if (m_pCamera)
		m_pCamera->WndProc(hWnd, message, wParam, lParam);
}

void cTestScene::OnClick(cUIButton * pSender)
{
	if (pSender->GetTag() == (cUIObject::Ui_Tag)1) // ¿ÞÂÊ
	{
		//m_pUIRoot->FindChildByTag((cUIObject::Ui_Tag)3);
	}
	if (pSender->GetTag() == (cUIObject::Ui_Tag)2) // ¿À¸¥ÂÊ
	{
		int a = 0;
	}
}
