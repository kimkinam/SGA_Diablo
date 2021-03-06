#include "stdafx.h"
#include "cSkeleton.h"
#include "cActionTrace.h"


cSkeleton::cSkeleton()
{
}


cSkeleton::~cSkeleton()
{
}

void cSkeleton::Setup()
{
	m_stStat = ST_MONSTER_STAT(150, 0, 3, 5, 0.07f, 1.0f, 4.0f, CHARACTER_SKELETON);

	cMonster::Setup("Skeleton.x");

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

void cSkeleton::Setup(ST_SAVEOBJECT wObj)
{
	cMonster::Setup(wObj);

	m_pOBB = new cOBB;
	D3DXVECTOR3 vMin, vMax;

	vMin = m_pMesh->GetMin();
	vMax = m_pMesh->GetMax();

	vMin.x += 0.7f;
	vMax.x -= 0.6f;
	vMin.y -= 0.4f;
	vMax.y -= 0.6f;
	vMax.z -= 0.1f;
	vMin.z += 0.8f;

	//vMax.y -= 0.6f;

	//m_stStat.fAttackRange = m_stStat.fTraceRange - 1.0f;
	m_pOBB->Setup(vMin, vMax);
}

void cSkeleton::Update()
{
	cMonster::Update();
}

void cSkeleton::Render()
{
	cMonster::Render();
}


void cSkeleton::OnActionFinish(cAction * pSender)
{
	cMonster::OnActionFinish(pSender);
}
