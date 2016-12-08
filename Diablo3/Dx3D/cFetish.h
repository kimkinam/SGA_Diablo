#pragma once
#include "cMonster.h"
class cFetish :
	public cMonster
{
public:
	cFetish();
	virtual ~cFetish();

	virtual void Setup(D3DXVECTOR3* vLookAt = NULL);
	virtual void Update();
	virtual void Render();

	virtual void OnActionFinish(cAction* pSender) override;
};

