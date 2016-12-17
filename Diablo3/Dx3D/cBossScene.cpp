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
#include "cObjLoader.h"


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
	m_vecTiles.push_back(ST_PC_VERTEX(D3DXVECTOR3(0,  3.4f,   0), c));
	m_vecTiles.push_back(ST_PC_VERTEX(D3DXVECTOR3(20, 3.4f,   0), c));
	m_vecTiles.push_back(ST_PC_VERTEX(D3DXVECTOR3(20, 3.4f, -20), c));
	m_vecTiles.push_back(ST_PC_VERTEX(D3DXVECTOR3(20, 3.4f, -20), c));
	m_vecTiles.push_back(ST_PC_VERTEX(D3DXVECTOR3(0,  3.4f, -20), c));
	m_vecTiles.push_back(ST_PC_VERTEX(D3DXVECTOR3(0,  3.4f,   0), c));

	m_pPlayer = new cPlayer;
	m_pPlayer->SetPosition(D3DXVECTOR3(18, 3.4f, -18));
	D3DXVECTOR3 vDirection = D3DXVECTOR3(-1, 0, 1);
	D3DXVec3Normalize(&vDirection, &vDirection);
	m_pPlayer->Setup(&vDirection);
	

	m_pBoss = new cBoss;
	m_pBoss->SetTarget(m_pPlayer);
	m_pBoss->SetPosition(D3DXVECTOR3(10, 0, 10));
	m_pBoss->Setup(&D3DXVECTOR3(1, 0, 0)); 
	m_pBoss->SetID(1);

	m_pMap = new cObj;
	m_pMap->SetUp("DiabloMap2.objobj", "./Resources/Object/");

	g_pAIManager->RegisterAIBase(m_pPlayer);
	
	D3DXVECTOR3 vDir;
	vDir = m_pPlayer->GetPosition() - BOSSSCENE_CAMERAPOS;/*D3DXVECTOR3(24, 10, -17)*/;
	D3DXVec3Normalize(&vDir, &vDir);
	
	float distance = 9.0f;
	
	m_pCamera->SetEye(m_pPlayer->GetPosition() - vDir * distance);
	m_pCamera->SetNewDirection(vDir);


	m_fLifeTime = 2.0f;
	m_fLifeTimeDecrease = 0.01f;

	m_fTotalFireTime = 5.0f;
	m_fFireInterval = 0.2f;

	SetLight();
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
	{
		//m_pCamera->Update(m_pPlayer->GetPtPosition());
		m_pCamera->Update();
	}
	
}

void cBossScene::Render()
{
	//if (m_pGrid)
	//	m_pGrid->Render();




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

	/*if (m_pMap)
		m_pMap->Render();*/
	//g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	//g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	//g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);






	
	if (m_pMap)
		m_pMap->Render();
	

	if (m_pPlayer)
	{
		//if (m_pPlayer->m_pSateMachnie->GetCurState() == cPlayerAttackState::Instance())
			m_pPlayer->TrailRender();
	}
	if (m_pPlayer)
		m_pPlayer->SkillRender();
}


void cBossScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pCamera)
		m_pCamera->WndProc(hWnd, message, wParam, lParam);
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
				//cActionMove* pAction = new cActionMove;
				//
				//pAction->SetTo(pickPos);
				//pAction->SetFrom(m_pPlayer->GetPosition());
				//pAction->SetTarget(m_pPlayer);
				//pAction->SetDelegate(m_pPlayer);
				//pAction->SetSpeed(0.05f);
				//pAction->Start();
				//m_pPlayer->SetAction(pAction);
				//m_pPlayer->GetMesh()->SetAnimationIndex("run");
				ST_RUN_EXTRAINFO MSG;
				//MSG.nBoxCount = m_vecBoundBox.size();
				//MSG.vecBox = m_vecBoundBox;
				MSG.fSpeed = m_pPlayer->GetStat().fSpeed;
				
				MSG.nTarget = m_pPlayer->GetID();
				MSG.vDest = vPickPos;

				g_pMessageManager->MessageSend(0.0f, m_pPlayer->GetID(), m_pPlayer->GetID(), MESSAGE_TYPE::MSG_RUN, &MSG);

			}
		}
	}
}


void cBossScene::SetLight()
{
	D3DMATERIAL9 Mtl;
	ZeroMemory(&Mtl, sizeof(D3DMATERIAL9));
	Mtl.Diffuse.r = 0.1;
	Mtl.Diffuse.g = 0.1;
	Mtl.Diffuse.b = 0.1;
	g_pD3DDevice->SetMaterial(&Mtl);

	D3DLIGHT9 stLight;
	stLight.Ambient = D3DXCOLOR(0.01f, 0.01f, 0.01f, 1.0f);
	stLight.Diffuse = D3DXCOLOR(0.01f, 0.01f, 0.01f, 1.0f);
	stLight.Specular = D3DXCOLOR(0.01f, 0.01f, 0.01f, 1.0f);
	stLight.Type = D3DLIGHT_DIRECTIONAL;
	D3DXVECTOR3 vDir(1, -1, 1);
	D3DXVec3Normalize(&vDir, &vDir);
	stLight.Direction = vDir;
	g_pD3DDevice->SetRenderState(D3DRS_AMBIENT, 0x00202020);
	g_pD3DDevice->SetLight(2, &stLight);
	g_pD3DDevice->LightEnable(2, true);
	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);

}

