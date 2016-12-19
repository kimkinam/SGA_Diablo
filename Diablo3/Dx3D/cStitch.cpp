#include "stdafx.h"
#include "cStitch.h"


cStitch::cStitch()
{
}


cStitch::~cStitch()
{
}

void cStitch::Setup(D3DXVECTOR3* vLookAt)
{
	m_stStat = ST_MONSTER_STAT(200, 0, 5, 5, 0.05f, 1.0f, 4.0f, CHARACTER_STITCH);
	
	cMonster::Setup("stitch.x");

	m_pOBB = new cOBB;
	D3DXVECTOR3 vMin, vMax;

	vMin = m_pMesh->GetMin();
	vMax = m_pMesh->GetMax();

	vMin.x += 0.2f;
	vMax.x -= 0.2f;
	vMin.y -= 1.0f;
	vMax.y -= 0.8f;
	vMax.z += 0.2f;


	m_pOBB->Setup(vMin, vMax);
}
void cStitch::Setup(ST_SAVEOBJECT wObj)
{
	cMonster::Setup(wObj);

	m_pOBB = new cOBB;
	D3DXVECTOR3 vMin, vMax;

	vMin = m_pMesh->GetMin();
	vMax = m_pMesh->GetMax();

	vMin.x += 0.4f;
	vMax.x -= 0.4f;

	vMin.y -= 0.5f;
	vMax.y -= 0.5f;

	vMax.z += 0.0f;
	vMin.z += 0.1f;

	//m_stStat.fMaxHp = m_stStat.fHp;

	m_pOBB->Setup(vMin, vMax);
}

void cStitch::Update()
{
	cMonster::Update();
}

void cStitch::Render()
{
	cMonster::Render();
}

void cStitch::OnActionFinish(cAction* pSender)
{

}