#include "stdafx.h"
#include "cBossScene.h"
#include "cBoss.h"
#include "cPlayer.h"
#include "cGrid.h"
#include "cCamera.h"
#include "cActionMove.h"
#include "cActionTrace.h"
#include "cSkinnedMesh.h"
#include "cSkeleton.h"

cBossScene::cBossScene()
	: m_pGrid(NULL)
	, m_pBoss(NULL)
	, m_pPlayer(NULL)
	, m_vpickPos(0, 0, 0)
{
}


cBossScene::~cBossScene()
{
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pCamera);
	SAFE_RELEASE(m_pBoss);
	SAFE_RELEASE(m_pPlayer);
	SAFE_RELEASE(m_pSkeleton);

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

	m_pPlayer = new cPlayer;
	m_pPlayer->SetUp();

	m_pBoss = new cBoss;
	m_pBoss->Setup();
	m_pBoss->SetTarget(m_pPlayer);
	m_pBoss->SetPosition(D3DXVECTOR3(10, 0, 10));

	m_pSkeleton = new cSkeleton;
	m_pSkeleton->Setup();
	m_pSkeleton->SetTarget(m_pPlayer);
	m_pSkeleton->SetPosition(D3DXVECTOR3(-10, 0, 10));

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
	//BossMoveTest();
	PlayerMove();

	if (m_pPlayer)
		m_pPlayer->Update();

	if (m_pBoss)
		m_pBoss->Update();

	if (m_pSkeleton)
		m_pSkeleton->Update();

	if (m_pCamera)
		m_pCamera->Update();

}

void cBossScene::Render()
{
	if (m_pGrid)
		m_pGrid->Render();

	if (m_pCamera)
		m_pCamera->Render();

	if (m_pPlayer)
		m_pPlayer->Render();

	if (m_pBoss)
		m_pBoss->Render();

	if (m_pSkeleton)
		m_pSkeleton->Render();

	LPD3DXFONT font;
	font = g_pFontManger->GetFont(cFontManager::E_NORMAL);

	char temp[512];
	if (m_pBoss->GetAction())
	{
		sprintf_s(temp, "Distance: %f", ((cActionTrace*)m_pBoss->GetAction())->GetDistance(), 512);

		RECT rc = { DEBUG_STARTX, DEBUG_STARTY + 120, DEBUG_STARTX + 250, DEBUG_STARTY + 150 };
		font->DrawText(NULL,
			temp,
			128,
			&rc,
			DT_LEFT,
			D3DCOLOR_XRGB(255, 255, 255));
	}

	sprintf_s(temp, "state: %d", m_pBoss->GetState(), 512);

	RECT rc = { DEBUG_STARTX, DEBUG_STARTY + 170, DEBUG_STARTX + 250, DEBUG_STARTY + 200 };
	font->DrawText(NULL,
		temp,
		128,
		&rc,
		DT_LEFT,
		D3DCOLOR_XRGB(255, 255, 255));

}


void cBossScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pCamera)
		m_pCamera->WndProc(hWnd, message, wParam, lParam);
}


void cBossScene::BossMoveTest()
{
	cActionTrace* trace = new cActionTrace;

	trace->SetTo(m_pPlayer->GetPtPosition());
	trace->SetFrom(m_pBoss->GetPtPosition());
	trace->SetTarget(m_pBoss);
	trace->SetDelegate(m_pBoss);
	trace->Start();
	m_pBoss->SetAction(trace);
	m_pBoss->GetMesh()->SetAnimationIndex("run");


}

void cBossScene::PlayerMove()
{
	if (g_pKeyManager->isOnceKeyDown(VK_RBUTTON))
	{
		cRay r = cRay::RayAtWorldSpace(g_ptMouse.x, g_ptMouse.y);
		D3DXVECTOR3 pickPos;
		for (size_t i = 0; i < m_vecTiles.size(); i += 3)
		{
			if (r.IntersectTri(m_vecTiles[i].p,
				m_vecTiles[i + 1].p,
				m_vecTiles[i + 2].p,
				pickPos))
			{
				m_pPlayer->SetIsMove(true);
				cActionMove* pAction = new cActionMove;

				pAction->SetTo(pickPos);
				pAction->SetFrom(m_pPlayer->GetPosition());
				pAction->SetTarget(m_pPlayer);
				pAction->SetDelegate(m_pPlayer);
				pAction->Start();
				m_pPlayer->SetAction(pAction);
				m_pPlayer->GetMesh()->SetAnimationIndex(0);

			}
		}
	}
}


