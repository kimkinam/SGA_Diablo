#include "stdafx.h"
#include "cGamingScene.h"
#include "cCamera.h"
#include "cGrid.h"
#include "cUiManager.h"
#include "cSkinnedMesh.h"
#include "cMap.h"
#include "cMonster.h"
#include "cPlayer.h"
#include "cActionMove.h"

#include "cFetish.h"	
#include "cSkeleton.h"
#include "cSkeletonArcher.h"
#include "cStitch.h"
#include "cZombieDog.h"
#include "cGargantuan.h"

#include "cUIImage.h"

cGamingScene::cGamingScene()
	: m_pGrid(NULL)
	, m_pPlayer(NULL)
	, m_pUI(NULL)
	, m_pCurMonster(NULL)
{
	
}


cGamingScene::~cGamingScene()
{
	g_pAIManager->Destroy();

	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
	SAFE_RELEASE(m_pPlayer);
	SAFE_DELETE(m_pUI);

	for each(auto c in m_vecMap)
	{
		SAFE_RELEASE(c);
	}

	for each(auto c in m_vecBoundBox)
	{
		SAFE_DELETE(c);
	}

	for each(auto c in m_vecMonster)
	{
		//c->Release();
		SAFE_RELEASE(c);
	}

}


HRESULT cGamingScene::SetUp()
{
	if (m_bIsLoad)
	{
		Reset();

		return S_OK;
	}
	cSceneObject::SetUp();

	m_pGrid = new cGrid;
	m_pGrid->Setup(30);

	m_bIsLoad = true;


	m_pPlayer = new cPlayer;
	m_pPlayer->Setup(&D3DXVECTOR3(0,0,1));

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

	LoadMap("map1");

	m_pPlayer->SetBoundBox(m_vecBoundBox);
	for (size_t i = 0; i < m_vecMonster.size(); ++i)
	{
		m_vecMonster[i]->SetBoundBox(m_vecBoundBox);
	}
	g_pAIManager->RegisterAIBase(m_pPlayer);

	D3DXVECTOR3 vDir;
	vDir = m_pPlayer->GetPosition() - GAMINGSCENE_CAMERAPOS;/*D3DXVECTOR3(24, 10, -17)*/;
	D3DXVec3Normalize(&vDir, &vDir);
	
	float distance = 8.67f;
	
	m_pCamera->SetEye(m_pPlayer->GetPosition() - vDir * distance);
	m_pCamera->SetNewDirection(vDir);

	UISetting();

	return S_OK;
}

void cGamingScene::Update()
{
	//assert(m_pPlayer->GetAction() && "wss");
	
	g_pMessageManager->MessageDelayedSend();

	for (m_vecMonsterIter = m_vecMonster.begin(); m_vecMonsterIter != m_vecMonster.end();)
	{
		//if ((*m_vecMonsterIter)->GetStat().fHp <= 0 && (*m_vecMonsterIter)->IsDoneCurAni())
		if((*m_vecMonsterIter)->GetRefCount() == 1)
		{
			(*m_vecMonsterIter)->Release();
			m_vecMonsterIter = m_vecMonster.erase(m_vecMonsterIter);
		}
		else
			++m_vecMonsterIter;
	}

	//if(g_pKeyManager->isOnceKeyDown('L'))
	//	LoadMap("map1");

	PlayerMoveTest();

	


	if (m_pPlayer)
	{
		m_pPlayer->Update();
		m_pPlayer->TrailUpdate();
	}
		
	for (size_t i = 0; i < m_vecMonster.size(); ++i)
	{
		m_vecMonster[i]->Update();
	}

	for (size_t i = 0; i < m_vecBoundBox.size(); ++i)
	{
		m_vecBoundBox[i]->Update(NULL);
	}

	if (m_pCamera)
	{
		m_pCamera->Update(m_pPlayer->GetPtPosition());
	}
	
	if (m_pUI)
		m_pUI->Update();
	
}

void cGamingScene::Render()
{
	if (m_pGrid)
		m_pGrid->Render();

	if (m_pPlayer)
		m_pPlayer->Render();
	
	for (size_t i = 0; i < m_vecMonster.size(); ++i)
	{
		m_vecMonster[i]->Render();
	}


	for (size_t i = 0; i < m_vecMap.size(); ++i)
	{
		m_vecMap[i]->Render();
	}

	for (size_t i = 0; i < m_vecBoundBox.size(); ++i)
	{
		m_vecBoundBox[i]->DebugRender(D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	}

	if (m_pCamera)
		m_pCamera->Render();

	if (m_pPlayer)
		m_pPlayer->TrailRender();

	if (m_pUI)
		m_pUI->Render();

	if (m_pCurMonster)
	{
		LPD3DXFONT font;
		font = g_pFontManger->GetFont(cFontManager::E_NORMAL);
	
		char temp[128];
		sprintf_s(temp, "hp :%f", m_pCurMonster->GetStat().fHp, 128);
		RECT rc;
		SetRect(&rc, DEBUG_STARTX, DEBUG_STARTY + 100, DEBUG_STARTX + 250, DEBUG_STARTY + 115);
		font->DrawText(NULL,
			temp,
			128,
			&rc,
			DT_LEFT,
			D3DCOLOR_XRGB(255, 255, 255));
	}
	

}

void cGamingScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pCamera)
		m_pCamera->WndProc(hWnd, message, wParam, lParam);
}

void cGamingScene::LoadMap(string fileName)
{
	if (!m_vecMap.empty()) return;

	for (size_t i = 0; i < m_vecMap.size(); ++i)
	{
		SAFE_RELEASE(m_vecMap[i]);
	}

	string path = "./Resources/Object/";
	string exp = ".wobj";
	string fullName = path + fileName + exp;

	FILE* fp = NULL;

	fp = fopen(fullName.c_str(), "rb");

	assert(fp != NULL && "���̺� ������ �������� �ʾҽ��ϴ�");

	//������ �ε�
	int nObj;
	fread(&nObj, sizeof(int), 1, fp);

	for (int i = 0; i < nObj; ++i)
	{
		ST_SAVEOBJECT wObj;
		fread(&wObj, sizeof(ST_SAVEOBJECT), 1, fp);
		cMap* map = new cMap;

		map->Setup(wObj);
		m_vecMap.push_back(map);
	}

	//�浹�ڽ� ���� �ε�
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

	//�������� �ε�
	int nMonsterCount;
	fread(&nMonsterCount, sizeof(int), 1, fp);
	for (size_t i = 0; i < nMonsterCount; ++i)
	{
		ST_SAVEOBJECT wObj;
		fread(&wObj, sizeof(ST_SAVEOBJECT), 1, fp);

		ST_MONSTER_STAT st;
		fread(&st, sizeof(ST_MONSTER_STAT), 1, fp);

		string name = wObj.szfileName;
		cMonster* monster = NULL;

		if (name == "Fetish.x")
		{
			monster = new cFetish;
		}
		else if (name == "Skeleton.x")
		{
			monster = new cSkeleton;
		}
		else if (name == "SkeletonArcher.x")
		{
			monster = new cSkeletonArcher;
		}
		else if (name == "stitch.x")
		{
			monster = new cStitch;
		}
		else if (name == "ZombieDog.x")
		{
			monster = new cZombieDog;
		}
		else if (name == "Gargantuan.x")
		{
			monster = new cGargantuan;
		}
		
		assert(monster != NULL && "���� x");

		monster->SetStat(st);
		monster->Setup(wObj);

		m_vecMonster.push_back(monster);
	}

	for (size_t i = 0; i < m_vecMonster.size(); ++i)
	{
		//m_vecMonster[i]->set
		m_vecMonster[i]->SetTarget(m_pPlayer);
		g_pAIManager->RegisterAIBase(m_vecMonster[i]);
	}

	fclose(fp);
}

void cGamingScene::UISetting()
{
	m_pUI = new cUiManager;
	m_pUI->SetAddressLink(m_pPlayer);
	m_pUI->SetUp();

	
}

void cGamingScene::PlayerMoveTest()
{
	cRay r = cRay::RayAtWorldSpace(g_ptMouse.x, g_ptMouse.y);

	//���͸� Ŭ���� ���
	for (size_t i = 0; i < m_vecMonster.size(); ++i)
	{
		if (r.IntersectShpere(m_vecMonster[i]->GetMesh()->GetBoundingSphere()))
		{
			m_pCurMonster = m_vecMonster[i];
			break;
		}
		else
			m_pCurMonster = NULL;
	}
	//�÷��̾� ��ŷ
	if (g_pKeyManager->isOnceKeyDown(VK_RBUTTON))
	{
		//�浹ó�� �ؾ��� �ڽ������� �÷��̾� ���ǵ带 �޽����� �����Ѵ�.
		ST_RUN_EXTRAINFO MSG;
		MSG.nBoxCount = m_vecBoundBox.size();
		MSG.vecBox = m_vecBoundBox;
		MSG.fSpeed = m_pPlayer->GetStat().fSpeed;

		//���̸� ������ǥ�� ��ȯ�Ѵ�.
		//cRay r = cRay::RayAtWorldSpace(g_ptMouse.x, g_ptMouse.y);

		//���͸� Ŭ���� ���
		for (size_t i = 0; i < m_vecMonster.size(); ++i)
		{
			if (r.IntersectShpere(m_vecMonster[i]->GetMesh()->GetBoundingSphere()) && !m_vecMonster[i]->GetStat().bIsDead)
			{
				if (m_vecMonster[i]->GetStat().fHp <= 0) return;
				//������ Ÿ���� �޼����� ��´�.
				MSG.nTarget = m_vecMonster[i]->GetID();

				//���ͱ����� ������ ���Ѵ�.
				D3DXVECTOR3 vDir = m_vecMonster[i]->GetPosition() - m_pPlayer->GetPosition();

				//���ͱ����� �Ÿ��� ���Ѵ�.
				float fDisToMonster = D3DXVec3Length(&vDir);

				//���͹��������� ĳ���� ��Ÿ��� ���Ѵ�.
				D3DXVec3Normalize(&vDir, &vDir);
				float fRange = D3DXVec3Length(&(vDir * m_pPlayer->GetStat().fAttackRange));
				
				//���ݻ�Ÿ����� �ָ� �ִ� ���
				if (fRange < fDisToMonster)
				{
					MSG.vDest = m_vecMonster[i]->GetPosition() - vDir * m_pPlayer->GetStat().fAttackRange;
					
				}
				//���ݻ�Ÿ� ���ʿ� �ִ� ���.
				else
				{
					MSG.vDest = m_pPlayer->GetPosition();
				}
				g_pMessageManager->MessageSend(0.0f, m_pPlayer->GetID(), m_pPlayer->GetID(), MESSAGE_TYPE::MSG_RUN, &MSG);
				
				//���͸� Ŭ���� ��� �ٴڰ��� ��ŷó���� ���� �ʴ´�.
				return;
			}
		}
		//�ٴڰ��� ��ŷó��
		D3DXVECTOR3 vPickPos;
		for (size_t i = 0; i < m_vecTiles.size(); i += 3)
		{
			if (r.IntersectTri(m_vecTiles[i].p,
				m_vecTiles[i + 1].p,
				m_vecTiles[i + 2].p,
				vPickPos) && !CollisionTest())
			{
				vPickPos.y = 0;
				
				MSG.nTarget = m_pPlayer->GetID();
				MSG.vDest = vPickPos;

				g_pMessageManager->MessageSend(0.0f, m_pPlayer->GetID(), m_pPlayer->GetID(), MESSAGE_TYPE::MSG_RUN, &MSG);
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
	if (!m_vecMap.empty() && m_pPlayer)
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
		for (size_t i = 0; i < m_vecMap[nCurMap]->GetBoundBox().size(); ++i)
		{
			//����� ������Ʈ�� �ٿ��ڽ��� ī�޶󿡼��� ray�� �ɷ����� distance�� ��ȯ
			if (m_vecMap[nCurMap]->GetBoundBox()[i]->GetRayDistance(
				m_pCamera->GetEye(), dir, disToHidden))
			{
				//����� ������Ʈ�� �÷��̾�� ������ ������
				if (disToHidden < disToPlayer)
				{
					//�� ������Ʈ�� �׸��� �ʴ´�.
					m_vecMap[nCurMap]->GetHiddenDraw()[i] = true;
					break;
				}
			}
			//����� ������Ʈ�� �ٿ�� �ڽ��� ī�޶󿡼��� ray�� �ɸ��� �ʾ�����
			else
				//�� ������Ʈ�� �׸���.
				m_vecMap[nCurMap]->GetHiddenDraw()[i] = false;
		}

		for (size_t i = 0; i < m_vecMap.size(); ++i)
		{
			for (size_t j = 0; j < m_vecMap[i]->GetBoundBox().size(); ++j)
			{
				if (i == nCurMap) continue;
				m_vecMap[i]->GetHiddenDraw()[j] = false;
			}
		}
	}
}

bool cGamingScene::CollisionTest()
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

