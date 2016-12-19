#include "stdafx.h"
#include "cGargantuan.h"


cGargantuan::cGargantuan()
{
}


cGargantuan::~cGargantuan()
{
}

void cGargantuan::Setup(D3DXVECTOR3* vLookAt)
{
	m_stStat = ST_MONSTER_STAT(250, 0, 10, 5, 0.05f, 1.0f, 4.0f, CHARACTER_GARHANTUAN);

	cMonster::Setup("Gargantuan.x");

	m_pOBB = new cOBB;
	D3DXVECTOR3 vMin, vMax;

	vMin = m_pMesh->GetMin();
	vMax = m_pMesh->GetMax();

	vMin.x += 0.0f;
	vMax.x -= 0.0f;
	vMin.y -= 0.0f;
	vMax.y -= 0.8f;
	vMax.z += 0.0f;


	m_pOBB->Setup(vMin, vMax);
}

void cGargantuan::Setup(ST_SAVEOBJECT wObj)
{
	cMonster::Setup(wObj);

	m_pOBB = new cOBB;
	D3DXVECTOR3 vMin, vMax;

	vMin = m_pMesh->GetMin();
	vMax = m_pMesh->GetMax();

	vMin.x += 1.2f;
	vMax.x -= 1.2f;
	vMin.y -= 0.4f;
	vMax.y -= 0.8f;
	vMax.z -= 0.2f;
	vMin.z += 0.1f;


	m_pOBB->Setup(vMin, vMax);
}

void cGargantuan::Update()
{
	cMonster::Update();
}

void cGargantuan::Render()
{
	cMonster::Render();
}

void cGargantuan::OnActionFinish(cAction* pSender)
{

}
