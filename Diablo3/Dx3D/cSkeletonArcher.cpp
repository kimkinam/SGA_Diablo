#include "stdafx.h"
#include "cSkeletonArcher.h"


cSkeletonArcher::cSkeletonArcher()
{
}


cSkeletonArcher::~cSkeletonArcher()
{
}

void cSkeletonArcher::Setup(D3DXVECTOR3 * vLookAt)
{
	m_stStat = ST_MONSTER_STAT(100, 0, 10, 5, 0.05f, 2.0f, 3.0f);

	cMonster::Setup("SkeletonArcher.x");

	m_pOBB = new cOBB;
	D3DXVECTOR3 vMin, vMax;

	vMin = m_pMesh->GetMin();
	vMax = m_pMesh->GetMax();

	vMin.x += 0.4f;
	vMax.x -= 0.4f;
	vMin.y -= 0.4f;
	vMax.y -= 0.6f;
	vMax.z -= 0.4f;


	m_pOBB->Setup(vMin, vMax);
}

void cSkeletonArcher::Setup(ST_SAVEOBJECT wObj)
{
	cMonster::Setup(wObj);

	m_pOBB = new cOBB;
	D3DXVECTOR3 vMin, vMax;

	vMin = m_pMesh->GetMin();
	vMax = m_pMesh->GetMax();

	vMin.x += 0.5f;
	vMax.x -= 0.5f;
	vMin.y -= 0.7f;
	vMax.y -= 0.7f;
	vMin.z += 0.2f;
	vMax.z -= 0.2f;

	//vMax.y -= 0.6f;

	m_stStat.fAttackRange = m_stStat.fTraceRange - 1.0f;
	m_pOBB->Setup(vMin, vMax);
}

void cSkeletonArcher::Update()
{
	cMonster::Update();
}

void cSkeletonArcher::Render()
{
	cMonster::Render();
}

void cSkeletonArcher::OnActionFinish(cAction * pSender)
{
}
