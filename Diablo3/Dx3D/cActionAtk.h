#pragma once
#include "cAction.h"

class cGameObject;
class cPlayer;

class cActionAtk :
	public cAction
{
private:
	SYNTHESIZE(cGameObject*, m_pAttakTarget, AttackTarget);
	SYNTHESIZE(float, m_fAtkRange, AtkRange);
	SYNTHESIZE(float, m_fAtkTime, AtkTime);
	

public:
	cActionAtk();
	virtual~cActionAtk();

	static cAction* Create();

	virtual void Start() override;
	virtual void Update() override;
};

