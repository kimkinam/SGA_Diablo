#include "stdafx.h"
#include "cZombieDog.h"


cZombieDog::cZombieDog()
{
}


cZombieDog::~cZombieDog()
{
}

void cZombieDog::Setup(D3DXVECTOR3* vLookAt)
{
	m_stStat = ST_MONSTER_STAT(100, 0, 10, 5, 0.05f, 1.0f, 4.0f, CHARACTER_ZOMBIEDOG);

	cMonster::Setup("ZombieDog.x");

	m_pOBB = new cOBB;
	D3DXVECTOR3 vMin, vMax;

	vMin = m_pMesh->GetMin();
	vMax = m_pMesh->GetMax();

	vMin.x += 0.6f;
	vMax.x -= 0.6f;
	vMin.y -= 0.45f;
	vMax.y -= 0.8f;
	vMax.z -= 0.2f;
	vMin.z -= 0.2f;


	m_pOBB->Setup(vMin, vMax);
}

void cZombieDog::Setup(ST_SAVEOBJECT wObj)
{
	cMonster::Setup(wObj);

	m_pOBB = new cOBB;
	D3DXVECTOR3 vMin, vMax;

	vMin = m_pMesh->GetMin();
	vMax = m_pMesh->GetMax();

	vMin.x += 0.5f;
	vMax.x -= 0.5f;
	vMin.y -= 0.45f;
	vMax.y -= 0.8f;
	vMax.z -= 0.3f;
	vMin.z -= 0.1f;
	//vMin.z -= 0.1f;


	m_pOBB->Setup(vMin, vMax);
}

void cZombieDog::Update()
{
	cMonster::Update();
}

void cZombieDog::Render()
{
	cMonster::Render();
}

void cZombieDog::OnActionFinish(cAction* pSender)
{

}