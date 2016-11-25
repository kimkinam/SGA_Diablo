#include "stdafx.h"
#include "cTestScene.h"
#include "cGrid.h"
#include "cCamera.h"
#include "cSkinnedMesh.h"
#include "cSkinnedMeshManager.h"
#include "cObj.h"
#include "cObjLoader.h"
#include "cCube.h"
#include "cGroup.h"


cTestScene::cTestScene()
	: m_pGrid(NULL)
	, m_pMesh(NULL)
	//, m_pSword(NULL)
	, m_pCube(NULL)
	, m_pCamera(NULL)
{
	m_pCamera = new cCamera;
	m_pCamera->Setup();

	m_pGrid = new cGrid;
	m_pGrid->Setup(30);

	m_pMesh = new cSkinnedMesh("./Resources/Player/", "Bab2.X");
	m_pMesh->SetAnimationIndex(5);

	m_pSword = new cObj;
	m_pSword->LoadFile("twohandsword.objobj", "./Resources/Obj/");

	m_pSword->SetWorldTM(m_pMesh->AttachItem("right_weapon"));

	//g_pLogManager->Init(LOG_WINDOW | LOG_FILE, g_hWnd, "Dump");

	m_pCube = new cCube;
	m_pCube->Setup(NULL);
}


cTestScene::~cTestScene()
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);

	SAFE_DELETE(m_pMesh);
	SAFE_DELETE(m_pSword);
	SAFE_DELETE(m_pCube);

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
	if (m_pCamera)
	{
		m_pCamera->Update(NULL);
	}

	//if (g_pKeyManager->isOnceKeyDown('1'))
	//{
	//	m_pMesh->ChangeItem("Barb_M_MED_Gloves", "./Zealot/Barb_M_MED_Norm_Base_B_diff.dds");
	//}
	//if (g_pKeyManager->isOnceKeyDown('2'))
	//{
	//	m_pMesh->ChangeItem("Barb_M_MED_Pants", "./Zealot/Barb_M_MED_Norm_Base_B_diff.dds");
	//}
	//if (g_pKeyManager->isOnceKeyDown('3'))
	//{
	//	m_pMesh->ChangeItem("Barb_M_MED_Cloth", "./Zealot/Barb_M_MED_Norm_Base_B_diff.dds");
	//}
	//if (g_pKeyManager->isOnceKeyDown('4'))
	//{
	//	m_pMesh->ChangeItem("Barb_M_MED_Armor", "./Zealot/Barb_M_MED_Norm_Base_B_diff.dds");
	//}
	//if (g_pKeyManager->isOnceKeyDown('5'))
	//{
	//	m_pMesh->ChangeItem("Barb_M_MED_Boots", "./Zealot/Barb_M_MED_Norm_Base_B_diff.dds");
	//}
	
}

void cTestScene::Render()
{
	if (m_pGrid)
		m_pGrid->Render();
	
	if (m_pCamera)
		m_pCamera->Render();

	if (m_pMesh)
	{
		m_pMesh->UpdateAndRender();
	}

	if (m_pSword)
		m_pSword->Render();

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
