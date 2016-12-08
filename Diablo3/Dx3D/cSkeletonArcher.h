#pragma once
#include "cMonster.h"
class cSkeletonArcher :
	public cMonster
{
public:
	cSkeletonArcher();
	virtual ~cSkeletonArcher();

	virtual void Setup();
	virtual void Update();
	virtual void Render();

	virtual void OnActionFinish(cAction* pSender) override;
};

