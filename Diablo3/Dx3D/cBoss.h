#pragma once
#include "cMonster.h"

class cLighting;
class cShaderManager;
class cPlayer;
class cParticleEmitter;

class cMonsterGlobalState;
class cMonsterTrace;

struct ST_STOM
{
	cParticleEmitter*		m_pFireParticle;
	cParticleEmitter*		m_pFireTail;
	D3DXMATRIXA16			m_pMat;

	
	float					fLiveTimer;
};

class cBoss :
	public cMonster
{

private:
	std::vector<cLighting*> m_vecLights;

	float		m_fLightingTimer;

	float		m_fLightingLifeTime;
	float		m_fLightingDecrease;

	SYNTHESIZE_PASS_BY_REF(cPlayer*, m_pTargetPlayer, TargetPlayer);
	SYNTHESIZE(bool, m_bIsActive, IsActive);	//Stom;
	SYNTHESIZE(bool, m_bIsBreathActive, IsBreathActive);	//Stom;
	float		m_fBreathLifeTime;
private:
	cShaderManager*		FireBomb;
	D3DXVECTOR3			cPlayerPosition;
	float				Red;
	float				Yellow;
	float				Alpha;

	ST_STOM				m_aryStoms[8];
	cParticleEmitter*	m_pBreathParticle;
public:
	cBoss();
	virtual ~cBoss();

	virtual void Setup(D3DXVECTOR3* vLookAt = NULL);
	virtual void Update();
	virtual void Render();

	virtual void OnActionFinish(cAction* pSender) override;


public:
	void LightingBreathUpdate();
	void LightingBreathRender();

	void ParticleTestSetUp();
	void ParticleTestUpdate();
	void ParticleTestRender();


	void StomSkillStart(float liveTime);
	void BreathSkillStart(float liveTime);
	void SetState();
};

