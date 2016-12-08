#include "stdafx.h"
#include "cGamingScene.h"
#include "cCamera.h"
#include "cGrid.h"
#include "cUiManager.h"
#include "cMonsterManager.h"
#include "cPlayerManager.h"
#include "cSkinnedMesh.h"
#include "cBoss.h"
#include "cMap.h"

cGamingScene::cGamingScene()
	: m_pGrid(NULL)
	, m_pMap(NULL)
	, m_pUIManager(NULL)
	, m_pMonsterManager(NULL)
	, m_pPlayerManager(NULL)
	, Boss_diablo(NULL)
{
	
}


cGamingScene::~cGamingScene()
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pMap);
	SAFE_DELETE(m_pUIManager);
	SAFE_DELETE(m_pPlayerManager);
	SAFE_RELEASE(Boss_diablo);

	for each(auto c in m_vecMap)
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

	//addressLink
	m_pUIManager = new cUiManager;
	m_pPlayerManager = new cPlayerManager;
	m_pMonsterManager = new cMonsterManager;


	//Boss_diablo = new cBoss;
	//Boss_diablo->Setup();

	m_pUIManager->SetAddressLink(m_pPlayerManager);
	m_pPlayerManager->SetAddressLink(m_pMonsterManager);
	m_pMonsterManager->SetAddressLink(m_pPlayerManager);

	m_pUIManager->SetUp();
	m_pPlayerManager->Setup();
	m_pMonsterManager->Setup();

	m_bIsLoad = true;

	return S_OK;
}

void cGamingScene::Update()
{
	if (m_pUIManager)
		m_pUIManager->Update();

	if (m_pPlayerManager)
		m_pPlayerManager->Update();

	if (m_pMonsterManager)
		m_pMonsterManager->Update();

	//if (Boss_diablo)
	//	Boss_diablo->Update();

	if (m_pCamera)
	{
		m_pCamera->Update(NULL);
	}

	if (g_pKeyManager->isOnceKeyDown('L'))
		LoadMap("map1");
}

void cGamingScene::Render()
{
	if (m_pGrid)
		m_pGrid->Render();

	if (m_pUIManager)
		m_pUIManager->Render();

	if (m_pPlayerManager)
		m_pPlayerManager->Render();

	if (m_pMonsterManager)
		m_pMonsterManager->Render();

	if (m_pCamera)
		m_pCamera->Render();

	//if (Boss_diablo)
	//	Boss_diablo->Render();

	for (size_t i = 0; i < m_vecMap.size(); ++i)
	{
		m_vecMap[i]->Render();
	}
}

void cGamingScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int a = 0;
	if (m_pCamera)
		m_pCamera->WndProc(hWnd, message, wParam, lParam);
}

void cGamingScene::LoadMap(string fileName)
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

	assert(fp != NULL && "세이브 파일이 생성되지 않았습니다");

	//int nObj;
	//fread(&nObj, sizeof(int), 1, fp);
	//
	//for (int i = 0; i < nObj; ++i)
	//{
	//	ST_SAVEOBJECT wObj;
	//	fread(&wObj, sizeof(ST_SAVEOBJECT), 1, fp);
	//	cMap* map = new cMap;
	//	
	//	map->Setup(wObj);
	//	m_vecOutMap.push_back(map);
	//}

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

	int nMonsterCount;

	fread(&nMonsterCount, sizeof(int), 1, fp);
	for (size_t i = 0; i < nMonsterCount; ++i)
	{
		ST_SAVEOBJECT wObj;
		fread(&wObj, sizeof(ST_SAVEOBJECT), 1, fp);

		cMonster* map = new cMonster;

		map->Setup(wObj);
		m_vecOutMonster.push_back(map);

	}


	fclose(fp);
}