#pragma once
#include "cMonster.h"

class cLighting;

class cBoss :
	public cMonster
{
private:
	std::vector<cLighting*> m_vecLights;

	float		m_fLightingTimer;

	float		m_fLightingLifeTime;
	float		m_fLightingDecrease;
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
};

