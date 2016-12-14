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
#include "cBoss.h"
#include "cUIObject.h"
#include "cFetish.h"
#include "cSkeleton.h"
#include "cSkeletonArcher.h"
#include "cStitch.h"
#include "cZombieDog.h"
#include "cGargantuan.h"

cTestScene::cTestScene()
	: m_pGrid(NULL)
	, m_pPlayer(NULL)
	, m_bisClone(false)
	, m_pSprite(NULL)
	, m_pCurObj(NULL)
	, m_pCurMonster(NULL)
	, m_vpickPos(0, 0, 0)
	, m_pUIRoot(NULL)
	, m_bIsBound(false)
	, m_vMin(0, 0, 0)
	, m_vMax(0, 0, 0)
	, m_nCurIndex(0)
	, m_emState(SET_MAP)
{
	D3DXMatrixIdentity(&m_matR);
}


cTestScene::~cTestScene()
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
	SAFE_RELEASE(m_pPlayer);


	for each(auto c in m_vecMonster)
	{
		SAFE_RELEASE(c);
	}
	
	SAFE_RELEASE(m_pSprite);

	for each(auto c in m_vecObj)
	{
		SAFE_RELEASE(c);
	}

	for each(auto c in m_vecOutMap)
	{
		SAFE_RELEASE(c);
	}

	for each(auto c in m_vecOutMonster)
	{
		SAFE_RELEASE(c);
	}

	for each(auto c in m_vecBoundBox)
	{
		SAFE_DELETE(c);
	}

	if (m_pUIRoot)
		m_pUIRoot->Destroy();


	for each(auto c in m_vecObjUI)
	{
		SAFE_RELEASE(c);
	}

	for each(auto c in m_vecMonsterUI)
	{
		SAFE_RELEASE(c);
	}

	SAFE_RELEASE(m_pCurObj);
	SAFE_RELEASE(m_pCurMonster);
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
	//m_pPlayer = new cPlayer;
	//m_pPlayer->Setup();

	//몬스터
	cFetish* Fetish = new cFetish;
	Fetish->Setup();
	Fetish->SetSumNailName("Fetish.png");
	m_vecMonster.push_back(Fetish);

	cSkeleton* Skeleton = new cSkeleton;
	Skeleton->Setup();
	Skeleton->SetSumNailName("Skeleton.png");
	m_vecMonster.push_back(Skeleton);

	cSkeletonArcher* SkeletonArcher = new cSkeletonArcher;
	SkeletonArcher->Setup();
	SkeletonArcher->SetSumNailName("skeletonArcher.png");
	m_vecMonster.push_back(SkeletonArcher);

	cGargantuan* Gargantuan = new cGargantuan;
	Gargantuan->Setup();
	Gargantuan->SetSumNailName("Gargantuan.png");
	m_vecMonster.push_back(Gargantuan);

	cZombieDog* ZombieDog = new cZombieDog;
	ZombieDog->Setup();
	ZombieDog->SetSumNailName("ZombieDog.png");
	m_vecMonster.push_back(ZombieDog);

	cStitch* Stitch = new cStitch;
	Stitch->Setup();
	Stitch->SetSumNailName("Stitch.png");
	m_vecMonster.push_back(Stitch);



	////완전한 맵
	cMap* obj1 = new cMap;
	obj1->SetScale(D3DXVECTOR3(0.5f, 0.5f, 0.5f));
	obj1->Setup("a1dun_01_test.objobj", "./Resources/Object/");
	obj1->SetSumNailName("a1Dun_01.jpg");
	m_vecObj.push_back(obj1);
	
	//cMap* obj2 = new cMap;
	//obj2->SetScale(D3DXVECTOR3(0.5f, 0.5f, 0.5f));
	//obj2->Setup("a1dun_02_test.objobj", "./Resources/Object/");
	//obj2->SetSumNailName("a1Dun_02.jpg");
	//m_vecObj.push_back(obj2);
	//
	//cMap* obj3 = new cMap;
	//obj3->SetScale(D3DXVECTOR3(0.5f, 0.5f, 0.5f));
	//obj3->Setup("a1dun_03_test.objobj", "./Resources/Object/");
	//obj3->SetSumNailName("a1Dun_03.jpg");
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
			cUIImage* thumbnail = new cUIImage;
			D3DXMatrixScaling(&matS, 0.08f, 0.08f, 0.08f);
			thumbnail->SetmatS(matS);
			string sPath = "./Resources/MapTool/" + m_vecObj[i]->GetSumNailName();
			thumbnail->SetTexture(StringToChar(sPath));
			thumbnail->SetPosition(39 + i * 45, 5, 0);

			thumbnail->SetTag(cUIObject::Ui_Tag::thumbnail_Object);
			thumbnail->SetCount(i);

			m_pUIRoot->AddChild(thumbnail);
			SAFE_ADDREF(thumbnail);
			m_vecObjUI.push_back(thumbnail);

		}
	}

	if (!m_vecMonster.empty())
	{
		for (size_t i = 0; i < m_vecMonster.size(); ++i)
		{
			cUIImage* thumbnail = new cUIImage;
			D3DXMatrixScaling(&matS, 0.08f, 0.08f, 0.08f);
			thumbnail->SetmatS(matS);
			string sPath = "./Resources/MapTool/" + m_vecMonster[i]->GetSumNailName();
			thumbnail->SetTexture(StringToChar(sPath));
			thumbnail->SetPosition(39 + i * 45, 5, 0);

			thumbnail->SetTag(cUIObject::Ui_Tag::thumbnail_Monster);
			thumbnail->SetCount(i);

			m_pUIRoot->AddChild(thumbnail);
			SAFE_ADDREF(thumbnail);
			m_vecMonsterUI.push_back(thumbnail);

		}
	}

	m_pUIRoot->SetDraw(cUIObject::Ui_Tag::thumbnail_Monster, false);

	m_bIsLoad = true;
	return S_OK;
}

HRESULT cTestScene::Reset()
{
	cSceneObject::Reset();

	m_vecOutMap.clear();

	return S_OK;
}


void cTestScene::Update()
{
	StateChange();

	SetMap();
	
	if (m_pCamera && !InCollider(m_pUIRoot))
	{
		m_pCamera->Update(NULL);
	}

	for (size_t i = 0; i < m_vecBoundBox.size(); ++i)
	{
		m_vecBoundBox[i]->Update(NULL);
	}

	if (m_pUIRoot)
		m_pUIRoot->Update();

	if (g_pKeyManager->isOnceKeyDown('L'))
	{
		Save("map1");
	}

	if (g_pKeyManager->isOnceKeyDown('P'))
	{
		Load("map1");
	}

	
}

void cTestScene::Render()
{
	if (m_pGrid)
		m_pGrid->Render();

	if (m_pCamera)
		m_pCamera->Render();

	if (m_pPlayer)
		m_pPlayer->Render();

	

	for (size_t i = 0; i < m_vecOutMap.size(); ++i)
	{
		if (m_emState == SET_DONE)
			m_vecOutMap[i]->Render();
		else
			m_vecOutMap[i]->RenerComplete();
	}
	
	for (size_t i = 0; i < m_vecOutMonster.size(); ++i)
	{
		m_vecOutMonster[i]->Render();
	}

	if (m_pCurObj)
		m_pCurObj->RenerComplete();

	if (m_pCurMonster)
		m_pCurMonster->Render();

	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &mat);
	
	for (size_t i = 0; i < m_vecBoundBox.size(); ++i)
	{
		m_vecBoundBox[i]->DebugRender(D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	}

	g_pD3DDevice->SetTexture(0, NULL);

	if (m_pUIRoot)
		m_pUIRoot->Render(m_pSprite);

	
	DebugRender();
}

void cTestScene::DebugRender()
{
	LPD3DXFONT font;
	font = g_pFontManger->GetFont(cFontManager::E_NORMAL);

	char temp[512];
	sprintf_s(temp, "=================================", 512);
	RECT rc;
	SetRect(&rc, DEBUG_STARTX, DEBUG_STARTY + 110, DEBUG_STARTX + 600, DEBUG_STARTY + 125);
	font->DrawText(NULL,
		temp,
		128,
		&rc,
		DT_LEFT,
		D3DCOLOR_XRGB(255, 255, 255));

	sprintf_s(temp, "1 : SET_MAP \n2 : SET_MONSTER\n3 : SET_BOX\n4 : SET_DONE\nDelete : erase Object\nL : SaveMap", 512);

	SetRect(&rc, DEBUG_STARTX, DEBUG_STARTY + 130, DEBUG_STARTX + 600, DEBUG_STARTY + 235);
	//RECT rc = { DEBUG_STARTX, DEBUG_STARTY + 130, DEBUG_STARTX + 600, DEBUG_STARTY + 145 };
	font->DrawText(NULL,
		temp,
		128,
		&rc,
		DT_LEFT,
		D3DCOLOR_XRGB(255, 255, 255));

	sprintf_s(temp, "=================================", 512);

	SetRect(&rc, DEBUG_STARTX, DEBUG_STARTY + 220, DEBUG_STARTX + 600, DEBUG_STARTY + 230);
	//RECT rc = { DEBUG_STARTX, DEBUG_STARTY + 130, DEBUG_STARTX + 600, DEBUG_STARTY + 145 };
	font->DrawText(NULL,
		temp,
		128,
		&rc,
		DT_LEFT,
		D3DCOLOR_XRGB(255, 255, 255));

	switch (m_emState)
	{
	case SET_MAP:
		sprintf_s(temp, "CurrentMode : SET_MAP", 512);
		break;
	case SET_MONSTER:
		sprintf_s(temp, "CurrentMode : SET_MONSTER", 512);
		break;
	case SET_BOX:
		sprintf_s(temp, "CurrentMode : SET_BOX", 512);
		break;
	case SET_DONE:
		sprintf_s(temp, "CurrentMode : SET_DONE", 512);
		break;
	default:
		break;
	}

	SetRect(&rc, DEBUG_STARTX, DEBUG_STARTY + 240, DEBUG_STARTX + 600, DEBUG_STARTY + 260);
	//RECT rc = { DEBUG_STARTX, DEBUG_STARTY + 130, DEBUG_STARTX + 600, DEBUG_STARTY + 145 };
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
	

	float deltaX = pUI->GetPosition().x;
	float deltaY = pUI->GetPosition().y;

	RECT rc;

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

	}

	if (PtInRect(&rc, g_ptMouse))
		return true;

	return false;
}

void cTestScene::SetMap()
{
	switch (m_emState)
	{
		case SET_MAP:
			if (m_pCurObj)
			{
				cRay r = cRay::RayAtWorldSpace(g_ptMouse.x, g_ptMouse.y);
				D3DXVECTOR3 pickPos;
				float fAngle;
				for (size_t i = 0; i < m_vecTiles.size(); i += 3)
				{
					if (r.IntersectTri(m_vecTiles[i].p,
						m_vecTiles[i + 1].p,
						m_vecTiles[i + 2].p,
						pickPos))
					{

						if (g_pKeyManager->isOnceKeyDown(VK_LEFT))
						{
							D3DXMATRIXA16 matR;
							D3DXMatrixRotationY(&matR, D3DXToRadian(-90));
							D3DXVECTOR3 vDir = m_pCurObj->GetDirection();
							D3DXVec3TransformNormal(&vDir, &vDir, &matR);
							m_pCurObj->SetNewDirection(vDir);
							m_matR *= matR;
							
						}
						if (g_pKeyManager->isOnceKeyDown(VK_RIGHT))
						{
							D3DXMATRIXA16 matR;
							D3DXMatrixRotationY(&matR, D3DXToRadian(90));
							D3DXVECTOR3 vDir = m_pCurObj->GetDirection();
							D3DXVec3TransformNormal(&vDir, &vDir, &matR);
							m_pCurObj->SetNewDirection(vDir);
							m_matR *= matR;
							
						
						}
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

							D3DXMATRIXA16 matW, matT;
							D3DXMatrixTranslation(&matT, m_pCurObj->GetPosition().x, m_pCurObj->GetPosition().y, m_pCurObj->GetPosition().z);
							matW = m_matR * matT;
							m_pCurObj->SetLocalBoundBox(&matW);

							m_vecOutMap.push_back(m_pCurObj);

							m_pCurObj = NULL;
						}
						else
						{
							if (m_bisClone)
							{
								m_pCurObj->CloneMap(m_vecObj[m_nCurIndex]);

								m_bisClone = false;
							}
							m_pCurObj->SetPosition(pickPos);
						}


					}
				}
			}
			else
			{
				for (size_t i = 0; i < m_vecObjUI.size(); ++i)
				{
					//유아이를 선택 했다면
					if (InCollider(m_vecObjUI[i]))
					{
						if (g_pKeyManager->isOnceKeyDown(VK_LBUTTON))
						{
							m_pCurObj = new cMap;
							m_nCurIndex = i;
							m_bisClone = true;
						}
					}
				}

			}

			if (g_pKeyManager->isOnceKeyDown(VK_DELETE))
			{
				D3DXMatrixIdentity(&m_matR);
				if (m_pCurObj)
				{
					m_pCurObj->Release();
					m_pCurObj = NULL;
				}
				else
				{
					if (!m_vecOutMap.empty())
					{
						m_vecOutMap.back()->Release();
						m_vecOutMap.pop_back();
					}
						
				}
			}
		break;
			break;
		case SET_MONSTER:
			if (m_pCurMonster)
			{
				cRay r = cRay::RayAtWorldSpace(g_ptMouse.x, g_ptMouse.y);
				D3DXVECTOR3 pickPos = D3DXVECTOR3(0, 0, 0);
				for (size_t i = 0; i < m_vecTiles.size(); i += 3)
				{
					if (r.IntersectTri(m_vecTiles[i].p,
						m_vecTiles[i + 1].p,
						m_vecTiles[i + 2].p,
						pickPos))
					{
						if (g_pKeyManager->isOnceKeyDown(VK_RBUTTON))
						{
							//m_pCurMonster->Setup()
							pickPos.y = 0.0f;
							m_pCurMonster->SetPosition(pickPos);

							m_vecOutMonster.push_back(m_pCurMonster);

							m_pCurMonster = NULL;
						}
						else
						{
							if (m_bisClone)
							{
								//m_pCurMonster->SetAttackRange(m_vecMonster[m_nCurIndex]->GetAttackRange());
								
								m_pCurMonster->SetStat(m_vecMonster[m_nCurIndex]->GetStat());
								m_pCurMonster->Setup(StringToChar(m_vecMonster[m_nCurIndex]->GetObjName()));
								//m_pCurMonster->CloneMonster(m_vecMonster[m_nCurIndex]);;
								//m_pCurMonster->CloneMap(m_vecObj[m_nCurIndex]);

								m_bisClone = false;
							}
							m_pCurMonster->SetPosition(pickPos);

						}
						if (g_pKeyManager->isOnceKeyDown(VK_LEFT))
						{
							D3DXMATRIXA16 matR;
							D3DXMatrixRotationY(&matR, D3DXToRadian(-90));
							D3DXVECTOR3 vDir = m_pCurMonster->GetDirection();
							D3DXVec3TransformNormal(&vDir, &vDir, &matR);
							m_pCurMonster->SetNewDirection(vDir);

							int a = 0;
						}
						if (g_pKeyManager->isOnceKeyDown(VK_RIGHT))
						{
							D3DXMATRIXA16 matR;
							D3DXMatrixRotationY(&matR, D3DXToRadian(90));
							D3DXVECTOR3 vDir = m_pCurMonster->GetDirection();
							D3DXVec3TransformNormal(&vDir, &vDir, &matR);
							m_pCurMonster->SetNewDirection(vDir);
						}
					}
				}
			}
			else
			{
				for (size_t i = 0; i < m_vecMonsterUI.size(); ++i)
				{
					//유아이를 선택 했다면
					if (InCollider(m_vecMonsterUI[i]))
					{
						if (g_pKeyManager->isOnceKeyDown(VK_LBUTTON))
						{
							m_pCurMonster = new cMonster;
							m_nCurIndex = i;
							m_bisClone = true;
						}
					}
				}
			}

			if (g_pKeyManager->isOnceKeyDown(VK_DELETE))
			{
				D3DXMatrixIdentity(&m_matR);
				if (m_pCurMonster)
				{
					m_pCurMonster->Release();
					m_pCurMonster = NULL;
				}
				else
				{
					if (!m_vecOutMonster.empty())
					{
						m_vecOutMonster.back()->Release();
						m_vecOutMonster.pop_back();
					}
						
				}
			}
		break;
		case SET_BOX:
			SetBoundBox();
		break;
		case SET_DONE:
			

			if (m_pPlayer)
			{
				m_pPlayer->Update();
				PlayerMoveTest();
			}
				
		break;
		default:
		break;
	}
	
}

void cTestScene::StateChange()
{
	if (g_pKeyManager->isOnceKeyDown('1'))
	{
		D3DXMatrixIdentity(&m_matR);
		m_emState = SET_MAP;
		m_pUIRoot->SetDraw(cUIObject::Ui_Tag::thumbnail_Monster, false);
		m_pUIRoot->SetDraw(cUIObject::Ui_Tag::thumbnail_Object, true);
	}

	if (g_pKeyManager->isOnceKeyDown('2'))
	{
		D3DXMatrixIdentity(&m_matR);
		m_emState = SET_MONSTER;
		m_pUIRoot->SetDraw(cUIObject::Ui_Tag::thumbnail_Monster, true);
		m_pUIRoot->SetDraw(cUIObject::Ui_Tag::thumbnail_Object, false);

	}
	if (g_pKeyManager->isOnceKeyDown('3'))
		m_emState = SET_BOX;

	if (g_pKeyManager->isOnceKeyDown('4'))
		m_emState = SET_DONE;

}

void cTestScene::PlayerMoveTest()
{
	//CollisionTest();

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
				pickPos) && !CollisionTest())
			{
				cActionMove* pAction = new cActionMove;

				//m_pPlayer->SetIsMove(true);
				pAction->SetTo(pickPos);
				pAction->SetFrom(m_pPlayer->GetPosition());
				pAction->SetTarget(m_pPlayer);
				pAction->SetDelegate(m_pPlayer);
				pAction->SetSpeed(0.05f);
				pAction->SetOBB(m_vecBoundBox);
				pAction->Start();
				m_pPlayer->SetAction(pAction);
				m_pPlayer->GetMesh()->SetAnimationIndex("run");

				
				SAFE_RELEASE(pAction);
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
	if (!m_vecOutMap.empty())
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
		for (size_t i = 0; i < m_vecOutMap[nCurMap]->GetBoundBox().size(); ++i)
		{
			//사라질 오브젝트의 바운드박스가 카메라에서의 ray에 걸렸으면 distance를 반환
			if (m_vecOutMap[nCurMap]->GetBoundBox()[i]->GetRayDistance(
				m_pCamera->GetEye(), dir, disToHidden))
			{
				//사라질 오브젝트가 플레이어보다 가깝게 있으면
				if (disToHidden < disToPlayer)
				{
					//그 오브젝트를 그리지 않는다.
					m_vecOutMap[nCurMap]->GetHiddenDraw()[i] = true;
					break;
				}
			}
			//사라질 오브젝트의 바운드 박스가 카메라에서의 ray에 걸리지 않았으면
			else
				//그 오브젝트는 그린다.
				m_vecOutMap[nCurMap]->GetHiddenDraw()[i] = false;
		}

		for (size_t i = 0; i < m_vecOutMap.size(); ++i)
		{
			for (size_t j = 0; j < m_vecOutMap[i]->GetBoundBox().size(); ++j)
			{
				if (i == nCurMap) continue;
				m_vecOutMap[i]->GetHiddenDraw()[j] = false;
			}
		}
	}

}

void cTestScene::SetBoundBox()
{
	BOOL bHit = FALSE;
	DWORD dwFace = 0;
	D3DXVECTOR3 vRayPos;
	D3DXVECTOR3 vRayDir;
	FLOAT fBary1 = -1.0f;
	FLOAT fBary2 = -1.0f;
	FLOAT fDist = -1.0f;

	if (g_pKeyManager->isOnceKeyDown(VK_RBUTTON))
		{
			cRay r = cRay::RayAtWorldSpace(g_ptMouse.x, g_ptMouse.y);

			if (!m_bIsBound)
			{
				for (size_t i = 0; i < m_vecOutMap.size(); ++i)
				{
					vRayPos = r.GetOrg() - m_vecOutMap[i]->GetPosition();
					D3DXVec3Normalize(&vRayDir, &r.GetDir());

					LPD3DXMESH mesh = m_vecOutMap[i]->GetComMesh();
					D3DXIntersect(mesh, &vRayPos, &vRayDir
						, &bHit, &dwFace, &fBary1, &fBary2, &fDist, NULL, NULL);

					if (bHit)
					{
						m_vMin = vRayPos + fDist*vRayDir + m_vecOutMap[i]->GetPosition();
						m_bIsBound = true;
						break;
					}
				}
			}
			else
			{

				for (size_t i = 0; i < m_vecOutMap.size(); ++i)
				{
					vRayPos = r.GetOrg() - m_vecOutMap[i]->GetPosition();
					D3DXVec3Normalize(&vRayDir, &r.GetDir());

					LPD3DXMESH mesh = m_vecOutMap[i]->GetComMesh();
					D3DXIntersect(mesh, &vRayPos, &vRayDir
						, &bHit, &dwFace, &fBary1, &fBary2, &fDist, NULL, NULL);

					if (bHit)
					{
						m_vMax = vRayPos + fDist*vRayDir + m_vecOutMap[i]->GetPosition();

						//cBoundBox* box = new cBoundBox;
						//box->Setup(m_vMin, m_vMax, NULL);
						//box->SetIsDraw(true);
						//m_vecBoundBox.push_back(box);

						cOBB* box = new cOBB;
						box->Setup(m_vMin, m_vMax);
						m_vecBoundBox.push_back(box);
						m_bIsBound = false;
						break;
					}
				}
			}
			
		}

	if (g_pKeyManager->isToggleKey(VK_CONTROL) && g_pKeyManager->isOnceKeyDown('Z'))
	{
		if (m_bIsBound) 
		{
			m_bIsBound = false;
			m_vMin = D3DXVECTOR3(0, 0, 0);
			m_vMax = D3DXVECTOR3(0, 0, 0);
			return;
		}
		m_vecBoundBox.pop_back();
	}

}

bool cTestScene::CollisionTest()
{

	for (size_t i = 0; i < m_vecBoundBox.size(); ++i)
	{
		if (cOBB::IsCollision(m_pPlayer->GetOBB(), m_vecBoundBox[i]))
		{
			//m_pPlayer->SetIsMove(false);
			return true;
			break;
		}
		else
		{
			return false;
		}
	}
}

void cTestScene::Save(string fileName)
{
	string path = "./Resources/Object/";
	string exp = ".wobj";
	string fullName = path + fileName + exp;

	FILE* fp = NULL;

	fp = fopen(fullName.c_str(), "wb");

	assert(fp != NULL && "세이브 파일이 생성되지 않았습니다");


	//맵정보 저장

	int nObj = m_vecOutMap.size();
	fwrite(&nObj, sizeof(int), 1, fp);
	
	for (size_t i = 0; i < m_vecOutMap.size(); ++i)
	{
		ST_SAVEOBJECT wObj;
		ZeroMemory(&wObj, sizeof(ST_SAVEOBJECT));
	
		string name = m_vecOutMap[i]->GetObjName();
		strncpy(wObj.szfileName, name.c_str(), name.length());
		string folderName = m_vecOutMap[i]->GetFolderName();
		strncpy(wObj.szFolderName, folderName.c_str(), folderName.length());
		
		wObj.vPosition = m_vecOutMap[i]->GetPosition();
		wObj.vScale = m_vecOutMap[i]->GetScale();
		wObj.vForward = m_vecOutMap[i]->GetDirection();
		wObj.vUp = m_vecOutMap[i]->GetUpVector();
		
		D3DXVECTOR3 right;
		D3DXVec3Cross(&right, &wObj.vUp, &wObj.vForward);
		D3DXVec3Normalize(&wObj.vRight, &right);
	
		fwrite(&wObj, sizeof(ST_SAVEOBJECT), 1, fp);
	
	}

	//충돌박스 정보 저장
	int nBoxCount = m_vecBoundBox.size();
	fwrite(&nBoxCount, sizeof(int), 1, fp);
	for (size_t i = 0; i < m_vecBoundBox.size(); ++i)
	{
		D3DXVECTOR3 vMin = m_vecBoundBox[i]->GetMin();
		D3DXVECTOR3 vMax = m_vecBoundBox[i]->GetMax();

		fwrite(&vMin, sizeof(D3DXVECTOR3), 1, fp);
		fwrite(&vMax, sizeof(D3DXVECTOR3), 1, fp);
	}

	//몬스터 정보 저장
	int nMonsterCount = m_vecOutMonster.size();
	fwrite(&nMonsterCount, sizeof(int), 1, fp);
	for (size_t i = 0; i < m_vecOutMonster.size(); ++i)
	{
		ST_SAVEOBJECT wObj;
		ZeroMemory(&wObj, sizeof(ST_SAVEOBJECT));

		string name = m_vecOutMonster[i]->GetObjName();
		strncpy(wObj.szfileName, name.c_str(), name.length());
		string folderName = m_vecOutMonster[i]->GetFolderName();
		strncpy(wObj.szFolderName, folderName.c_str(), folderName.length());

		wObj.vPosition = m_vecOutMonster[i]->GetPosition();
		wObj.vScale = m_vecOutMonster[i]->GetScale();
		wObj.vForward = m_vecOutMonster[i]->GetDirection();
		wObj.vUp = m_vecOutMonster[i]->GetUpVector();

		D3DXVECTOR3 right;
		D3DXVec3Cross(&right, &wObj.vUp, &wObj.vForward);
		D3DXVec3Normalize(&wObj.vRight, &right);

		ST_MONSTER_STAT st;

		st = m_vecOutMonster[i]->GetStat();
		fwrite(&wObj, sizeof(ST_SAVEOBJECT), 1, fp);
		fwrite(&st, sizeof(ST_MONSTER_STAT), 1, fp);
	}

	fclose(fp);
}

void cTestScene::Load(string fileName)
{
	if (!m_vecOutMap.empty()) return;

	for (size_t i = 0; i < m_vecOutMap.size(); ++i)
	{
		SAFE_RELEASE(m_vecOutMap[i]);
	}

	string path = "./Resources/Object/";
	string exp = ".wobj";
	string fullName = path + fileName + exp;

	FILE* fp = NULL;

	fp = fopen(fullName.c_str(), "rb");

	assert(fp != NULL && "세이브 파일이 생성되지 않았습니다");

	//맵정보 로드
	int nObj;
	fread(&nObj, sizeof(int), 1, fp);
	
	for (int i = 0; i < nObj; ++i)
	{
		ST_SAVEOBJECT wObj;
		fread(&wObj, sizeof(ST_SAVEOBJECT), 1, fp);
		cMap* map = new cMap;
		
		map->Setup(wObj);
		m_vecOutMap.push_back(map);
	}

	//충돌박스 정보 로드
	int nBoxCount;
	fread(&nBoxCount, sizeof(int), 1, fp);
	for (size_t i = 0; i < nBoxCount; ++i)
	{
		
		D3DXVECTOR3 vMin;
		D3DXVECTOR3 vMax;

		fread(&vMin, sizeof(D3DXVECTOR3), 1, fp);
		fread(&vMax, sizeof(D3DXVECTOR3), 1, fp);
		cOBB* obb = new cOBB;
		obb->Setup(vMin, vMax);
		m_vecBoundBox.push_back(obb);
	}

	//몬스터정보 로드
	int nMonsterCount;
	fread(&nMonsterCount, sizeof(int), 1, fp);
	for (size_t i = 0; i < nMonsterCount; ++i)
	{
		ST_SAVEOBJECT wObj;
		fread(&wObj, sizeof(ST_SAVEOBJECT), 1, fp);

		ST_MONSTER_STAT st;
		fread(&st, sizeof(ST_MONSTER_STAT), 1, fp);

		cMonster* monster = new cMonster;

		monster->SetStat(st);
		monster->Setup(wObj);

		m_vecOutMonster.push_back(monster);
	}

	fclose(fp);

}

