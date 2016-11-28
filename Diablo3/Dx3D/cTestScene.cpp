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
#include "cPlayer.h"	
#include "cActionMove.h"


cTestScene::cTestScene()
	: m_pGrid(NULL)
	, m_pPlayer(NULL)
	, m_pCamera(NULL)
	, m_bIsSetMap(false)
	, m_pSprite(NULL)
	, m_pCurObj(NULL)
{
	m_pCamera = new cCamera;
	m_pCamera->Setup();

	m_pGrid = new cGrid;
	m_pGrid->Setup(120);

	//ÇÃ·¹ÀÌ¾î
	m_pPlayer = new cPlayer;
	m_pPlayer->SetUp();
	
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
	m_vecTiles.push_back(ST_PC_VERTEX(D3DXVECTOR3(-120, 0,	120), c));
	m_vecTiles.push_back(ST_PC_VERTEX(D3DXVECTOR3( 120, 0,	120), c));
	m_vecTiles.push_back(ST_PC_VERTEX(D3DXVECTOR3( 120, 0, -120), c));
	m_vecTiles.push_back(ST_PC_VERTEX(D3DXVECTOR3( 120, 0, -120), c));
	m_vecTiles.push_back(ST_PC_VERTEX(D3DXVECTOR3(-120, 0, -120), c));
	m_vecTiles.push_back(ST_PC_VERTEX(D3DXVECTOR3(-120, 0,	120), c));
	

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
		m_vecObjUI.push_back(sumNail);
	}

}


cTestScene::~cTestScene()
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pPlayer);

	SAFE_RELEASE(m_pCurObj);

	SAFE_RELEASE(m_pSprite);


	for each(auto c in m_vecObj)
	{
		SAFE_RELEASE(c);
	}
	
	for each(auto c in m_vecMap)
	{
		SAFE_RELEASE(c);
	}

	if(m_pUIRoot)
		m_pUIRoot->Destroy();

	for each(auto c in m_vecObjUI)
	{
		SAFE_RELEASE(c);
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
	if (m_pCamera && !InCollider(m_pUIRoot))
	{
		m_pCamera->Update(NULL);
	}

	
	for (size_t i = 0; i < m_vecObjUI.size(); ++i)
	{
		if (InCollider(m_vecObjUI[i]))
		{
			
			if (g_pKeyManager->isOnceKeyDown(VK_LBUTTON))
			{
				m_pCurObj = m_vecObj[i];

				m_bIsSetMap = false;
			}
		}
	}

	if (m_pCurObj)
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

				if (g_pKeyManager->isOnceKeyDown(VK_RBUTTON))
				{
					if (pickPos.x < 0 && pickPos.z > 0)
						m_pCurObj->SetPosition(D3DXVECTOR3(-10, 0, 10));
					if (pickPos.x < 0 && pickPos.z < 0)
						m_pCurObj->SetPosition(D3DXVECTOR3(-10, 0, -10));
					if (pickPos.x > 0 && pickPos.z > 0)
						m_pCurObj->SetPosition(D3DXVECTOR3(10, 0, 10));
					if (pickPos.x > 0 && pickPos.z < 0)
						m_pCurObj->SetPosition(D3DXVECTOR3(10, 0, -10));
					m_bIsSetMap = false;

					cObj* obj = new cObj;
					obj->SetMtl(m_pCurObj->GetMtl());
					obj->SetHiddenMtl(m_pCurObj->GetHiddenMtl());
					obj->SetHiddenObj(m_pCurObj->GetHiddenObj());
					obj->SetObjName(m_pCurObj->GetObjName());
					obj->SetSumNailName(m_pCurObj->GetSumNailName());

					obj->SetPosition(m_pCurObj->GetPosition());

					m_pCurObj->GetMesh()->CloneMeshFVF(
						m_pCurObj->GetMesh()->GetOptions(),
						m_pCurObj->GetMesh()->GetFVF(),
						g_pD3DDevice,
						&obj->GetMesh());
					m_vecMap.push_back(obj);

					m_pCurObj = NULL;
				}
				else
					m_pCurObj->SetPosition(pickPos);

			}
		}
	}

	if (!m_pCurObj && g_pKeyManager->isOnceKeyDown(VK_RBUTTON))
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
				cActionMove* pAction = new cActionMove;

				pAction->SetTo(pickPos);
				pAction->SetFrom(m_pPlayer->GetPosition());
				pAction->SetTarget(m_pPlayer);
				pAction->SetDelegate(m_pPlayer);
				pAction->Start();
				m_pPlayer->SetAction(pAction);
				m_pPlayer->GetMesh()->SetAnimationIndex(4);

				m_bIsSetMap = true;
			}
		}
	}

	//if (g_pKeyManager->isOnceKeyDown('1'))
	//{
	//	m_pPlayer->GetMesh()->ChangeItem("Barb_M_MED_Gloves", "./Resources/Player/Barb_M_MED_Norm_Base_A_diff.dds");
	//}
	//if (g_pKeyManager->isOnceKeyDown('2'))
	//{
	//	m_pPlayer->GetMesh()->ChangeItem("Barb_M_MED_Pants", "./Resources/Player/Barb_M_MED_Norm_Base_A_diff.dds");
	//}
	//if (g_pKeyManager->isOnceKeyDown('3'))
	//{
	//	m_pPlayer->GetMesh()->ChangeItem("Barb_M_MED_Cloth", "./Resources/Player/Barb_M_MED_Norm_Base_A_diff.dds");
	//}
	//if (g_pKeyManager->isOnceKeyDown('4'))
	//{
	//	m_pPlayer->GetMesh()->ChangeItem("Barb_M_MED_Armor", "./Resources/Player/Barb_M_MED_Norm_Base_A_diff.dds");
	//}
	//if (g_pKeyManager->isOnceKeyDown('5'))
	//{
	//	m_pPlayer->GetMesh()->ChangeItem("Barb_M_MED_Boots", "./Resources/Player/Barb_M_MED_Norm_Base_A_diff.dds");
	//}


	//if (m_bIsSetMap)
	//{
	//
	//}



	if (m_pPlayer)
		m_pPlayer->Update();
	
	if (m_pUIRoot)
		m_pUIRoot->Update();


	
}

void cTestScene::Render()
{
	if (m_pGrid)
		m_pGrid->Render();
	
	if (m_pCamera)
		m_pCamera->Render();

	//
	if (m_pPlayer)
		m_pPlayer->Render();

	//for each (auto c in m_vecObj)
	//{
	//	c->Render();
	//}
	//if (m_pMap)
	//	m_pMap->Render();

	for (size_t i = 0; i < m_vecMap.size(); ++i)
		m_vecMap[i]->Render();

	if (m_pCurObj)
		m_pCurObj->Render();

	g_pD3DDevice->SetTexture(0, NULL);

	if (m_pUIRoot)
		m_pUIRoot->Render(m_pSprite);

	LPD3DXFONT font;
	font = g_pFontManger->GetFont(cFontManager::E_NORMAL);

	char temp[128];
	sprintf_s(temp, "PlayerPos : %.2f, %.2f, %.2f", 
		m_pPlayer->GetPosition().x,
		m_pPlayer->GetPosition().y,
		m_pPlayer->GetPosition().z,
		128);
	RECT rc = { DEBUG_STARTX, DEBUG_STARTY + 150, DEBUG_STARTX + 250, DEBUG_STARTY + 165 };
	font->DrawText(NULL,
		temp,
		128,
		&rc,
		DT_LEFT,
		D3DCOLOR_XRGB(255, 255, 255));

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
		m_pUIRoot->FindChildByTag((cUIObject::Ui_Tag)3);
	}
	if (pSender->GetTag() == (cUIObject::Ui_Tag)2) // ¿À¸¥ÂÊ
	{
		int a = 0;
	}
}

bool cTestScene::InCollider(cUIObject * pUI)
{
	RECT rc;
	float deltaX = pUI->GetPosition().x;
	float deltaY = pUI->GetPosition().y;

	if (pUI->GetParent())
	{
		SetRect(&rc,  pUI->GetParent()->GetPosition().x + pUI->GetCollider().nStartX + deltaX
					, pUI->GetParent()->GetPosition().y + pUI->GetCollider().nStartY + deltaY
					, pUI->GetParent()->GetPosition().x + pUI->GetCollider().nWidth + deltaX
					, pUI->GetParent()->GetPosition().y + pUI->GetCollider().nHeight + deltaY);
	}
	else
	{
		SetRect(&rc,  pUI->GetCollider().nStartX + deltaX
					, pUI->GetCollider().nStartY + deltaY
					, pUI->GetCollider().nWidth + deltaX
					, pUI->GetCollider().nHeight + deltaY);

		int a = 0;
	}
	

	if (PtInRect(&rc, g_ptMouse))
		return true;

	return false;
}
