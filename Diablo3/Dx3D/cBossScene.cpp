#include "stdafx.h"
#include "cBossScene.h"
#include "cBoss.h"
#include "cPlayer.h"
#include "cGrid.h"
#include "cCamera.h"
#include "cActionMove.h"
#include "cActionTrace.h"
#include "cSkinnedMesh.h"

cBossScene::cBossScene()
	: m_pGrid(NULL)
	, Boss_diablo(NULL)
	, m_pPlayer(NULL)
	, m_vpickPos(0, 0, 0)
	, m_bIsSetMap(false)
	, m_isAnim(false)
	, m_isTurn(false)
{
}


cBossScene::~cBossScene()
{
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pCamera);
	SAFE_RELEASE(Boss_diablo);
	SAFE_RELEASE(m_pPlayer);

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
	m_vecTiles.reserve(sizeof(ST_PC_VERTEX)* 6);
	m_vecTiles.push_back(ST_PC_VERTEX(D3DXVECTOR3(-120, 0, 120), c));
	m_vecTiles.push_back(ST_PC_VERTEX(D3DXVECTOR3(120, 0, 120), c));
	m_vecTiles.push_back(ST_PC_VERTEX(D3DXVECTOR3(120, 0, -120), c));
	m_vecTiles.push_back(ST_PC_VERTEX(D3DXVECTOR3(120, 0, -120), c));
	m_vecTiles.push_back(ST_PC_VERTEX(D3DXVECTOR3(-120, 0, -120), c));
	m_vecTiles.push_back(ST_PC_VERTEX(D3DXVECTOR3(-120, 0, 120), c));

	m_pPlayer = new cPlayer;
	m_pPlayer->SetUp();

	Boss_diablo = new cBoss;
	Boss_diablo->Setup();
	Boss_diablo->SetTarget(m_pPlayer);
	Boss_diablo->SetPosition(D3DXVECTOR3(0, 0, 10));

	//BossMoveTest();

	Boss_diablo->GetPosition().x = 10.f;
	Boss_diablo->GetPosition().z = 10.f;

	//BossMoveTest();

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
	//BossMoveTest();
	PlayerMove();

	if (m_pPlayer)
		m_pPlayer->Update();

	if (Boss_diablo)
		Boss_diablo->Update();

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

	if (Boss_diablo)
		Boss_diablo->Render();

	LPD3DXFONT font;
	font = g_pFontManger->GetFont(cFontManager::E_NORMAL);

	char temp[512];
	if (Boss_diablo->GetAction())
	sprintf_s(temp, "Distance: %f", ((cActionTrace*)Boss_diablo->GetAction())->GetDistance(), 512);

	RECT rc = { DEBUG_STARTX, DEBUG_STARTY + 120, DEBUG_STARTX + 250, DEBUG_STARTY + 150 };
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
	trace->SetFrom(Boss_diablo->GetPtPosition());
	trace->SetTarget(Boss_diablo);
	trace->SetDelegate(Boss_diablo);
	trace->Start();
	Boss_diablo->SetAction(trace);
	Boss_diablo->GetMesh()->SetAnimationIndex("run");
	

	m_bIsSetMap = true;
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
				cActionMove* pAction = new cActionMove;

				pAction->SetTo(pickPos);
				pAction->SetFrom(m_pPlayer->GetPosition());
				pAction->SetTarget(m_pPlayer);
				pAction->SetDelegate(m_pPlayer);
				pAction->Start();
				m_pPlayer->SetAction(pAction);
				m_pPlayer->GetMesh()->SetAnimationIndex(0);

				m_bIsSetMap = true;
			}
		}
	}
}


