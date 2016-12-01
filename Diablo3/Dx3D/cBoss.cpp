#include "stdafx.h"
#include "cBoss.h"


cBoss::cBoss()
{
}


cBoss::~cBoss()
{
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


