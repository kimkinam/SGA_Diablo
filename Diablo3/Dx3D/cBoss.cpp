#include "stdafx.h"
#include "cBoss.h"


cBoss::cBoss()
{
}


cBoss::~cBoss()
{
	int a = 0;
}

void cBoss::Setup()
{
	cMonster::Setup("diablo");
}
void cBoss::Update()
{
	cMonster::Update();

}
void cBoss::Render()
{
	cMonster::Render();
}


