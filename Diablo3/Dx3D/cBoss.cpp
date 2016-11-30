#include "stdafx.h"
#include "cBoss.h"


cBoss::cBoss()
{
}


cBoss::~cBoss()
{
	SAFE_RELEASE(Boss);
}

void cBoss::Setup()
{
	Boss = new cMonster;
	Boss->Setup("diablo");
}
void cBoss::Update()
{
	Boss->Update();
}
void cBoss::Render()
{
	Boss->Update();
}


