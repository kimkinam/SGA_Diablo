#include "stdafx.h"
#include "cBossScene.h"
#include "cBoss.h"
#include "cPlayer.h"
#include "cGrid.h"
#include "cCamera.h"
#include "cActionMove.h"
#include "cActionTrace.h"
#include "cSkinnedMesh.h"
#include "cBoss.h"
#include "cObj.h"


cBossScene::cBossScene()
	: m_pGrid(NULL)
	, m_pBoss(NULL)
	, m_pPlayer(NULL)
	, m_pMap(NULL)
	, m_vpickPos(0, 0, 0)
{
}


cBossScene::~cBossScene()
{
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pCamera);
	SAFE_RELEASE(m_pMap);
	SAFE_RELEASE(m_pBoss);
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
	m_vecTiles.reserve(sizeof(ST_PC_VERTEX) * 6);
	m_vecTiles.push_back(ST_PC_VERTEX(D3DXVECTOR3(0, 3.4f, 0), c));
	m_vecTiles.push_back(ST_PC_VERTEX(D3DXVECTOR3(20, 3.4f, 0), c));
	m_vecTiles.push_back(ST_PC_VERTEX(D3DXVECTOR3(20, 3.4f, -20), c));
	m_vecTiles.push_back(ST_PC_VERTEX(D3DXVECTOR3(20, 3.4f, -20), c));
	m_vecTiles.push_back(ST_PC_VERTEX(D3DXVECTOR3(0, 3.4f, -20), c));
	m_vecTiles.push_back(ST_PC_VERTEX(D3DXVECTOR3(0, 3.4f, 0), c));


	m_pPlayer = new cPlayer;
	m_pPlayer->SetPosition(D3DXVECTOR3(18, 3.4f, -18));
	D3DXVECTOR3 vDirection = D3DXVECTOR3(-1, 0, 1);
	D3DXVec3Normalize(&vDirection, &vDirection);
	m_pPlayer->Setup(&vDirection);

	m_pBoss = new cBoss;
	m_pBoss->SetTarget(m_pPlayer);
	m_pBoss->SetPosition(D3DXVECTOR3(10, 3.4f, -10));
	m_pBoss->Setup(&D3DXVECTOR3(1, 0, 0));
	m_pBoss->SetID(1);

	m_pMap = new cObj;
	m_pMap->SetUp("DiabloMap.objobj", "./Resources/Object/");

	g_pAIManager->RegisterAIBase(m_pPlayer);

	D3DXVECTOR3 vDir;
	vDir = m_pPlayer->GetPosition() - BOSSSCENE_CAMERAPOS;/*D3DXVECTOR3(24, 10, -17)*/;
	D3DXVec3Normalize(&vDir, &vDir);

	float distance = 9.0f;

	m_pCamera->SetEye(m_pPlayer->GetPosition() - vDir * distance);
	m_pCamera->SetNewDirection(vDir);

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

	//g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	//g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	//g_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	//g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

	if (m_pMap)
		m_pMap->Render();
	//g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	//g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	//g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);

	if (m_pPlayer)
		m_pPlayer->TrailRender();




}


void cBossScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pCamera)
		m_pCamera->WndProc(hWnd, message, wParam, lParam);
}


void cBossScene::BossMoveTest()
{
	//cActionTrace* trace = new cActionTrace;
	//
	//trace->SetTo(m_pPlayer->GetPtPosition());
	//trace->SetFrom(m_pBoss->GetPtPosition());
	//trace->SetTarget(m_pBoss);
	//trace->SetDelegate(m_pBoss);
	//trace->Start();
	//m_pBoss->SetAction(trace);
	////m_pBoss->GetAni()->Play("run");
	//m_pBoss->GetMesh()->SetAnimationIndex("run");
	//
	//SAFE_RELEASE(trace);

}

void cBossScene::PlayerMove()
{
	if (g_pKeyManager->isOnceKeyDown(VK_RBUTTON))
	{

		cRay r = cRay::RayAtWorldSpace(g_ptMouse.x, g_ptMouse.y);
		D3DXVECTOR3 vPickPos;
		for (size_t i = 0; i < m_vecTiles.size(); i += 3)
		{
			if (r.IntersectTri(m_vecTiles[i].p,
				m_vecTiles[i + 1].p,
				m_vecTiles[i + 2].p,
				vPickPos))

			{
				ST_RUN_EXTRAINFO msg;
				msg.fSpeed = m_pPlayer->GetStat().fSpeed;

				msg.nTarget = m_pPlayer->GetID();
				msg.vDest = vPickPos;
				g_pMessageManager->MessageSend(0.0f, m_pPlayer->GetID(), m_pPlayer->GetID(), MESSAGE_TYPE::MSG_RUN, &msg);

			}

		}
	}
}



