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

	m_pOBB = new cOBB;
	D3DXVECTOR3 vMin, vMax;
	
	vMin = m_pMesh->GetMin();
	vMax = m_pMesh->GetMax();
	
	vMin.x += 0.2f;
	vMax.x -= 0.2f;
	vMax.y -= 0.2f;
	
	m_pOBB->Setup(vMin, vMax);
}

void cFetish::Setup(ST_SAVEOBJECT wObj)
{
	cMonster::Setup(wObj);

	m_pOBB = new cOBB;
	D3DXVECTOR3 vMin, vMax;

	vMin = m_pMesh->GetMin();
	vMax = m_pMesh->GetMax();

	vMin.x += 0.2f;
	vMax.x -= 0.2f;
	vMax.y -= 0.2f;

	m_pOBB->Setup(vMin, vMax);
}

void cFetish::Update()
{
	cMonster::Update();
}

void cFetish::Render()
{
	cMonster::Render();
}

