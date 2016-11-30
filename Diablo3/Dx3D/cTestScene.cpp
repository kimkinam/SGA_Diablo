#include "stdafx.h"
#include "cTestScene.h"
#include "cGrid.h"
#include "cCamera.h"
#include "cSkinnedMesh.h"
#include "cObj.h"
#include "cMap.h"
#include "cObjLoader.h"
#include "cGroup.h"
#include "cMtlTex.h"
#include "cUIImage.h"
#include "cUIButton.h"
#include "cPlayer.h"	
#include "cActionMove.h"
#include "cMonster.h"

cTestScene::cTestScene()
	: m_pGrid(NULL)
	, m_pPlayer(NULL)
	, m_pCamera(NULL)
	, m_bIsSetMap(false)
	, m_pSprite(NULL)
	, m_pCurObj(NULL)
	, m_vpickPos(0, 0, 0)
	, m_pMonster(NULL)
{
	m_pCamera = new cCamera;
	m_pCamera->Setup();

	m_pGrid = new cGrid;
	m_pGrid->Setup(120);

	//플레이어
	m_pPlayer = new cPlayer;
	m_pPlayer->SetUp();

	//몬스터
	m_pMonster = new cMonster;
	m_pMonster->Setup("Skeleton");
	
	//맵
	m_vecObj.reserve(sizeof(cObj) * 3);

	cMap* obj1 = new cMap;
	obj1->Setup("a1dun_01_test.objobj", "./Resources/Object/");
	obj1->SetSumNailName("a1Dun_01.jpg");
	m_vecObj.push_back(obj1);
	
	//cMap* obj2 = new cMap;
	//obj2->Setup("a1dun_02_test.objobj", "./Resources/Object/");
	//obj2->SetSumNailName("a1Dun_02.jpg");
	//m_vecObj.push_back(obj2);
	//
	//cMap* obj3 = new cMap;
	//obj3->Setup("a1dun_03_test.objobj", "./Resources/Object/");
	//obj3->SetSumNailName("a1dun_03.jpg");
	//m_vecObj.push_back(obj3);
	

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

	if (!m_vecObj.empty())
	{
		for (size_t i = 0; i < m_vecObj.size(); ++i)
		{
			cUIImage* sumNail = new cUIImage;
			D3DXMatrixScaling(&matS, 0.08f, 0.08f, 0.08f);
			sumNail->SetmatS(matS);
			string sPath = "./Resources/MapTool/" + m_vecObj[0]->GetSumNailName();
			sumNail->SetTexture(StringToChar(sPath));
			sumNail->SetPosition(39 + i * 45, 5, 0);
			sumNail->SetTag((cUIObject::Ui_Tag)3);
			m_pUIRoot->AddChild(sumNail);
			m_vecObjUI.push_back(sumNail);
		}
	}
	

}


cTestScene::~cTestScene()
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pPlayer);
	SAFE_DELETE(m_pMonster);

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

	SetMap();

	PlayerMoveTest();
	

	


	if (m_pPlayer)
		m_pPlayer->Update();
	
	if (m_pMonster)
		m_pMonster->Update();

	if (m_pUIRoot)
		m_pUIRoot->Update();

	if (g_pKeyManager->isToggleKey(VK_TAB))
	{
		for (size_t i = 0; i < m_vecMap.size(); ++i)
		{
			m_vecMap[i]->SetIsDrawBound(false);
		}
	}
	else
	{
		for (size_t i = 0; i < m_vecMap.size(); ++i)
		{
			m_vecMap[i]->SetIsDrawBound(true);
		}
	}

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

	if (m_pMonster)
		m_pMonster->Render();


	for (size_t i = 0; i < m_vecMap.size(); ++i)
		m_vecMap[i]->Render();

	if (m_pCurObj)
		m_pCurObj->Render();

	g_pD3DDevice->SetTexture(0, NULL);

	if (m_pUIRoot)
		m_pUIRoot->Render(m_pSprite);

	LPD3DXFONT font;
	font = g_pFontManger->GetFont(cFontManager::E_NORMAL);

	char temp[512];
	sprintf_s(temp, "PlayerPos : %.2f, %.2f, %.2f // CurMap : %d", 
		m_pPlayer->GetPosition().x,
		m_pPlayer->GetPosition().y,
		m_pPlayer->GetPosition().z,
		m_pPlayer->GetCurMap(),
		512);
	RECT rc = { DEBUG_STARTX, DEBUG_STARTY + 150, DEBUG_STARTX + 600, DEBUG_STARTY + 165 };
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
	if (pSender->GetTag() == (cUIObject::Ui_Tag)1) // 왼쪽
	{
		m_pUIRoot->FindChildByTag((cUIObject::Ui_Tag)3);
	}
	if (pSender->GetTag() == (cUIObject::Ui_Tag)2) // 오른쪽
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

void cTestScene::SetMap()
{
	for (size_t i = 0; i < m_vecObjUI.size(); ++i)
	{
		if (InCollider(m_vecObjUI[i]))
		{
			if (g_pKeyManager->isOnceKeyDown(VK_LBUTTON))
			{
				//m_pCurObj = m_vecObj[i];

				m_pCurObj = new cMap;

				m_pCurObj->SetMtl(m_vecObj[i]->GetMtl());
				m_pCurObj->SetHiddenMtl(m_vecObj[i]->GetHiddenMtl());
				m_pCurObj->SetHiddenObj(m_vecObj[i]->GetHiddenObj());
				m_pCurObj->SetObjName(m_vecObj[i]->GetObjName());
				m_pCurObj->SetSumNailName(m_vecObj[i]->GetSumNailName());
				m_pCurObj->SetBoundBox(m_vecObj[i]->GetBoundBox());
				m_pCurObj->SetPosition(m_vecObj[i]->GetPosition());
				m_pCurObj->SetHiddenDraw(m_vecObj[i]->GetHiddenDraw());

				m_vecObj[i]->GetMesh()->CloneMeshFVF(
					m_vecObj[i]->GetMesh()->GetOptions(),
					m_vecObj[i]->GetMesh()->GetFVF(),
					g_pD3DDevice,
					&m_pCurObj->GetMesh());

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

					cMap* obj = new cMap;
					obj = m_pCurObj;
					m_vecMap.push_back(obj);

					m_pCurObj = NULL;
				}
				else
					m_pCurObj->SetPosition(pickPos);

			}
		}
	}
}

void cTestScene::PlayerMoveTest()
{
	if (g_pKeyManager->isOnceKeyDown(VK_RBUTTON))
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
				m_pPlayer->GetMesh()->SetAnimationIndex(0);

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

	if (m_bIsSetMap && !m_vecMap.empty())
	{
		int nCurMap = m_pPlayer->GetCurMap();

		for (size_t i = 0; i < m_vecMap[nCurMap]->GetHiddenObj().size(); ++i)
		{
			for (size_t j = 0; j < m_vecMap[nCurMap]->GetBoundBox()[i].size(); j += 3)
			{
				float u, v, length;
				D3DXVECTOR3 dir = m_pPlayer->GetPosition() - m_pCamera->GetEye();
				D3DXVec3Normalize(&dir, &dir);

				if (D3DXIntersectTri(&(m_vecMap[nCurMap]->GetBoundBox()[i][j].p + m_vecMap[nCurMap]->GetPosition()),
					&(m_vecMap[nCurMap]->GetBoundBox()[i][j + 1].p + m_vecMap[nCurMap]->GetPosition()),
					&(m_vecMap[nCurMap]->GetBoundBox()[i][j + 2].p + m_vecMap[nCurMap]->GetPosition()),
					&m_pCamera->GetEye(),
					&dir,
					&u, &v, &length))
				{

					D3DXVECTOR3 d = m_pPlayer->GetPosition() - m_pCamera->GetEye();
					float dis = D3DXVec3Length(&d);
					if (length < dis)
					{
						m_vecMap[nCurMap]->GetHiddenDraw()[i] = true;
						break;
					}

				}
				else
				{
					m_vecMap[nCurMap]->GetHiddenDraw()[i] = false;
				}

			}

		}


		/*for (size_t i = 0; i < m_vecMap[nCurMap]->GetHiddenObj().size(); ++i)
		{
		for (size_t j = 0; j < m_vecMap[nCurMap]->GetBoundBox()[i].size(); j += 3)
		{
		float u, v, length;
		D3DXVECTOR3 dir = m_pPlayer->GetPosition() - m_pCamera->GetEye();
		D3DXVec3Normalize(&dir, &dir);

		if (D3DXIntersectTri(&(m_vecMap[nCurMap]->GetBoundBox()[i][j].p + m_vecMap[nCurMap]->GetPosition()),
		&(m_vecMap[nCurMap]->GetBoundBox()[i][j+1].p + m_vecMap[nCurMap]->GetPosition()),
		&(m_vecMap[nCurMap]->GetBoundBox()[i][j+2].p + m_vecMap[nCurMap]->GetPosition()),
		&m_pCamera->GetEye(),
		&dir,
		&u, &v, &length))
		{

		D3DXVECTOR3 d = m_pPlayer->GetPosition() - m_pCamera->GetEye();
		float dis = D3DXVec3Length(&d);
		if (length < dis)
		{
		m_vecMap[nCurMap]->GetHiddenDraw()[i] = true;
		break;
		}

		}
		else
		{
		m_vecMap[nCurMap]->GetHiddenDraw()[i] = false;
		}

		}

		}*/
	}

}
