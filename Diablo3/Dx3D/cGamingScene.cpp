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

cGamingScene::cGamingScene()
	: m_pGrid(NULL)
	, m_pPlayer(NULL)
{
	
}


cGamingScene::~cGamingScene()
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
	SAFE_RELEASE(m_pPlayer);

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
	//m_pPlayer->SetID(m_pPlayer->GetID());


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

	//LoadMap("map1");

	//g_pAIManager->RegisterAIBase(m_pPlayer);
	//
	//for (size_t i = 0; i < m_vecMonster.size(); ++i)
	//{
	//	g_pAIManager->RegisterAIBase(m_vecMonster[i]);
	//}

	//cGameObject* g = g_pAIManager->GetAIBaseFromID(0);
	//
	//g_pAIManager->RemoveAIBase(m_pPlayer);
	//SAFE_RELEASE(m_pPlayer);
	//
	//cGameObject* b = g_pAIManager->GetAIBaseFromID(0);
	//
	//int a = 0;

	return S_OK;
}

void cGamingScene::Update()
{

	PlayerMoveTest();

	if (m_pPlayer)
		m_pPlayer->Update();

	
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
		m_pCamera->Update(NULL);
	}

	
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

		cMonster* monster = new cMonster;

		monster->SetStat(st);
		monster->Setup(wObj);

		m_vecMonster.push_back(monster);
	}

	fclose(fp);
}

void cGamingScene::PlayerMoveTest()
{
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
