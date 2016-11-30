#pragma once
#include "cMonster.h"
class cBoss :
	public cMonster
{
public:
	cBoss();
	~cBoss();

	void Setup();
	void Update();
	void Render();
};

