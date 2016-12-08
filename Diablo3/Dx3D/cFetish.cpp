#include "stdafx.h"
#include "cFetish.h"


cFetish::cFetish()
{
}


cFetish::~cFetish()
{
}

void cFetish::Setup(D3DXVECTOR3 * vLookAt)
{	 
	m_stStat = ST_MONSTER_STAT(100, 0, 10, 5, 0.05f, 2.0f, 3.0f);

	cMonster::Setup("Fetish.x");
}

void cFetish::Update()
{
}

void cFetish::Render()
{
	cMonster::Render();
}



void cFetish::OnActionFinish(cAction * pSender)
{
}
