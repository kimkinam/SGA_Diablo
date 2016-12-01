#include "stdafx.h"
#include "cGamingScene.h"
#include "cCamera.h"
#include "cGrid.h"
#include "cUiManager.h"
#include "cMonsterManager.h"
#include "cPlayerManager.h"
#include "cSkinnedMesh.h"
#include "cBoss.h"


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
	SAFE_RELEASE(Boss_diablo)

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


	Boss_diablo = new cBoss;
	Boss_diablo->Setup();

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

	if (Boss_diablo)
		Boss_diablo->Update();

	if (m_pCamera)
	{
		m_pCamera->Update(NULL);
	}
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

	if (Boss_diablo)
		Boss_diablo->Render();
}

void cGamingScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int a = 0;
	if (m_pCamera)
		m_pCamera->WndProc(hWnd, message, wParam, lParam);
}
