#include "stdafx.h"
#include "cBoss.h"
#include "cActionTrace.h"
#include "cLighting.h"

cBoss::cBoss()
	: m_fLightingTimer(0.0f)
	, m_fLightingLifeTime(2.0f)
	, m_fLightingDecrease(0.1f)
{
}


cBoss::~cBoss()
{
	for each(auto c in m_vecLights)
	{
		c->Release();

		SAFE_DELETE(c);
	}
}

void cBoss::Setup(D3DXVECTOR3* vLookAt)
{
	
	m_stStat = ST_MONSTER_STAT(100, 0, 10, 5, 0.05f, 2.0f, 10.0f, CHARACTER_DIABLO);


	cMonster::Setup("diablo.x", vLookAt);

}
void cBoss::Update()
{
	cMonster::Update();

	LightingBreathUpdate();
	
}
void cBoss::Render()
{
	cMonster::Render();

	LightingBreathRender();
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
