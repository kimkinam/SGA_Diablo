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
	, m_pTargetPlayer(NULL)
	, m_pBreathParticle(NULL)
	, m_bIsActive(false)
	, m_bIsBreathActive(false)

{	
	m_pSateMachnie->SetCurState(NULL);
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

	for (size_t i = 0; i < 8; ++i)
	{
		
		if(m_aryStoms[i].m_pFireParticle)
			m_aryStoms[i].m_pFireParticle->Release();
		SAFE_DELETE(m_aryStoms[i].m_pFireParticle);
		if(m_aryStoms[i].m_pFireTail)
			m_aryStoms[i].m_pFireTail->Release();
		SAFE_DELETE(m_aryStoms[i].m_pFireTail);
	}
	
	if (m_pBreathParticle)
		m_pBreathParticle->Release();
	SAFE_DELETE(m_pBreathParticle);
	SAFE_RELEASE(m_pTargetPlayer);
}

void cBoss::Setup(D3DXVECTOR3* vLookAt)
{
	
	m_stStat = ST_MONSTER_STAT(100, 0, 10, 5, 0.05f, 2.0f, 5.0f, CHARACTER_DIABLO);


	cMonster::Setup("diablo.x", vLookAt);
	FireBomb = new cShaderManager;
	FireBomb->Setup("FireField.fx","Cube.x","불판.png",NULL,NULL);

	m_pTarget = m_pTargetPlayer;
	ParticleTestSetUp();

	
}
void cBoss::Update()
{
	cMonster::Update();

	//m_player->Update();
	LightingBreathUpdate();

	ParticleTestUpdate();

	if (m_bIsBreathActive)
	{
		LightingBreathUpdate();
	
	}
		
}
void cBoss::Render()
{
	cMonster::Render();

	LightingBreathRender();

	//FireBomb->Shader_info_Set(Red,Yellow,Alpha);
	//
	//
	//FireBomb->Render();

	
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
	for (size_t i = 0; i < 8; ++i)
	{
		m_bIsActive = false;
		m_aryStoms[i].fLiveTimer = 9999;
		m_aryStoms[i].m_pFireParticle = new cParticleEmitter;
		m_aryStoms[i].m_pFireTail = new cParticleEmitter;
		//불꽃
		//컬러배열을 2 개이상 넣어줘야함

		VEC_COLOR colors;
		colors.push_back(D3DXCOLOR(0.3f, 0.3f, 0.1f, 1.0f));
		colors.push_back(D3DXCOLOR(0.3f, 0.3f, 0.1f, 1.0f));
		colors.push_back(D3DXCOLOR(0.4f, 0.1f, 0.1f, 0.9f));
		colors.push_back(D3DXCOLOR(0.4f, 0.1f, 0.1f, 0.5f));
		colors.push_back(D3DXCOLOR(1.0f, 0.0f, 0.1f, 1.0f));
		colors.push_back(D3DXCOLOR(1.0f, 0.0f, 0.1f, 1.0f));


		float ScaleAll = 4.0f;
		VEC_SCALE scales;
		scales.push_back(1.0f  * ScaleAll);
		scales.push_back(0.3f  * ScaleAll);
		scales.push_back(0.8f  * ScaleAll);
		scales.push_back(0.2f  * ScaleAll);
		scales.push_back(0.05f * ScaleAll);
		scales.push_back(0.01f * ScaleAll);


		LPDIRECT3DTEXTURE9 pTex = g_pTextureManager->GetTexture(
			"./Resources/Images/Trail/Whirlwind_spiral.dds");
		//m_aryStoms[i].m_pFireParticle->SetParent(&m_aryStoms[i].m_pMat);
		//파티클 랜더러 셋팅
		m_aryStoms[i].m_pFireParticle->Init(
			1000,            //최대 파티클 수
			10.0f,            //초당 파티클 발생 량
			3,               //하나의 파티클 입자 라이프 최소값
			5,               //하나의 파티클 입자 라이프 최대값
			D3DXVECTOR3(0, 0, 0),   //파티클 입자 속도 최소값 ( 로컬기준 )
			D3DXVECTOR3(0, 0, 0),   //파티클 입자 속도 최대값 ( 로컬기준 )
			D3DXVECTOR3(1, 1, 1),   //파티클 입자 가속 최소값 ( 로컬기준 )
			D3DXVECTOR3(-1, -1, -1), //파티클 입자 가속 최대값 ( 로컬기준 )
			colors,            //컬러 배열
			scales,            //스케일 배열
			0.1f,            //입자크기 최소값
			1.0f,            //최대값
			pTex,            //텍스쳐
			false);




		pTex = g_pTextureManager->GetTexture(
			"./Resources/Images/Trail/Whirlwind_spiral.dds");

		//m_pFireParticle->SetParent(&m_matFire);

		//불꽃 꼬리
		colors.clear();
		colors.push_back(D3DXCOLOR(1.1f, 0.3f, 0.0f, 1.0f));
		colors.push_back(D3DXCOLOR(1.2f, 0.3f, 0.0f, 1.0f));
		colors.push_back(D3DXCOLOR(1.3f, 0.2f, 0.0f, 0.9f));
		colors.push_back(D3DXCOLOR(1.7f, 0.1f, 0.0f, 0.5f));
		colors.push_back(D3DXCOLOR(2.3f, 0.0f, 0.0f, 1.0f));
		colors.push_back(D3DXCOLOR(3.5f, 0.0f, 0.0f, 1.0f));



		ScaleAll = 4.0f;
		scales.clear();
		scales.push_back(0.5f *  ScaleAll);
		scales.push_back(0.3f *  ScaleAll);
		scales.push_back(0.2f *  ScaleAll);
		scales.push_back(0.2f *  ScaleAll);
		scales.push_back(0.05f * ScaleAll);
		scales.push_back(0.01f * ScaleAll);

		//파티클 랜더러 셋팅
		m_aryStoms[i].m_pFireTail->Init(
			1000,				//최대 파티클 수
			10.0f,				//초당 파티클 발생 량
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


		D3DXVECTOR3 FireDirection = D3DXVECTOR3(cosf(D3DXToRadian(i*360.0f / 8.0f)), 0, sinf(D3DXToRadian(i*360.0f / 8.0f)));
		D3DXVec3Normalize(&FireDirection, &FireDirection);

		D3DXVECTOR3 vRight;
		D3DXVECTOR3 vUp;

		D3DXVec3Cross(&vRight, &D3DXVECTOR3(0, 1, 0), &FireDirection);
		D3DXVec3Normalize(&vRight, &vRight);

		D3DXVec3Cross(&vUp, &FireDirection, &vRight);
		D3DXVec3Normalize(&vUp, &vUp);

		D3DXMatrixIdentity(&m_aryStoms[i].m_pMat);

		memcpy(&m_aryStoms[i].m_pMat._11, &vRight, sizeof(D3DXVECTOR3));
		memcpy(&m_aryStoms[i].m_pMat._21, &vUp, sizeof(D3DXVECTOR3));
		memcpy(&m_aryStoms[i].m_pMat._31, &FireDirection, sizeof(D3DXVECTOR3));

	}

	//
	// 화염 방사기
	//
	m_pBreathParticle = new cParticleEmitter;
	VEC_COLOR colors;
	colors.push_back(D3DXCOLOR(1.0f,  1.0f, 0.0f, 0.0f));
	colors.push_back(D3DXCOLOR(0.8f,  0.5f, 0.0f, 0.5f));
	colors.push_back(D3DXCOLOR(0.6f,  0.3f, 0.1f, 0.7f));
	colors.push_back(D3DXCOLOR(0.12f, 0.08f, 0.2f, 0.7f));
	colors.push_back(D3DXCOLOR(0.05f, 0.05f, 0.3f, 0.8f));
	colors.push_back(D3DXCOLOR(0.05f, 0.05f, 0.3f, 0.8f));
	colors.push_back(D3DXCOLOR(0.05f, 0.05f, 0.3f, 0.5f));


	float  ScaleAll = 2.0f;
	VEC_SCALE scales;
	scales.push_back(0.2f*ScaleAll);
	scales.push_back(0.3f*ScaleAll);
	scales.push_back(0.4f*ScaleAll);
	scales.push_back(0.4f*ScaleAll);
	scales.push_back(0.5f*ScaleAll);
	scales.push_back(0.6f*ScaleAll);
	scales.push_back(0.7f*ScaleAll);

	LPDIRECT3DTEXTURE9 pTex = g_pTextureManager->GetTexture(
		"./Resources/Images/Particles/diablo_SE1_magicCracks.dds");
	
	//D3DXMATRIX* matP = m_pMesh->AttachItem("Jaw");
	//m_pBreathParticle->SetParent((D3DXMATRIXA16*)matP);
	m_pBreathParticle->Init(
		1000,
		10.0f,
		3,
		3.5f,
		D3DXVECTOR3(0, 0, 3),
		D3DXVECTOR3(0, 0, 3),
		D3DXVECTOR3(0, 0.03f*ScaleAll, 0.0f*ScaleAll),
		D3DXVECTOR3(0, -0.03f*ScaleAll, 1.0f*ScaleAll),
		colors,
		scales,
		2.3f,
		3.0f,
		pTex
	);

	

}

void cBoss::ParticleTestUpdate()
{
	for (int i = 0; i < 8; i++)
	{
		m_aryStoms[i].m_pFireParticle->BaseObjectUpdate(g_pTimeManager->GetDeltaTime());
		m_aryStoms[i].m_pFireTail->BaseObjectUpdate(g_pTimeManager->GetDeltaTime());


		if (m_bIsActive)
		{
			D3DXVECTOR3 matDir(m_aryStoms[i].m_pMat._31, m_aryStoms[i].m_pMat._32, m_aryStoms[i].m_pMat._33);
			D3DXVECTOR3 matPos(m_aryStoms[i].m_pMat._41, m_aryStoms[i].m_pMat._42, m_aryStoms[i].m_pMat._43);
			
			matPos += matDir*3.0f*g_pTimeManager->GetDeltaTime();
			
			m_aryStoms[i].m_pMat._41 = matPos.x; 
			m_aryStoms[i].m_pMat._42 = matPos.y; 
			m_aryStoms[i].m_pMat._43 = matPos.z;

			//m_aryStoms[i].m_pFireParticle->StartEmission();
			m_aryStoms[i].m_pFireParticle->Fire(160, matPos, 0.7f, 0.9f, 0.7f, 2.0f);
			m_aryStoms[i].m_pFireTail->FireTail(70, matPos, 0.5f, 0.8f, 0.5f, 2.0f);

			m_aryStoms[i].fLiveTimer -= g_pTimeManager->GetDeltaTime();
			if (m_aryStoms[i].fLiveTimer <= 0)
			{
				m_aryStoms[i].fLiveTimer = 9999;
				m_bIsActive = false;
				//m_aryStoms[i].m_pFireParticle->StopEmission();
			}
			
		}
		
	}

	if (m_pBreathParticle)
		m_pBreathParticle->BaseObjectUpdate(g_pTimeManager->GetDeltaTime());
	if (m_bIsBreathActive)
	{
		//m_pBreathParticle->StartEmission();
		D3DXMATRIXA16 matJaw = *((D3DXMATRIXA16*) (m_pMesh->AttachItem("Jaw") )) ;
		D3DXVECTOR3 pos(matJaw._41,matJaw._42,matJaw._43);
		
		m_pBreathParticle->LighteningBreath(50,pos, m_vDirection,0,0, 2.f, 4.f);//스피드안씀

		m_fBreathLifeTime -= g_pTimeManager->GetDeltaTime();

		if (m_fBreathLifeTime <= 0)
		{
			//m_pBreathParticle->StopEmission();
			m_bIsBreathActive = false;
		}
			
	}
		

}

void cBoss::ParticleTestRender()
{
	for (size_t i = 0; i < 8; ++i)
	{
		m_aryStoms[i].m_pFireParticle->BaseObjectRender();
		m_aryStoms[i].m_pFireTail->BaseObjectRender();
	}

	m_pBreathParticle->BaseObjectRender();


}

void cBoss::StomSkillStart(float liveTime)
{
	for(int i=0; i < 8; i++)
	{
		m_bIsActive = true;
		m_aryStoms[i].fLiveTimer = liveTime;
		D3DXMATRIX* mat = m_pMesh->AttachItem("left_ball");
		m_aryStoms[i].m_pMat._41 = mat->_41;
		m_aryStoms[i].m_pMat._42 = mat->_42 + 0.5f;
		m_aryStoms[i].m_pMat._43 = mat->_43;
	}
}

void cBoss::BreathSkillStart(float liveTime)
{
	m_bIsBreathActive = true;
	m_fBreathLifeTime = liveTime;
}

void cBoss::SetState()
{
	
	m_pSateMachnie->SetCurState(cMonsterDetecting::Instance());
}
