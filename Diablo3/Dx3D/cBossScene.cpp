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
#include "cUiManager.h"


cBossScene::cBossScene()
	: m_pGrid(NULL)
	, m_pBoss(NULL)
	, m_pPlayer(NULL)
	, m_pMap(NULL)
	, m_vpickPos(0, 0, 0)
	, m_pUI(NULL)
	
{
}


cBossScene::~cBossScene()
{
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pCamera);
	SAFE_RELEASE(m_pMap);
	SAFE_RELEASE(m_pBoss);
	SAFE_RELEASE(m_pPlayer);
	SAFE_DELETE(m_pUI);

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
	m_pBoss->Setup(&D3DXVECTOR3(1, 0, 0));

	m_pMap = new cObj;
	m_pMap->SetUp("DiabloMap.objobj", "./Resources/Object/");

	

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

	g_pAIManager->RegisterAIBase(m_pPlayer);
	g_pAIManager->RegisterAIBase(m_pBoss);

	m_pPlayer->SetTarget(m_pBoss);
	m_pBoss->SetTarget(m_pPlayer);

	UISetting();
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
	if (m_pUI)
		m_pUI->Update();

	if (m_pPlayer)
		m_pPlayer->Update();


	if (m_pBoss)
		m_pBoss->Update();

	if (m_pCamera)
	{
		m_pCamera->Update(m_pPlayer->GetPtPosition());
		//m_pCamera->Update();
	}

	SetLight();
	SetPointLight();

	if (m_pPlayer)
		m_pPlayer->SkillRender();
	
}

void cBossScene::Render()
{
	if (m_pGrid)
		m_pGrid->Render();

	if (m_pUI)
		m_pUI->Render();

	if (m_pCamera)
		m_pCamera->Render();
	
	if (m_pPlayer)
		m_pPlayer->Render();

	if (m_pBoss)
		m_pBoss->Render();
	//
	//
	if (m_pMap)
		m_pMap->Render();
	//
	//
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
	cRay r = cRay::RayAtWorldSpace(g_ptMouse.x, g_ptMouse.y);
	ST_RUN_EXTRAINFO MSG;
	D3DXVECTOR3 vPickPos;

	////플레이어 피킹
	if (g_pKeyManager->isOnceKeyDown(VK_LBUTTON))
	{
		//충돌처리 해야할 박스정보와 플레이어 스피드를 메시지에 포함한다.

		MSG.fSpeed = m_pPlayer->GetStat().fSpeed;

		//레이를 월드좌표로 변환한다.
		//cRay r = cRay::RayAtWorldSpace(g_ptMouse.x, g_ptMouse.y);

		//몬스터를 클릭할 경우
		
		if (r.IntersectShpere(m_pBoss->GetMesh()->GetBoundingSphere()) && !m_pBoss->GetStat().bIsDead)
		{
			if (m_pBoss->GetStat().fHp <= 0) return;
			//공격할 타겟을 메세지에 담는다.
			MSG.nTarget = m_pBoss->GetID();

			//몬스터까지의 방향을 구한다.
			D3DXVECTOR3 vDir = m_pBoss->GetPosition() - m_pPlayer->GetPosition();

			//몬스터까지의 거리를 구한다.
			float fDisToMonster = D3DXVec3Length(&vDir);

			//몬스터방향으로의 캐릭터 사거리를 구한다.
			D3DXVec3Normalize(&vDir, &vDir);
			float fRange = D3DXVec3Length(&(vDir * m_pPlayer->GetStat().fAttackRange));

			//공격사거리보다 멀리 있는 경우
			if (fRange < fDisToMonster)
			{
				MSG.vDest = m_pBoss->GetPosition() - vDir * m_pPlayer->GetStat().fAttackRange;

			}
			//공격사거리 안쪽에 있는 경우.
			else
			{
				//SOUNDMANAGER->play("SwordSwing", 0.8f);
				MSG.vDest = m_pPlayer->GetPosition();
			}
			g_pMessageManager->MessageSend(0.0f, m_pPlayer->GetID(), m_pPlayer->GetID(), MESSAGE_TYPE::MSG_RUN, &MSG);

			//몬스터를 클릭한 경우 바닥과의 피킹처리는 하지 않는다.
			return;
		}
		
		//바닥과의 피킹처리

		for (size_t i = 0; i < m_vecTiles.size(); i += 3)
		{
			if (r.IntersectTri(m_vecTiles[i].p,
				m_vecTiles[i + 1].p,
				m_vecTiles[i + 2].p,
				vPickPos))
			{
				vPickPos.y = 3.4f;

				MSG.nTarget = m_pBoss->GetID();
				MSG.vDest = vPickPos;

				g_pMessageManager->MessageSend(0.0f, m_pPlayer->GetID(), m_pPlayer->GetID(), MESSAGE_TYPE::MSG_RUN, &MSG);
			}
		}
	}
	if (g_pKeyManager->isStayKeyDown(VK_RBUTTON))
	{

		MSG.fSpeed = m_pPlayer->GetStat().fSpeed;

		for (size_t i = 0; i < m_vecTiles.size(); i += 3)
		{
			if (r.IntersectTri(m_vecTiles[i].p,
				m_vecTiles[i + 1].p,
				m_vecTiles[i + 2].p,
				vPickPos))
			{
				vPickPos.y = 3.4f;

				MSG.nTarget = m_pPlayer->GetID();
				MSG.vDest = vPickPos;

				g_pMessageManager->MessageSend(0.0f, m_pPlayer->GetID(), m_pPlayer->GetID(), MESSAGE_TYPE::MSG_WHIRLWIND, &MSG);
			}
		}
	}

	//if (g_pKeyManager->isOnceKeyDown(VK_RBUTTON))
	//{
	//
	//	cRay r = cRay::RayAtWorldSpace(g_ptMouse.x, g_ptMouse.y);
	//	D3DXVECTOR3 vPickPos;
	//	for (size_t i = 0; i < m_vecTiles.size(); i += 3)
	//	{
	//		if (r.IntersectTri(m_vecTiles[i].p,
	//			m_vecTiles[i + 1].p,
	//			m_vecTiles[i + 2].p,
	//			vPickPos))
	//		{
	//			ST_RUN_EXTRAINFO msg;
	//			msg.fSpeed = m_pPlayer->GetStat().fSpeed;
	//
	//			msg.nTarget = m_pPlayer->GetID();
	//			msg.vDest = vPickPos;
	//			g_pMessageManager->MessageSend(0.0f, m_pPlayer->GetID(), m_pPlayer->GetID(), MESSAGE_TYPE::MSG_RUN, &msg);
	//			{
	//				//cActionMove* pAction = new cActionMove;
	//				//
	//				//pAction->SetTo(pickPos);
	//				//pAction->SetFrom(m_pPlayer->GetPosition());
	//				//pAction->SetTarget(m_pPlayer);
	//				//pAction->SetDelegate(m_pPlayer);
	//				//pAction->SetSpeed(0.05f);
	//				//pAction->Start();
	//				//m_pPlayer->SetAction(pAction);
	//				//m_pPlayer->GetMesh()->SetAnimationIndex("run");
	//				ST_RUN_EXTRAINFO MSG;
	//				//MSG.nBoxCount = m_vecBoundBox.size();
	//				//MSG.vecBox = m_vecBoundBox;
	//				MSG.fSpeed = m_pPlayer->GetStat().fSpeed;
	//
	//				MSG.nTarget = m_pPlayer->GetID();
	//				MSG.vDest = vPickPos;
	//
	//				g_pMessageManager->MessageSend(0.0f, m_pPlayer->GetID(), m_pPlayer->GetID(), MESSAGE_TYPE::MSG_RUN, &MSG);
	//
	//			}
	//		}
	//	}
	//}
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
	stLight.Diffuse = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	stLight.Ambient = stLight.Specular = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	stLight.Type = D3DLIGHT_DIRECTIONAL;
	D3DXVECTOR3 vDir(1, -1, 0);
	D3DXVec3Normalize(&vDir, &vDir);
	stLight.Direction = vDir;
	g_pD3DDevice->SetLight(0, &stLight);
	g_pD3DDevice->LightEnable(0, true);
	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);

}


void cBossScene::SetPointLight()
{
	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 0.6f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 0.6;
	mtrl.Diffuse.b = mtrl.Ambient.b = 0.6f;
	g_pD3DDevice->SetMaterial(&mtrl);

	D3DLIGHT9 st_pLight;
	ZeroMemory(&st_pLight, sizeof(D3DLIGHT9));
	st_pLight.Diffuse = D3DXCOLOR(0.05, 0.05, 0.05, 1.0f);
	st_pLight.Type = D3DLIGHT_POINT;
	PointLightWorldTM = m_pPlayer->GetMesh()->AttachItem("Spine2");
	D3DXVECTOR3 vPosition = D3DXVECTOR3(0, 0, 0);
	vPosition.x = PointLightWorldTM->_41;
	vPosition.y = PointLightWorldTM->_42;
	vPosition.z = PointLightWorldTM->_43;

	st_pLight.Position = vPosition;
	st_pLight.Range = 2.0f;
	st_pLight.Attenuation0 = 0.1;
	st_pLight.Attenuation1 = 0.001;
	st_pLight.Attenuation1 = 0.0001;
	g_pD3DDevice->SetLight(1, &st_pLight);
	g_pD3DDevice->LightEnable(1, true);
	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetRenderState(D3DRS_AMBIENT, 0x00202020);

	//---------------------------------------------
	D3DMATERIAL9 mtrl2;
	ZeroMemory(&mtrl2, sizeof(D3DMATERIAL9));
	mtrl2.Diffuse.r = mtrl2.Ambient.r = 0.6f;
	mtrl2.Diffuse.g = mtrl2.Ambient.g = 0.2;
	mtrl2.Diffuse.b = mtrl2.Ambient.b = 0.2f;
	g_pD3DDevice->SetMaterial(&mtrl2);

	D3DLIGHT9 st_pLigh2t;
	ZeroMemory(&st_pLigh2t, sizeof(D3DLIGHT9));
	st_pLigh2t.Diffuse = D3DXCOLOR(0.4, 0.02, 0.03, 1.0f);
	st_pLigh2t.Type = D3DLIGHT_POINT;

	PointLightWorldTM = m_pBoss->GetMesh()->AttachItem("Spine2");
	
	vPosition = D3DXVECTOR3(0, 0, 0);

	vPosition.x = PointLightWorldTM->_41;
	vPosition.y = PointLightWorldTM->_42;
	vPosition.z = PointLightWorldTM->_43;

	st_pLigh2t.Position = vPosition;
	st_pLigh2t.Range = 2.0f;
	st_pLigh2t.Attenuation0 = 0.1;
	st_pLigh2t.Attenuation1 = 0.001;
	st_pLigh2t.Attenuation1 = 0.0001;
	g_pD3DDevice->SetLight(2, &st_pLigh2t);
	g_pD3DDevice->LightEnable(2, true);
	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetRenderState(D3DRS_AMBIENT, 0x00202020);
}

void cBossScene::UISetting()
{
	m_pUI = new cUiManager;
	m_pUI->SetAddressLink(m_pPlayer);
	m_pUI->SetUp();
}



