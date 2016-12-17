#include "stdafx.h"
#include "cBoss.h"
#include "cActionTrace.h"
#include "cLighting.h"
#include "cShaderManager.h"
#include "cPlayer.h"
#include "cMonsterDetecting.h"
#include "cGameObjectGlobalState.h"
#include "cMonsterTrace.h"

#include "cParticleEmitter.h"
#include "cParticle.h"


cBoss::cBoss()
	: m_fLightingTimer(0.0f)
	, m_fLightingLifeTime(2.0f)
	, m_fLightingDecrease(0.1f)
	, FireBomb(NULL)
	, Red(0)
	, Yellow(0)
	, Alpha(0)
	, m_pFireTail(NULL)
{	

	m_pSateMachnie = new cStateMachine<cMonster>(this);

	m_pSateMachnie->SetCurState(cMonsterDetecting::Instance());
	m_pSateMachnie->SetGlobalState(cGameObjectGlobalState::Instance());

	m_stStat = ST_MONSTER_STAT();

}	 


cBoss::~cBoss()
{
	for each(auto c in m_vecLights)
	{
		c->Release();

		SAFE_DELETE(c);
	}
	SAFE_DELETE(FireBomb);
	SAFE_RELEASE(m_player);

	if (m_pFireTail)
		m_pFireTail->Release();
	SAFE_DELETE(m_pFireTail);
}

void cBoss::Setup(D3DXVECTOR3* vLookAt)
{
	
	m_stStat = ST_MONSTER_STAT(100, 0, 10, 5, 0.05f, 2.0f, 10.0f, CHARACTER_DIABLO);


	cMonster::Setup("diablo.x", vLookAt);
	FireBomb = new cShaderManager;
	FireBomb->Setup("FireField.fx","Cube.x","불판.png",NULL,NULL);
	m_player = new cPlayer;
	m_player->Setup();

	ParticleTestSetUp();
}
void cBoss::Update()
{
	cMonster::Update();

	m_player->Update();
	//LightingBreathUpdate();


	// 쉐이더 불판 업데이트
	Red -= 0.03;
	if (Red < 0.5)
	{
		Red = 1.2;
	}

	Alpha -= 0.002;//A
	if (Alpha < 0)
	{
		Alpha = 1.0f;
	}

	Yellow -= 0.03;
	if (Yellow<0.5f)
	{
		Yellow = 0.8f;
	}
	//cPlayerPosition = m_player->GetPosition();
	//cPlayerPosition = D3DXVECTOR3(0, 5, -5);
	FireBomb->SetPosition_xyz(cPlayerPosition); // 불판 포지션 
	D3DXVECTOR3 FireBomgScaling(0.05, 0.001, 0.05);
	FireBomb->SetScaling_xyz(FireBomgScaling);

	FireBomb->Update();
	
	ParticleTestUpdate();

}
void cBoss::Render()
{
	cMonster::Render();

	LightingBreathRender();

	FireBomb->Shader_info_Set(Red,Yellow,Alpha);
	
	
	FireBomb->Render();

	ParticleTestRender();
}


void cBoss::OnActionFinish(cAction * pSender)
{
	cMonster::OnActionFinish(pSender);

}

void cBoss::LightingBreathUpdate()
{
	m_fLightingTimer += g_pTimeManager->GetDeltaTime();

	if (m_fLightingTimer >= 0.05f)
	{

		for (size_t i = 0; i < 3; ++i)
		{
			cLighting* light = new cLighting;
			int a = (rand() % 8000) / 1000 + 1;

			light->SetLightingStat(m_fLightingLifeTime, m_fLightingDecrease, 4, 0.15f, 4.0f);
			light->Setup("./Resources/Images/Particles/diablo_lightningForks.png",
				"./Resources/Shaders/Lighting.fx", a);

			D3DXVECTOR3 vPos = this->m_vPosition + m_vDirection * 1.5f + D3DXVECTOR3(0, 1, 0);
			D3DXVECTOR3 vDir = this->m_vDirection;
			D3DXVec3Normalize(&vDir, &vDir);
			
			light->SetPosition(vPos);
			
			D3DXVECTOR3 vScale(3, 1, 3);
			if (i == 1)
			{
				vDir = vDir + this->GetRight() * 0.1f;
				D3DXVec3Normalize(&vDir, &vDir);
				
			}
			else if (i == 2)
			{
				vDir = vDir + this->GetRight() * 0.1f;
				D3DXVec3Normalize(&vDir, &vDir);
			}
			light->SetLocalMatrix(vDir, vScale, 0);
			light->SetDirection(vDir);
			m_vecLights.push_back(light);
		}
		m_fLightingTimer = 0.0f;
	}

	for (size_t i = 0; i < m_vecLights.size();)
	{
		if (m_vecLights[i]->GetInfo().fLifeTime <= 0)
		{
			m_vecLights[i]->Release();
			SAFE_DELETE(m_vecLights[i]);
			m_vecLights.erase(m_vecLights.begin() + i);
			continue;
		}
		else
		{
			m_vecLights[i]->Update();
			++i;
		}

		
	}
}

void cBoss::LightingBreathRender()
{
	for (size_t i = 0; i < m_vecLights.size(); ++i)
	{
		m_vecLights[i]->Render();
	}
}

void cBoss::ParticleTestSetUp()
{

	VEC_COLOR colors;
	VEC_SCALE scales;

	LPDIRECT3DTEXTURE9 pTex = g_pTextureManager->GetTexture(
		"./Resources/Images/Trail/Whirlwind_spiral.dds");

	//m_pFireParticle->SetParent(&m_matFire);

	//불꽃 꼬리
	m_pFireTail = new cParticleEmitter;
	colors.clear();
	colors.push_back(D3DXCOLOR(1.1f, 0.3f, 0.0f, 1.0f));
	colors.push_back(D3DXCOLOR(1.2f, 0.3f, 0.0f, 1.0f));
	colors.push_back(D3DXCOLOR(1.3f, 0.2f, 0.0f, 0.9f));
	colors.push_back(D3DXCOLOR(1.7f, 0.1f, 0.0f, 0.5f));
	colors.push_back(D3DXCOLOR(2.3f, 0.0f, 0.0f, 1.0f));
	colors.push_back(D3DXCOLOR(3.5f, 0.0f, 0.0f, 1.0f));


	scales.clear();
	scales.push_back(0.5f * 2);
	scales.push_back(0.3f * 2);
	scales.push_back(0.2f * 2);
	scales.push_back(0.2f * 2);
	scales.push_back(0.05f * 2);
	scales.push_back(0.01f * 2);

	//LPDIRECT3DTEXTURE9 pTex = g_pTextureManager->GetTexture(
	//	"./Resources/Images/Trail/particle_0.png" );


	//pTex = g_pTextureManager->GetTexture(
	//	"./Resources/Images/Trail/FireExplosionBlurredGray.png");

	//파티클 랜더러 셋팅
	m_pFireTail->Init(
		10000,				//최대 파티클 수
		20.0f,				//초당 파티클 발생 량
		3,					//하나의 파티클 입자 라이프 최소값
		5,					//하나의 파티클 입자 라이프 최대값
		D3DXVECTOR3(0, 0, 0),	//파티클 입자 속도 최소값 ( 로컬기준 )
		D3DXVECTOR3(0, 0, 0),	//파티클 입자 속도 최대값 ( 로컬기준 )
		D3DXVECTOR3(1, 1, 1),	//파티클 입자 가속 최소값 ( 로컬기준 )
		D3DXVECTOR3(-1, -1, -1), //파티클 입자 가속 최대값 ( 로컬기준 )
		colors,				//컬러 배열
		scales,				//스케일 배열
		0.1f,				//입자크기 최소값
		1.0f,				//최대값
		pTex,				//텍스쳐
		false);

	D3DXMatrixIdentity(&m_matFire);
	m_vFirePos = D3DXVECTOR3(0, 0, 0);
	m_vFireDir = D3DXVECTOR3(1.0f, 0, 0);
}

void cBoss::ParticleTestUpdate()
{
	if (m_pFireTail)
		m_pFireTail->BaseObjectUpdate(g_pTimeManager->GetDeltaTime());

	if (g_pKeyManager->isOnceKeyDown(VK_RETURN))
	{
		m_vFirePos = D3DXVECTOR3(0, 0, 0);
	}
	if (g_pKeyManager->isStayKeyDown(VK_RETURN))
	{
		m_vFirePos += m_vFireDir*3.0f*g_pTimeManager->GetDeltaTime();
		m_pFireTail->FireTail(50, m_vFirePos, 0.5f, 0.8f, 0.5f, 2.0f);
	}

	//m_vFirePos += m_vFireDir*3.0f*g_pTimeManager->GetDeltaTime();
	//m_pFireTail->FireTail(50, m_vFirePos, 0.5f, 0.8f, 0.5f, 2.0f);

}

void cBoss::ParticleTestRender()
{
	if (m_pFireTail)
		m_pFireTail->BaseObjectRender();
}
