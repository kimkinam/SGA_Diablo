#pragma once
#include "cMonster.h"

class cLighting;
class cShaderManager;
class cPlayer;
class cParticleEmitter;

class cMonsterGlobalState;
class cMonsterTrace;

class cBoss :
	public cMonster
{

public:
	cStateMachine<cMonster>* m_pSateMachnie;

private:
	std::vector<cLighting*> m_vecLights;

	float		m_fLightingTimer;

	float		m_fLightingLifeTime;
	float		m_fLightingDecrease;

	cPlayer*				m_player;

	cShaderManager*		FireBomb;
	D3DXVECTOR3			cPlayerPosition;
	float				Red;
	float				Yellow;
	float				Alpha;


	cParticleEmitter*		m_pFireTail;
	D3DXMATRIXA16			m_matFire;

	D3DXVECTOR3				m_vFirePos;
	D3DXVECTOR3				m_vFireDir;


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
};

