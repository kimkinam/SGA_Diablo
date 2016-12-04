#pragma once
#include "cMonster.h"

class cBoss :
	public cMonster
{
private:

public:
	cBoss();
	virtual ~cBoss();

	virtual void Setup(D3DXVECTOR3* vLookAt = NULL);
	virtual void Update();
	virtual void Render();
	virtual void Trace();

	virtual void OnActionFinish(cAction* pSender) override;
};

