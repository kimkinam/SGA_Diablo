#include "stdafx.h"
#include "cGamingScene.h"
#include "cCamera.h"
#include "cGrid.h"
#include "cUiManager.h"
#include "cSkinnedMesh.h"
#include "cMap.h"
#include "cMonster.h"
#include "cPlayer.h"
#include "cActionMove.h"

#include "cFetish.h"	
#include "cSkeleton.h"
#include "cSkeletonArcher.h"
#include "cStitch.h"
#include "cZombieDog.h"
#include "cGargantuan.h"

#include "cUIImage.h"
#include "cShaderManager.h"

cGamingScene::cGamingScene()
	: m_pGrid(NULL)
	, m_pPlayer(NULL)
	, m_pUI(NULL)
	, m_pCurMonster(NULL)
	, m_Cloud(NULL)
	, PointLightWorldTM(NULL)
	, CloudTranselation(0,0,0)
	, CloudScaling(1,1,1)
{
	
}


cGamingScene::~cGamingScene()
{
	g_pAIManager->Destroy();

	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
	SAFE_RELEASE(m_pPlayer);
	SAFE_DELETE(m_pUI);
	SAFE_DELETE(m_Cloud);

	for each(auto c in m_vecMap)
	{
		SAFE_RELEASE(c);
	}

	for each(auto c in m_vecBoundBox)
	{
		SAFE_DELETE(c);
	}

	for each(auto c in m_vecMonster)
	{
		//c->Release();
		SAFE_RELEASE(c);
	}

}


HRESULT cGamingScene::SetUp()
{
	if (m_bIsLoad)
	{
		Reset();
		SetLight();

		return S_OK;
	}
	cSceneObject::SetUp();

	m_pGrid = new cGrid;
	m_pGrid->Setup(30);

	m_bIsLoad = true;


	m_pPlayer = new cPlayer;
	m_pPlayer->Setup(&D3DXVECTOR3(0,0,1));

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

	LoadMap("map1");

	m_pPlayer->SetBoundBox(m_vecBoundBox);
	for (size_t i = 0; i < m_vecMonster.size(); ++i)
	{
		m_vecMonster[i]->SetBoundBox(m_vecBoundBox);
	}
	g_pAIManager->RegisterAIBase(m_pPlayer);

	D3DXVECTOR3 vDir;
	vDir = m_pPlayer->GetPosition() - GAMINGSCENE_CAMERAPOS;/*D3DXVECTOR3(24, 10, -17)*/;
	D3DXVec3Normalize(&vDir, &vDir);
	
	float distance = 8.67f;
	
	m_pCamera->SetEye(m_pPlayer->GetPosition() - vDir * distance);
	m_pCamera->SetNewDirection(vDir);

	UISetting();

	enemyBarRight = m_pUI->GetpEnemyBar()->GetDrawRc().right;
	enemyBarLeft = m_pUI->GetpEnemyBar()->GetDrawRc().left;


	SOUNDMANAGER->play("GamingSceneBGM", 0.8f);
	//SOUNDMANAGER->play("WhirlWind", 0.8f);

	m_Cloud = new cShaderManager;
	

	SetLight();
	SetPointLight();

	m_Cloud->Setup("cloud.fx", "cloud.x", "cloud.dds", NULL, NULL);
	return S_OK;
}

void cGamingScene::Update()
{
	//assert(m_pPlayer->GetAction() && "wss");
	
	g_pMessageManager->MessageDelayedSend();

	for (m_vecMonsterIter = m_vecMonster.begin(); m_vecMonsterIter != m_vecMonster.end();)
	{
		//if ((*m_vecMonsterIter)->GetStat().fHp <= 0 && (*m_vecMonsterIter)->IsDoneCurAni())
		if((*m_vecMonsterIter)->GetRefCount() == 1)
		{
			(*m_vecMonsterIter)->Release();
			m_vecMonsterIter = m_vecMonster.erase(m_vecMonsterIter);
		}
		else
			++m_vecMonsterIter;
	}

	//if(g_pKeyManager->isOnceKeyDown('L'))
	//	LoadMap("map1");

	PlayerMoveTest();


	if (g_pKeyManager->isOnceKeyDown(VK_OEM_PERIOD))
	{
		m_pPlayer->GetMesh()->SetAnimationIndex("attack");
	}
	
	if (g_pKeyManager->isStayKeyDown('2'))
	{

		m_pPlayer->GetMesh()->SetAnimationIndex("whirlwinding");
	}



	if (g_pKeyManager->isOnceKeyDown('P'))
	{
		for (size_t i = 0; i < m_vecMonster.size(); ++i)
		{
			if (m_vecMonster[i]->GetStat().chType == CHARACTER_SKELETON)
				m_vecMonster[i]->GetStat().fHp = 0.0f;
		}
	}

	if (m_pPlayer)
	{
		m_pPlayer->Update();
		m_pPlayer->TrailUpdate();
		
		//PointLightWorldTM = m_pPlayer->GetMesh()->AttachItem("HP_chest");
	}
		
	for (size_t i = 0; i < m_vecMonster.size(); ++i)
	{
		m_vecMonster[i]->Update();
	}

	for (size_t i = 0; i < m_vecBoundBox.size(); ++i)
	{
		m_vecBoundBox[i]->Update(NULL);
	}

	if (m_pCamera)
	{
		m_pCamera->Update(m_pPlayer->GetPtPosition());
		//m_pCamera->Update(NULL);
	}
	
	if (m_pUI)
		m_pUI->Update();

	if (g_pKeyManager->isOnceKeyDown('3'))
	{
		g_pMessageManager->MessageSend(0.0f, m_pPlayer->GetID(), m_pPlayer->GetID(), MESSAGE_TYPE::MSG_WARCRY, NULL);
	}

	CloudScaling = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	CloudTranselation = D3DXVECTOR3(0.0f, 0.2f, 0.0f);
	m_Cloud->SetPosition_xyz(CloudTranselation);
	m_Cloud->SetScaling_xyz(CloudScaling);
	m_Cloud->Update();

	SetLight();
	SetPointLight();
}

void cGamingScene::Render()
{
	//g_pD3DDevice->LightEnable(0, true);

	if (m_pGrid)
		m_pGrid->Render();
	//
	if (m_pPlayer)
		m_pPlayer->Render();





	//
	for (size_t i = 0; i < m_vecMonster.size(); ++i)
	{
		m_vecMonster[i]->Render();
	}
	
	for (size_t i = 0; i < m_vecMap.size(); ++i)
	{
		m_vecMap[i]->Render();
	}
	
	
	m_Cloud->Shader_info_Set(1.5f, NULL, NULL, 0.005f); // 구름 
	m_Cloud->Render();

	for (size_t i = 0; i < m_vecBoundBox.size(); ++i)
	{
		m_vecBoundBox[i]->DebugRender(D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	}
	
	if (m_pCamera)
		m_pCamera->Render();
	//
	if (m_pPlayer)
		m_pPlayer->TrailRender();
	//
	if (m_pUI)
		m_pUI->Render();
	
	if (m_pCurMonster)
	{
		LPD3DXFONT font;
		font = g_pFontManger->GetFont(cFontManager::E_CHAT);
	
		LPD3DXFONT Damagefont;
		Damagefont = g_pFontManger->GetFont(cFontManager::E_NORMAL);
	
		char temp[128];
		RECT rc;
		sprintf_s(temp, "hp :%f", m_pCurMonster->GetStat().fHp, 128);
		
		SetRect(&rc, DEBUG_STARTX, DEBUG_STARTY + 100, DEBUG_STARTX + 550, DEBUG_STARTY + 315);
		Damagefont->DrawText(NULL,
			temp,
			128,
			&rc,
			DT_LEFT,
			D3DCOLOR_XRGB(255, 255, 255));
	
		string name = m_pCurMonster->GetObjName();
		int lastDotIndex = name.find_last_not_of(".");
		string result = name.substr(0, lastDotIndex - 1);
		sprintf_s(temp, "%s", result.c_str(), 128);

		RECT RC;
		GetClientRect(g_hWnd, &RC);

		
	
		if (m_pCurMonster->GetStat().chType == CHARACTER_GARHANTUAN)
			SetRect(&rc, DEBUG_STARTX + WINSIZE_X / 2.3f - result.length() - 30, DEBUG_STARTY - 22, DEBUG_STARTX + WINSIZE_X, DEBUG_STARTY + 315);
		else if (m_pCurMonster->GetStat().chType == CHARACTER_SKELETON || m_pCurMonster->GetStat().chType == CHARACTER_ZOMBIEDOG)
			SetRect(&rc, DEBUG_STARTX + WINSIZE_X / 2.3f - result.length() - 30, DEBUG_STARTY - 22, DEBUG_STARTX + WINSIZE_X, DEBUG_STARTY + 315);
		else
			SetRect(&rc, DEBUG_STARTX + WINSIZE_X / 2.3f + result.length() - 25, DEBUG_STARTY - 22, DEBUG_STARTX + WINSIZE_X, DEBUG_STARTY + 315);
	
		font->DrawText(NULL,
			temp,
			128,
			&rc,
			DT_LEFT ,
			D3DCOLOR_XRGB(255, 255, 255));
	}
	
	if (m_pPlayer)
		m_pPlayer->SkillRender();




}

void cGamingScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pCamera)
		m_pCamera->WndProc(hWnd, message, wParam, lParam);
}

void cGamingScene::LoadMap(string fileName)
{
	if (!m_vecMap.empty()) return;

	for (size_t i = 0; i < m_vecMap.size(); ++i)
	{
		SAFE_RELEASE(m_vecMap[i]);
	}

	string path = "./Resources/Object/";
	string exp = ".wobj";
	string fullName = path + fileName + exp;

	FILE* fp = NULL;

	fp = fopen(fullName.c_str(), "rb");

	assert(fp != NULL && "세이브 파일이 생성되지 않았습니다");

	//맵정보 로드
	int nObj;
	fread(&nObj, sizeof(int), 1, fp);

	for (int i = 0; i < nObj; ++i)
	{
		ST_SAVEOBJECT wObj;
		fread(&wObj, sizeof(ST_SAVEOBJECT), 1, fp);
		cMap* map = new cMap;

		map->Setup(wObj);
		m_vecMap.push_back(map);
	}

	//충돌박스 정보 로드
	int nBoxCount;
	fread(&nBoxCount, sizeof(int), 1, fp);
	for (size_t i = 0; i < nBoxCount; ++i)
	{
		D3DXVECTOR3 vMin;
		D3DXVECTOR3 vMax;

		fread(&vMin, sizeof(D3DXVECTOR3), 1, fp);
		fread(&vMax, sizeof(D3DXVECTOR3), 1, fp);
		cOBB* obb = new cOBB;
		obb->Setup(vMin, vMax);
		m_vecBoundBox.push_back(obb);
	}

	//몬스터정보 로드
	int nMonsterCount;
	fread(&nMonsterCount, sizeof(int), 1, fp);
	for (size_t i = 0; i < nMonsterCount; ++i)
	{
		ST_SAVEOBJECT wObj;
		fread(&wObj, sizeof(ST_SAVEOBJECT), 1, fp);

		ST_MONSTER_STAT st;
		fread(&st, sizeof(ST_MONSTER_STAT), 1, fp);

		string name = wObj.szfileName;
		cMonster* monster = NULL;

		if (name == "Fetish.x")
		{
			monster = new cFetish;
		}
		else if (name == "Skeleton.x")
		{
			monster = new cSkeleton;
		}
		else if (name == "SkeletonArcher.x")
		{
			monster = new cSkeletonArcher;
		}
		else if (name == "stitch.x")
		{
			monster = new cStitch;
		}
		else if (name == "ZombieDog.x")
		{
			monster = new cZombieDog;
		}
		else if (name == "Gargantuan.x")
		{
			monster = new cGargantuan;
		}
		
		assert(monster != NULL && "몬스터 x");

		monster->SetStat(st);
		monster->Setup(wObj);

		m_vecMonster.push_back(monster);
	}

	for (size_t i = 0; i < m_vecMonster.size(); ++i)
	{
		//m_vecMonster[i]->set
		m_vecMonster[i]->SetTarget(m_pPlayer);
		g_pAIManager->RegisterAIBase(m_vecMonster[i]);
	}

	fclose(fp);
}

void cGamingScene::UISetting()
{
	m_pUI = new cUiManager;
	m_pUI->SetAddressLink(m_pPlayer);
	m_pUI->SetUp();

	
}

void cGamingScene::PlayerMoveTest()
{
	cRay r = cRay::RayAtWorldSpace(g_ptMouse.x, g_ptMouse.y);
	
	D3DXVECTOR3 vPickPos;

	//몬스터를 클릭할 경우
	//마우스 오버
	for (size_t i = 0; i < m_vecMonster.size(); ++i)
	{
		if (r.IntersectShpere(m_vecMonster[i]->GetMesh()->GetBoundingSphere()))
		{
			if (m_vecMonster[i]->GetStat().fHp >= 0)
			{
				m_pUI->m_bIsEnemyBar = true;
				m_pCurMonster = m_vecMonster[i];
				//m_pUI->SetfEnemyHP(m_vecMonster[i]->GetStat().fHp);
				m_pUI->GetpEnemyBar()->GetDrawRc().right =
					(m_vecMonster[i]->GetStat().fHp / m_vecMonster[i]->GetStat().fMaxHp) * //ENEMY_HPBAR_MAXLENGTH;
					enemyBarRight - enemyBarLeft;
				break;
			}
			
		}
		else
		{
			m_pUI->m_bIsEnemyBar = false;
			m_pCurMonster = NULL;
		}
			
	}
	//플레이어 피킹
	if (g_pKeyManager->isOnceKeyDown(VK_LBUTTON))
	{
		//충돌처리 해야할 박스정보와 플레이어 스피드를 메시지에 포함한다.
		ST_RUN_EXTRAINFO MSG;
		ZeroMemory(&MSG, sizeof(ST_RUN_EXTRAINFO));
		MSG.nBoxCount = m_vecBoundBox.size();
		MSG.vecBox = m_vecBoundBox;
		MSG.fSpeed = m_pPlayer->GetStat().fSpeed;

		//레이를 월드좌표로 변환한다.
		//cRay r = cRay::RayAtWorldSpace(g_ptMouse.x, g_ptMouse.y);

		//몬스터를 클릭할 경우
		for (size_t i = 0; i < m_vecMonster.size(); ++i)
		{
			if (r.IntersectShpere(m_vecMonster[i]->GetMesh()->GetBoundingSphere()) && !m_vecMonster[i]->GetStat().bIsDead)
			{
				if (m_vecMonster[i]->GetStat().fHp <= 0) return;
				//공격할 타겟을 메세지에 담는다.
				MSG.nTarget = m_vecMonster[i]->GetID();

				//몬스터까지의 방향을 구한다.
				D3DXVECTOR3 vDir = m_vecMonster[i]->GetPosition() - m_pPlayer->GetPosition();

				//몬스터까지의 거리를 구한다.
				float fDisToMonster = D3DXVec3Length(&vDir);

				//몬스터방향으로의 캐릭터 사거리를 구한다.
				D3DXVec3Normalize(&vDir, &vDir);
				float fRange = D3DXVec3Length(&(vDir * m_pPlayer->GetStat().fAttackRange));
				
				//공격사거리보다 멀리 있는 경우
				if (fRange < fDisToMonster)
				{
					MSG.vDest = m_vecMonster[i]->GetPosition() - vDir * m_pPlayer->GetStat().fAttackRange;
					
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
		}
		//바닥과의 피킹처리
		
		for (size_t i = 0; i < m_vecTiles.size(); i += 3)
		{
			ST_RUN_EXTRAINFO MSG;
			ZeroMemory(&MSG, sizeof(ST_RUN_EXTRAINFO));

			if (r.IntersectTri(m_vecTiles[i].p,
				m_vecTiles[i + 1].p,
				m_vecTiles[i + 2].p,
				vPickPos) && !CollisionTest())
			{
				vPickPos.y = 0;
				
				MSG.nTarget = m_pPlayer->GetID();
				MSG.vDest = vPickPos;

				g_pMessageManager->MessageSend(0.0f, m_pPlayer->GetID(), m_pPlayer->GetID(), MESSAGE_TYPE::MSG_RUN, &MSG);
			}
		}
	}
	if (g_pKeyManager->isOnceKeyDown(VK_RBUTTON)) return;
	if (g_pKeyManager->isStayKeyDown(VK_RBUTTON))
	{
		ST_RUN_EXTRAINFO MSG;
		ZeroMemory(&MSG, sizeof(ST_RUN_EXTRAINFO));

		MSG.nBoxCount = m_vecBoundBox.size();
		MSG.vecBox = m_vecBoundBox;
		MSG.fSpeed = m_pPlayer->GetStat().fSpeed;

		for (size_t i = 0; i < m_vecTiles.size(); i += 3)
		{
			if (r.IntersectTri(m_vecTiles[i].p,
				m_vecTiles[i + 1].p,
				m_vecTiles[i + 2].p,
				vPickPos) && !CollisionTest())
			{
				vPickPos.y = 0;

				MSG.nTarget = m_pPlayer->GetID();
				MSG.vDest = vPickPos;

				g_pMessageManager->MessageSend(0.0f, m_pPlayer->GetID(), m_pPlayer->GetID(), MESSAGE_TYPE::MSG_WHIRLWIND, &MSG);
			}
		}
	}

	if (g_pKeyManager->isOnceKeyUp(VK_RBUTTON))
	{
		m_pPlayer->SetAction(NULL);
		m_pPlayer->m_pSateMachnie->ChangeState(cPlayerIdleState::Instance());
	}
	if (g_pKeyManager->isOnceKeyDown('2'))
	{
		//m_pPlayer->GetMesh()->SetAnimationIndex("whirlwinding");
		
	}

	//플레이어 아이템 변화 테스트

	//if (g_pKeyManager->isOnceKeyDown('1'))
	//{
	//	m_pPlayer->GetMesh()->ChangeItem("Barb_M_MED_Gloves", "./Resources/Player/Barb_M_MED_Norm_Base_A_diff.dds");
	//}
	//if (g_pKeyManager->isOnceKeyDown('2'))
	//{
	//	m_pPlayer->GetMesh()->ChangeItem("Barb_M_MED_Pants", "./Resources/Player/Barb_M_MED_Norm_Base_A_diff.dds");
	//}
	//if (g_pKeyManager->isOnceKeyDown('3'))
	//{
	//	m_pPlayer->GetMesh()->ChangeItem("Barb_M_MED_Cloth", "./Resources/Player/Barb_M_MED_Norm_Base_A_diff.dds");
	//}
	//if (g_pKeyManager->isOnceKeyDown('4'))
	//{
	//	m_pPlayer->GetMesh()->ChangeItem("Barb_M_MED_Armor", "./Resources/Player/Barb_M_MED_Norm_Base_A_diff.dds");
	//}
	//if (g_pKeyManager->isOnceKeyDown('5'))
	//{
	//	m_pPlayer->GetMesh()->ChangeItem("Barb_M_MED_Boots", "./Resources/Player/Barb_M_MED_Norm_Base_A_diff.dds");
	//}

	//사라지는 오브젝트 테스트
	if (!m_vecMap.empty() && m_pPlayer)
	{
		//현재 플레이어의 맵
		int nCurMap = m_pPlayer->GetCurMap();

		//플레이어를 바라보는 카메라 방향
		D3DXVECTOR3 dir = m_pPlayer->GetPosition() - m_pCamera->GetEye();

		//플레이어와 카메라의 거리
		float disToPlayer = D3DXVec3Length(&dir);

		//사라질 오브젝트와 카메라의 거리
		float disToHidden = 0.0f;

		//현재 맵에서 사라질수있는 오브젝트의 바운드박스를 검사
		for (size_t i = 0; i < m_vecMap[nCurMap]->GetBoundBox().size(); ++i)
		{
			//사라질 오브젝트의 바운드박스가 카메라에서의 ray에 걸렸으면 distance를 반환
			if (m_vecMap[nCurMap]->GetBoundBox()[i]->GetRayDistance(
				m_pCamera->GetEye(), dir, disToHidden))
			{
				//사라질 오브젝트가 플레이어보다 가깝게 있으면
				if (disToHidden < disToPlayer)
				{
					//그 오브젝트를 그리지 않는다.
					m_vecMap[nCurMap]->GetHiddenDraw()[i] = true;
					break;
				}
			}
			//사라질 오브젝트의 바운드 박스가 카메라에서의 ray에 걸리지 않았으면
			else
				//그 오브젝트는 그린다.
				m_vecMap[nCurMap]->GetHiddenDraw()[i] = false;
		}

		for (size_t i = 0; i < m_vecMap.size(); ++i)
		{
			for (size_t j = 0; j < m_vecMap[i]->GetBoundBox().size(); ++j)
			{
				if (i == nCurMap) continue;
				m_vecMap[i]->GetHiddenDraw()[j] = false;
			}
		}
	}
}

bool cGamingScene::CollisionTest()
{
	for (size_t i = 0; i < m_vecBoundBox.size(); ++i)
	{
		if (cOBB::IsCollision(m_pPlayer->GetOBB(), m_vecBoundBox[i]))
		{
			//m_pPlayer->SetIsMove(false);
			return true;
			break;
		}
		else
		{
			return false;
		}
	}
}

void cGamingScene::SetLight()
{
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

void cGamingScene::SetPointLight()
{
	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 0.6f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 0.6f;
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
}

