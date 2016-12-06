#include "stdafx.h"
#include "cLoadingScene.h"
#include "cGrid.h"
#include "cBoss.h"

cLoadingScene::cLoadingScene()
	: m_pGrid(NULL)
	, m_pBoss(NULL)
{
}


cLoadingScene::~cLoadingScene()
{
	SAFE_DELETE(m_pGrid);
	SAFE_RELEASE(m_pBoss);
}

HRESULT cLoadingScene::SetUp()
{
	if (m_bIsLoad)
	{
		Reset();

		return S_OK;
	}
	cSceneObject::SetUp();

	m_pGrid = new cGrid;
	m_pGrid->Setup(30);


	m_pBoss = new cBoss;
	m_pBoss->SetTarget(NULL);
	m_pBoss->SetPosition(D3DXVECTOR3(10, 0, 10));
	m_pBoss->Setup(&D3DXVECTOR3(1, 0, 0));
	m_pBoss->SetID(1);

	return S_OK;
}


void cLoadingScene::Update()

{
	if (m_pBoss)
		m_pBoss->Update();

}

void cLoadingScene::Render()
{
	if (m_pBoss)
		m_pBoss->Render();

	if (m_pGrid)
		m_pGrid->Render();

}

void cLoadingScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
}
