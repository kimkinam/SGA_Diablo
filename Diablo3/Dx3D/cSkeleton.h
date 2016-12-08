#pragma once
#include "cMonster.h"
class cSkeleton :
	public cMonster
{
private:

public:

	cSkeleton();
	virtual ~cSkeleton();

	virtual void Setup();
	virtual void Update();
	virtual void Render();

	virtual void OnActionFinish(cAction* pSender) override;
};

