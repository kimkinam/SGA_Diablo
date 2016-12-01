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
	, m_bIsSetMap(false)
	, m_pSprite(NULL)
	, m_pCurObj(NULL)
	, m_vpickPos(0, 0, 0)
	, m_pMonster(NULL)
<<<<<<< HEAD
=======
	, m_pUIRoot(NULL)
>>>>>>> 78e5322bef11eecc2a6e678622184223ce177bf8
{
	
}


cTestScene::~cTestScene()
{
	//ULONG ul = g_pD3DDevice->Release();
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
	SAFE_RELEASE(m_pPlayer);
	SAFE_RELEASE(m_pMonster);

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

	if (m_pUIRoot)
		m_pUIRoot->Destroy();

	for each(auto c in m_vecObjUI)
	{
		SAFE_RELEASE(c);
	}

}


HRESULT cTestScene::SetUp()
{
	if (m_bIsLoad)
	{
		Reset();

		return S_OK;
	}
	cSceneObject::SetUp();


	m_pGrid = new cGrid;
	m_pGrid->Setup(120);

	//플레이어
	m_pPlayer = new cPlayer;
	m_pPlayer->SetUp();

	//몬스터
	m_pMonster = new cMonster;
	m_pMonster->Setup("Skeleton");

	//맵

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
	m_vecTiles.push_back(ST_PC_VERTEX(D3DXVECTOR3(-120, 0, 120), c));
	m_vecTiles.push_back(ST_PC_VERTEX(D3DXVECTOR3(120, 0, 120), c));
	m_vecTiles.push_back(ST_PC_VERTEX(D3DXVECTOR3(120, 0, -120), c));
	m_vecTiles.push_back(ST_PC_VERTEX(D3DXVECTOR3(120, 0, -120), c));
	m_vecTiles.push_back(ST_PC_VERTEX(D3DXVECTOR3(-120, 0, -120), c));
	m_vecTiles.push_back(ST_PC_VERTEX(D3DXVECTOR3(-120, 0, 120), c));


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

	m_bIsLoad = true;
	return S_OK;
}

HRESULT cTestScene::Reset()
{
	cSceneObject::Reset();

	m_vecMap.clear();

	return S_OK;
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
	if (!pUI) return false;
	RECT rc;
	float deltaX = pUI->GetPosition().x;
	float deltaY = pUI->GetPosition().y;

	if (pUI->GetParent())
	{
		SetRect(&rc, pUI->GetParent()->GetPosition().x + pUI->GetCollider().nStartX + deltaX
			, pUI->GetParent()->GetPosition().y + pUI->GetCollider().nStartY + deltaY
			, pUI->GetParent()->GetPosition().x + pUI->GetCollider().nWidth + deltaX
			, pUI->GetParent()->GetPosition().y + pUI->GetCollider().nHeight + deltaY);
	}
	else
	{
		SetRect(&rc, pUI->GetCollider().nStartX + deltaX
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
		//유아이를 선택 했다면
		if (InCollider(m_vecObjUI[i]))
		{
			if (g_pKeyManager->isOnceKeyDown(VK_LBUTTON))
			{

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

					m_pCurObj->SetLocalBoundBox();
					//cMap* obj = new cMap;
					//obj = m_pCurObj;
					m_pCurObj->AddRef();
					m_vecMap.push_back(m_pCurObj);

					m_pCurObj = NULL;
				}
				else
					m_pCurObj->SetPosition(pickPos);

			}
		}
	}

	if (g_pKeyManager->isToggleKey(VK_TAB))
	{
		for (size_t i = 0; i < m_vecMap.size(); ++i)
		{
			for (size_t j = 0; j < m_vecMap[i]->GetBoundBox().size(); ++j)
				m_vecMap[i]->GetBoundBox()[j]->SetIsDraw(false);
		}
	}
	else
	{
		for (size_t i = 0; i < m_vecMap.size(); ++i)
		{
			for (size_t j = 0; j < m_vecMap[i]->GetBoundBox().size(); ++j)
				m_vecMap[i]->GetBoundBox()[j]->SetIsDraw(true);
		}
	}
}

void cTestScene::PlayerMoveTest()
{
	//플레이어 피킹
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

	//플레이어 아이템 변화 테스트

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

	//사라지는 오브젝트 테스트
	if (m_bIsSetMap && !m_vecMap.empty())
	{
		//현재 플레이어의 맵
		int nCurMap = m_pPlayer->GetCurMap();

		//플레이어를 바라보는 카메라 방향
		D3DXVECTOR3 dir = m_pPlayer->GetPosition() - m_pCamera->GetEye();

		//플레이어와 카메라의 거리
		float disToPlayer = D3DXVec3Length(&dir);

		//사라질 오브젝트와 카메라의 거리
		float disToHidden = 0.0f;

		//현재 맵에서 사라질수있는 오브젝트의 바운드박스를 검사
		for (size_t i = 0; i < m_vecMap[nCurMap]->GetBoundBox().size(); ++i)
		{
			//사라질 오브젝트의 바운드박스가 카메라에서의 ray에 걸렸으면 distance를 반환
			if (m_vecMap[nCurMap]->GetBoundBox()[i]->GetRayDistance(
				m_pCamera->GetEye(), dir, disToHidden))
			{
				//사라질 오브젝트가 플레이어보다 가깝게 있으면
				if (disToHidden < disToPlayer)
				{
					//그 오브젝트를 그리지 않는다.
					m_vecMap[nCurMap]->GetHiddenDraw()[i] = true;
					break;
				}
			}
			//사라질 오브젝트의 바운드 박스가 카메라에서의 ray에 걸리지 않았으면
			else
				//그 오브젝트는 그린다.
				m_vecMap[nCurMap]->GetHiddenDraw()[i] = false;
		}
	}

}
