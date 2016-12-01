#include "stdafx.h"
#include "cBossScene.h"
#include "cBoss.h"
#include "cGrid.h"
#include "cCamera.h"

cBossScene::cBossScene()
	: Boss_diablo(NULL)
{
}


cBossScene::~cBossScene()
{
	SAFE_RELEASE(Boss_diablo)
}

HRESULT cBossScene::SetUp()
{

	if (m_bIsLoad)
	{
		Reset();

		return S_OK;
	}

	cSceneObject::SetUp();

	m_pGrid = new cGrid;
	m_pGrid->Setup(30);

	Boss_diablo = new cBoss;
	Boss_diablo->Setup();

	m_bIsLoad = true;
}

HRESULT cBossScene::Reset()
{
	m_pCamera->SetEye(D3DXVECTOR3(0, 0, -5));
	m_pCamera->SetLookAt(D3DXVECTOR3(0, 0, 0));
	m_pCamera->SetUp(D3DXVECTOR3(0, 1, 0));
	m_pCamera->SetForward(D3DXVECTOR3(0, 0, 1));
	m_pCamera->SetRright(D3DXVECTOR3(1, 0, 0));

	return S_OK;
}

void cBossScene::Update()
{
	if (Boss_diablo)
		Boss_diablo->Update();

	if (m_pCamera)
		m_pCamera->Update();

}

void cBossScene::Render()
{
	if (m_pGrid)
		m_pGrid->Render();

	if (Boss_diablo)
		Boss_diablo->Render();

	if (m_pCamera)
		m_pCamera->Render();
}

void cBossScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pCamera)
		m_pCamera->WndProc(hWnd, message, wParam, lParam);

}
