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

UINT cUIObject::monsterCount = 0;
UINT cUIObject::objCount = 0;


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
	
}


cTestScene::~cTestScene()
{
	//ULONG ul = g_pD3DDevice->Release();
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

	//�÷��̾�
	m_pPlayer = new cPlayer;
	m_pPlayer->SetUp();

	//����
	cMonster* Fetish = new cMonster;
	Fetish->Setup("Fetish.x");
	Fetish->SetSumNailName("Fetish.png");
	m_vecMonster.push_back(Fetish);

	cMonster* Skeleton = new cMonster;
	Skeleton->Setup("Skeleton.x");
	Skeleton->SetSumNailName("Skeleton.png");
	m_vecMonster.push_back(Skeleton);

	cMonster* SkeletonArcher = new cMonster;
	SkeletonArcher->Setup("skeletonArcher.x");
	SkeletonArcher->SetSumNailName("skeletonArcher.png");
	m_vecMonster.push_back(SkeletonArcher);

	cMonster* Gargantuan = new cMonster;
	Gargantuan->Setup("Gargantuan.x");
	Gargantuan->SetSumNailName("Gargantuan.png");
	m_vecMonster.push_back(Gargantuan);

	cMonster* ZombieDog = new cMonster;
	ZombieDog->Setup("ZombieDog.x");
	ZombieDog->SetSumNailName("ZombieDog.png");
	m_vecMonster.push_back(ZombieDog);

	cMonster* Stitch = new cMonster;
	Stitch->Setup("Stitch.x");
	Stitch->SetSumNailName("Stitch.png");
	m_vecMonster.push_back(Stitch);



	////������ ��
	//cMap* obj1 = new cMap;
	//obj1->SetScale(D3DXVECTOR3(0.5f, 0.5f, 0.5f));
	//obj1->Setup("a1dun_01_test.objobj", "./Resources/Object/");
	//obj1->SetSumNailName("a1Dun_01.jpg");
	//m_vecObj.push_back(obj1);
	//
	//cMap* obj2 = new cMap;
	//obj2->SetScale(D3DXVECTOR3(0.5f, 0.5f, 0.5f));
	//obj2->Setup("a1dun_02_test.objobj", "./Resources/Object/");
	//obj2->SetSumNailName("a1Dun_02.jpg");
	//m_vecObj.push_back(obj2);


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

	LPD3DXFONT font;
	font = g_pFontManger->GetFont(cFontManager::E_NORMAL);

	char temp[512];
	sprintf_s(temp, "PlayerPos : %.2f, %.2f, %.2f",
		m_pPlayer->GetPosition().x,
		m_pPlayer->GetPosition().y,
		m_pPlayer->GetPosition().z,
		//m_pPlayer->GetCurMap(),
		
		512);
	RECT rc = { DEBUG_STARTX, DEBUG_STARTY + 150, DEBUG_STARTX + 600, DEBUG_STARTY + 165 };
	font->DrawText(NULL,
		temp,
		128,
		&rc,
		DT_LEFT,
		D3DCOLOR_XRGB(255, 255, 255));

	sprintf_s(temp, "BoundBoxCount : %d // IsBound : %d",
		m_vecBoundBox.size(), m_bIsBound,
		512);
	rc = { DEBUG_STARTX, DEBUG_STARTY + 200, DEBUG_STARTX + 600, DEBUG_STARTY + 215 };
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
	if (pSender->GetTag() == (cUIObject::Ui_Tag)1) // ����
	{
		m_pUIRoot->FindChildByTag((cUIObject::Ui_Tag)3);
	}
	if (pSender->GetTag() == (cUIObject::Ui_Tag)2) // ������
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

							m_pCurObj->SetLocalBoundBox(&m_pCurObj->GetPosition());

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
					//�����̸� ���� �ߴٸ�
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
				if (m_pCurObj)
				{
					m_pCurObj->Release();
					m_pCurObj = NULL;
				}
				else
				{
					if(!m_vecOutMap.empty())
						m_vecOutMap.pop_back();
				}
			}
		break;
			break;
		case SET_MONSTER:
			if (m_pCurMonster)
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

							m_pCurMonster->SetPosition(pickPos);

							m_vecOutMonster.push_back(m_pCurMonster);

							m_pCurMonster = NULL;
						}
						else
						{
							if (m_bisClone)
							{
								m_pCurMonster->CloneMonster(m_vecMonster[m_nCurIndex]);
								//m_pCurMonster->CloneMap(m_vecObj[m_nCurIndex]);

								m_bisClone = false;
							}
							m_pCurMonster->SetPosition(pickPos);

						}

					}
				}
			}
			else
			{
				for (size_t i = 0; i < m_vecMonsterUI.size(); ++i)
				{
					//�����̸� ���� �ߴٸ�
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
				if (m_pCurMonster)
				{
					m_pCurMonster->Release();
					m_pCurMonster = NULL;
				}
				else
				{
					if (!m_vecOutMonster.empty())
						m_vecOutMonster.pop_back();
				}
			}
		break;
		case SET_BOX:
			SetBoundBox();
		break;
		case SET_DONE:
			PlayerMoveTest();

			if (m_pPlayer)
				m_pPlayer->Update();
		break;
		default:
		break;
	}
	
}

void cTestScene::StateChange()
{
	if (g_pKeyManager->isOnceKeyDown('1'))
	{
		m_emState = SET_MAP;
		m_pUIRoot->SetDraw(cUIObject::Ui_Tag::thumbnail_Monster, false);
		m_pUIRoot->SetDraw(cUIObject::Ui_Tag::thumbnail_Object, true);
	}

	if (g_pKeyManager->isOnceKeyDown('2'))
	{
		m_emState = SET_MONSTER;
		m_pUIRoot->SetDraw(cUIObject::Ui_Tag::thumbnail_Monster, true);
		m_pUIRoot->SetDraw(cUIObject::Ui_Tag::thumbnail_Object, false);

	}
	if (g_pKeyManager->isOnceKeyDown('3'))
		m_emState = SET_BOX;

	if (g_pKeyManager->isOnceKeyDown(VK_SPACE))
		m_emState = SET_DONE;

}

void cTestScene::PlayerMoveTest()
{
	//CollisionTest();

	//�÷��̾� ��ŷ
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

				m_pPlayer->SetIsMove(true);
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

	//�÷��̾� ������ ��ȭ �׽�Ʈ

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

	//������� ������Ʈ �׽�Ʈ
	if (!m_vecOutMap.empty())
	{
		//���� �÷��̾��� ��
		int nCurMap = m_pPlayer->GetCurMap();

		//�÷��̾ �ٶ󺸴� ī�޶� ����
		D3DXVECTOR3 dir = m_pPlayer->GetPosition() - m_pCamera->GetEye();

		//�÷��̾�� ī�޶��� �Ÿ�
		float disToPlayer = D3DXVec3Length(&dir);

		//����� ������Ʈ�� ī�޶��� �Ÿ�
		float disToHidden = 0.0f;

		//���� �ʿ��� ��������ִ� ������Ʈ�� �ٿ��ڽ��� �˻�
		for (size_t i = 0; i < m_vecOutMap[nCurMap]->GetBoundBox().size(); ++i)
		{
			//����� ������Ʈ�� �ٿ��ڽ��� ī�޶󿡼��� ray�� �ɷ����� distance�� ��ȯ
			if (m_vecOutMap[nCurMap]->GetBoundBox()[i]->GetRayDistance(
				m_pCamera->GetEye(), dir, disToHidden))
			{
				//����� ������Ʈ�� �÷��̾�� ������ ������
				if (disToHidden < disToPlayer)
				{
					//�� ������Ʈ�� �׸��� �ʴ´�.
					m_vecOutMap[nCurMap]->GetHiddenDraw()[i] = true;
					break;
				}
			}
			//����� ������Ʈ�� �ٿ�� �ڽ��� ī�޶󿡼��� ray�� �ɸ��� �ʾ�����
			else
				//�� ������Ʈ�� �׸���.
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
			m_pPlayer->SetIsMove(false);
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

	assert(fp != NULL && "���̺� ������ �������� �ʾҽ��ϴ�");

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
		wObj.vForward = m_vecOutMap[i]->GetForward();
		wObj.vUp = m_vecOutMap[i]->GetUp();
		
		D3DXVECTOR3 right;
		D3DXVec3Cross(&right, &wObj.vUp, &wObj.vForward);
		D3DXVec3Normalize(&wObj.vRight, &right);

		fwrite(&wObj, sizeof(ST_SAVEOBJECT), 1, fp);

	}

	int nBoxCount = m_vecBoundBox.size();

	fwrite(&nBoxCount, sizeof(int), 1, fp);

	for (size_t i = 0; i < m_vecBoundBox.size(); ++i)
	{
		D3DXVECTOR3 vMin = m_vecBoundBox[i]->GetMin();
		D3DXVECTOR3 vMax = m_vecBoundBox[i]->GetMax();

		fwrite(&vMin, sizeof(D3DXVECTOR3), 1, fp);
		fwrite(&vMax, sizeof(D3DXVECTOR3), 1, fp);
	}

	int a = 0;
	/*string path = "./Resources/Object/";
	string exp = ".wBoundBox";
	string fullName = path + fileName + exp;

	FILE* fp = NULL;

	fp = fopen(fullName.c_str(), "wb");

	assert(fp != NULL && "���̺� ������ �������� �ʾҽ��ϴ�");

	int nObj = m_vecMap.size();
	fwrite(&nObj, sizeof(int), 1, fp);*/

	fclose(fp);
}

void cTestScene::Load(string fileName)
{
	for (size_t i = 0; i < m_vecOutMap.size(); ++i)
	{
		SAFE_RELEASE(m_vecOutMap[i]);
	}

	string path = "./Resources/Object/";
	string exp = ".wobj";
	string fullName = path + fileName + exp;

	FILE* fp = NULL;

	fp = fopen(fullName.c_str(), "rb");

	assert(fp != NULL && "���̺� ������ �������� �ʾҽ��ϴ�");

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


	fclose(fp);

}

