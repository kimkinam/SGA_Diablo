#include "stdafx.h"
#include "cBoss.h"
#include "cActionTrace.h"
#include "cLighting.h"
#include "cShaderManager.h"
#include "cPlayer.h"

cBoss::cBoss()
	: m_fLightingTimer(0.0f)
	, m_fLightingLifeTime(2.0f)
	, m_fLightingDecrease(0.1f)
	, FireBomb(NULL)
	, Red(0)
	, Yellow(0)
	, Alpha(0)



{	
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
}

void cBoss::Setup(D3DXVECTOR3* vLookAt)
{
	
	m_stStat = ST_MONSTER_STAT(100, 0, 10, 5, 0.05f, 2.0f, 10.0f, CHARACTER_DIABLO);


	cMonster::Setup("diablo.x", vLookAt);
	FireBomb = new cShaderManager;
	FireBomb->Setup("FireField.fx","Cube.x","불판.png",NULL,NULL);
	m_player = new cPlayer;
	m_player->Setup();
}
void cBoss::Update()
{
	cMonster::Update();

	m_player->Update();
	LightingBreathUpdate();


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
	
}
void cBoss::Render()
{
	cMonster::Render();

	LightingBreathRender();

	FireBomb->Shader_info_Set(Red,Yellow,Alpha);
	
	
	FireBomb->Render();
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
