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
	, m_bIsCutScene(true)
	, m_fCutSceneTimer(0)
	, m_nBossTellCount(1)
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
	m_pBoss->SetPosition(D3DXVECTOR3(10, 3.4f, -10));
	m_pBoss->Setup(&(-vDirection));
	m_pBoss->SetTarget(m_pPlayer);
	m_pMap = new cObj;
	m_pMap->SetUp("DiabloMap.objobj", "./Resources/Object/");

	g_pAIManager->RegisterAIBase(m_pPlayer);
	g_pAIManager->RegisterAIBase(m_pBoss);

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

	m_pPlayer->Update();
	m_pBoss->Update();

	m_pBoss->SetState();

	D3DXVECTOR3 cutSceneCamDir = D3DXVECTOR3(-0.65f, -0.4f, 0.65f);
	D3DXVec3Normalize(&cutSceneCamDir, &cutSceneCamDir);
	m_pCamera->SetEye(D3DXVECTOR3(22 ,7.5f ,-22));

	m_pCamera->SetNewDirection(cutSceneCamDir);
	
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

	if(!m_bIsCutScene)
		if (!CutScene()) return;

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

	


	SetLight();
	
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
	//
	
	if (m_pMap)
		m_pMap->Render();
	
	
	if (m_pPlayer)
	{
	//if (m_pPlayer->m_pSateMachnie->GetCurState() == cPlayerAttackState::Instance())
			m_pPlayer->TrailRender();
	}

	if(m_pBoss)
		m_pBoss->ParticleTestRender();
	//if (m_pPlayer)
	//	m_pPlayer->SkillRender();
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
				ST_RUN_EXTRAINFO msg;
				msg.fSpeed = m_pPlayer->GetStat().fSpeed;

				msg.nTarget = m_pPlayer->GetID();
				msg.vDest = vPickPos;
				g_pMessageManager->MessageSend(0.0f, m_pPlayer->GetID(), m_pPlayer->GetID(), MESSAGE_TYPE::MSG_RUN, &msg);
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
}

bool cBossScene::CutScene()
{
	m_fCutSceneTimer += g_pTimeManager->GetDeltaTime();
	if (m_fCutSceneTimer <= 6.0f)
	{
		return false;
	}

	D3DXVECTOR3 vStart = m_pCamera->GetEye();
	vStart.y = m_pBoss->GetPosition().y;

	D3DXVECTOR3 vDir = vStart - m_pBoss->GetPosition();
	D3DXVec3Normalize(&vDir, &vDir);
	//
	D3DXVECTOR3 vDest = m_pBoss->GetPosition() + vDir *5.0f;
	//
	//D3DXVECTOR3 vDest =   D3DXVECTOR3(13.58f,9.57f ,-13.95f);
	D3DXVECTOR3 vPosition= vStart;// = m_pCamera->GetEye();
	D3DXVec3Lerp(&vPosition, &vStart, &vDest, g_pTimeManager->GetDeltaTime()*0.3f);
	if (D3DXVec3LengthSq(&D3DXVECTOR3(vPosition - vDest)) <= 0.5f*0.5f)
	{
		return CutScene2();
	}


	vPosition.y = m_pCamera->GetEye().y;

	//vPosition += 3.0f * -vDir * g_pTimeManager->GetDeltaTime();
	
	D3DXVECTOR3 cutSceneCamDir = D3DXVECTOR3(-0.65f, -0.4f,	0.65f);
	D3DXVec3Normalize(&cutSceneCamDir, &cutSceneCamDir);
	
	
	m_pCamera->SetEye(vPosition);
	m_pCamera->SetNewDirection(cutSceneCamDir);


	//m_pCamera->Update();
	//m_pPlayer->Update();
	//m_pBoss->Update();
	
	//D3DXVECTOR3 vDir;
	//vDir = m_pPlayer->GetPosition() - BOSSSCENE_CAMERAPOS;/*D3DXVECTOR3(24, 10, -17)*/;
	//D3DXVec3Normalize(&vDir, &vDir);
	//
	//float distance = 9.0f;
	//
	//m_pCamera->SetEye(m_pPlayer->GetPosition() - vDir * distance);
	//m_pCamera->SetNewDirection(vDir);

	return false;
}

bool cBossScene::CutScene2()
{
	string name = m_pBoss->GetCurAnimation()->GetName();

	if (name != "stom")
		m_pBoss->SetAnimation("stom");
	else
	{
		LPD3DXANIMATIONSET pCurAS = this->m_pBoss->GetCurAnimation();

		D3DXTRACK_DESC td;
		this->m_pBoss->GetMesh()->GetAnimController()->GetTrackDesc(0, &td);

		double dCurTime = pCurAS->GetPeriodicPosition(td.Position);
		double dTotalTime = pCurAS->GetPeriod();
		double dPercent = dCurTime / dTotalTime;

		//if (dPercent >= dTotalTime)
		if (dPercent > 0.9f)
		{
			m_nBossTellCount--;
			m_pBoss->SetAnimation("stom");
			//return true;
			return false;
		}
		if (m_nBossTellCount <= 0)
		{
			D3DXVECTOR3 vDir;
			vDir = m_pPlayer->GetPosition() - BOSSSCENE_CAMERAPOS;/*D3DXVECTOR3(24, 10, -17)*/;
			D3DXVec3Normalize(&vDir, &vDir);

			float distance = 9.0f;

			m_pCamera->SetEye(m_pPlayer->GetPosition() - vDir * distance);
			m_pCamera->SetNewDirection(vDir);

			m_pBoss->SetAnimation("idle");
			m_bIsCutScene = true;
			m_pBoss->SetState();
			return true;
		}
	}
	return false;
}


void cBossScene::SetLight()
{
	//D3DMATERIAL9 Mtl;
	//ZeroMemory(&Mtl, sizeof(D3DMATERIAL9));
	//Mtl.Diffuse.r = 0.5;
	//Mtl.Diffuse.g = 0.5;
	//Mtl.Diffuse.b = 0.5;
	//g_pD3DDevice->SetMaterial(&Mtl);

	D3DLIGHT9 stLight;
	stLight.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	stLight.Ambient = stLight.Specular = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
	stLight.Type = D3DLIGHT_DIRECTIONAL;
	D3DXVECTOR3 vDir(1, -1, 0);
	D3DXVec3Normalize(&vDir, &vDir);
	stLight.Direction = vDir;
	g_pD3DDevice->SetLight(0, &stLight);
	g_pD3DDevice->LightEnable(0, true);
	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);

}

