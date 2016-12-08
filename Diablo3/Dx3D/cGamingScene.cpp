#include "stdafx.h"
#include "cGamingScene.h"
#include "cCamera.h"
#include "cGrid.h"
#include "cUiManager.h"
#include "cSkinnedMesh.h"
#include "cMap.h"

cGamingScene::cGamingScene()
	: m_pGrid(NULL)
{
	
}


cGamingScene::~cGamingScene()
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
	//SAFE_DELETE(m_pUIManager);

	for each(auto c in m_vecMap)
	{
		SAFE_RELEASE(c);
	}

	for each(auto c in m_vecBoundBox)
	{
		SAFE_DELETE(c);
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
	//m_pUIManager = new cUiManager;
	//m_pUIManager->SetUp();

	m_bIsLoad = true;

	return S_OK;
}

void cGamingScene::Update()
{
	if (m_pUIManager)
		m_pUIManager->Update();

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

	if (m_pCamera)
		m_pCamera->Render();

	for (size_t i = 0; i < m_vecMap.size(); ++i)
	{
		m_vecMap[i]->Render();
	}
}

void cGamingScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pCamera)
		m_pCamera->WndProc(hWnd, message, wParam, lParam);
}

void cGamingScene::LoadMap(string fileName)
{
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

	//int nBoxCount;
	//
	//fread(&nBoxCount, sizeof(int), 1, fp);
	//
	//for (size_t i = 0; i < nBoxCount; ++i)
	//{
	//
	//	D3DXVECTOR3 vMin;
	//	D3DXVECTOR3 vMax;
	//
	//	fread(&vMin, sizeof(D3DXVECTOR3), 1, fp);
	//	fread(&vMax, sizeof(D3DXVECTOR3), 1, fp);
	//	cOBB* obb = new cOBB;
	//	obb->Setup(vMin, vMax);
	//	m_vecBoundBox.push_back(obb);
	//}
	//
	//int nMonsterCount;
	//
	//fread(&nMonsterCount, sizeof(int), 1, fp);
	//for (size_t i = 0; i < nMonsterCount; ++i)
	//{
	//	ST_SAVEOBJECT wObj;
	//	fread(&wObj, sizeof(ST_SAVEOBJECT), 1, fp);
	//
	//	cMonster* map = new cMonster;
	//
	//	map->Setup(wObj);
	//	m_vecOutMonster.push_back(map);
	//
	//}


	fclose(fp);
}