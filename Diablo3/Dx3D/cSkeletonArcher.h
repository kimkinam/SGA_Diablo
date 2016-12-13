#pragma once
#include "cMonster.h"
class cSkeletonArcher :
	public cMonster
{
public:
	cSkeletonArcher();
	virtual ~cSkeletonArcher();

	virtual void Setup(D3DXVECTOR3* vLookAt = NULL);
	virtual void Setup(ST_SAVEOBJECT wObj);
	virtual void Update();
	virtual void Render();

	virtual void OnActionFinish(cAction* pSender) override;
};

