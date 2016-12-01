#pragma once
#include "cMonster.h"
class cBoss :
	public cMonster
{
private:

public:
	cBoss();
	~cBoss();

	void Setup();
	void Update();
	void Render();
	virtual void OnActionFinish(cAction* pSender) override;
};

