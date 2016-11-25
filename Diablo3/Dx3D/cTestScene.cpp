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
	, m_pSword(NULL)
	, m_pCube(NULL)
{
}


cTestScene::~cTestScene()
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);

	SAFE_DELETE(m_pMesh);
	SAFE_DELETE(m_pSword);
	SAFE_DELETE(m_pCube);
}

void cTestScene::SetUp()
{
	m_pGrid = new cGrid;
	m_pGrid->Setup(30);

	m_pMesh = new cSkinnedMesh("Zealot/", "Bab2.X");
	m_pMesh->SetAnimationIndex(5);

	m_pSword = new cObj;
	m_pSword->LoadFile("twohandsword.obj", "Zealot");

	//cObjLoader l;
	//l.Load("./Zealot/Sword.obj", m_vecGroup);

	m_pSword->SetWorldTM(m_pMesh->AttachItem("right_weapon"));

	

	//m_pCube = new cCube;
	//m_pCube->Setup(NULL);
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

	g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);

	if (m_pSword)
		m_pSword->Render();

	

	//D3DXMATRIX matT;
	//D3DXMatrixTranslation(&matT, 1, 1, 0);
	//g_pD3DDevice->SetTransform(D3DTS_WORLD, &matT);
	//for each(auto p in m_vecGroup)
	//{
	//	p->Render();
	//}

	if (m_pCube)
		m_pCube->Render();



}

void cTestScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pCamera)
		m_pCamera->WndProc(hWnd, message, wParam, lParam);
}
