#pragma once
#include "cMonster.h"

class cLighting;
class cShaderManager;
class cPlayer;

class cBoss :
	public cMonster
{
private:
	std::vector<cLighting*> m_vecLights;

	float		m_fLightingTimer;

	float		m_fLightingLifeTime;
	float		m_fLightingDecrease;



	cPlayer*				m_player;

	cShaderManager*		FireBomb;
	D3DXVECTOR3			cPlayerPosition;
	float		Red;
	float		Yellow;
	float		Alpha;


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

