#include "stdafx.h"
#include "cTestScene.h"
#include "cGrid.h"
#include "cCamera.h"
#include "cSkinnedMesh.h"
#include "cObj.h"


cTestScene::cTestScene()
	: m_pGrid(NULL)
	, m_pMesh(NULL)
	, m_pSword(NULL)
{
}


cTestScene::~cTestScene()
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);

	SAFE_DELETE(m_pMesh);
	SAFE_DELETE(m_pSword);
}

void cTestScene::SetUp()
{
	m_pGrid = new cGrid;
	m_pGrid->Setup(30);

	m_pMesh = new cSkinnedMesh;
	m_pMesh->SetIsPlaying(true);

	m_pMesh->Load("Zealot", "Bab2.X");

	m_pSword = new cObj;
	m_pSword->LoadFile("twohandsword.obj", "Zealot");

	m_pSword->SetWorldTM(m_pMesh->FindWithName("right_weapon"));

	int a = 0;

}

void cTestScene::Update()
{
	if (m_pCamera)
	{
		m_pCamera->Update(NULL);
	}

	if (g_pKeyManager->isOnceKeyDown('1'))
	{
		m_pMesh->FindFrame("Barb_M_MED_Gloves", "./Zealot/Barb_M_MED_Norm_Base_B_diff.dds");
	}
	if (g_pKeyManager->isOnceKeyDown('2'))
	{
		m_pMesh->FindFrame("Barb_M_MED_Pants", "./Zealot/Barb_M_MED_Norm_Base_B_diff.dds");
	}
	if (g_pKeyManager->isOnceKeyDown('3'))
	{
		m_pMesh->FindFrame("Barb_M_MED_Cloth", "./Zealot/Barb_M_MED_Norm_Base_B_diff.dds");
	}
	if (g_pKeyManager->isOnceKeyDown('4'))
	{
		m_pMesh->FindFrame("Barb_M_MED_Armor", "./Zealot/Barb_M_MED_Norm_Base_B_diff.dds");
	}
	if (g_pKeyManager->isOnceKeyDown('5'))
	{
		m_pMesh->FindFrame("Barb_M_MED_Boots", "./Zealot/Barb_M_MED_Norm_Base_B_diff.dds");
	}
	if (m_pMesh)
	{
		m_pMesh->Update();
	}
}

void cTestScene::Render()
{
	if (m_pGrid)
		m_pGrid->Render();

	if (m_pCamera)
		m_pCamera->Render();

	if (m_pMesh)
		m_pMesh->Render();

	m_pSword->Render();

}

void cTestScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pCamera)
		m_pCamera->WndProc(hWnd, message, wParam, lParam);
}
