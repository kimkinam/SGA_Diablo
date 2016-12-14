#include "stdafx.h"
#include "cSkeletonArcher.h"
#include "cArrow.h"


cSkeletonArcher::cSkeletonArcher()
	: m_bIsFire(false)
{
}


cSkeletonArcher::~cSkeletonArcher()
{
	for each (auto c in m_vecArrow)
	{
		SAFE_RELEASE(c);
	}
}

void cSkeletonArcher::Setup(D3DXVECTOR3 * vLookAt)
{
	m_stStat = ST_MONSTER_STAT(100, 0, 10, 5, 0.05f, 4.0f, 5.0f, CHARACTER_SKELETONARCHER);

	cMonster::Setup("SkeletonArcher.x");

	m_pOBB = new cOBB;
	D3DXVECTOR3 vMin, vMax;

	vMin = m_pMesh->GetMin();
	vMax = m_pMesh->GetMax();

	vMin.x += 0.6f;
	vMax.x -= 0.6f;

	vMin.y -= 2.0f;
	vMax.y -= 2.0f;

	vMin.z += 0.3f;
	vMax.z -= 0.3f;


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

	vMin.y -= 2.0f;
	vMax.y -= 2.0f;

	vMin.z += 0.2f;
	vMax.z -= 0.2f;


	m_stStat.fAttackRange = m_stStat.fTraceRange - 1.0f;
	m_pOBB->Setup(vMin, vMax);
}

void cSkeletonArcher::Update()
{
	cMonster::Update();

	for (size_t i = 0; i < m_vecArrow.size(); ++i)
	{
		m_vecArrow[i]->Update();
	}
}

void cSkeletonArcher::Render()
{
	cMonster::Render();

	for (size_t i = 0; i < m_vecArrow.size(); ++i)
	{
		m_vecArrow[i]->Render();
	}

	if (!m_vecArrow.empty())
	{
		LPD3DXFONT font;
		font = g_pFontManger->GetFont(cFontManager::E_NORMAL);

		char temp[128];
		sprintf_s(temp, "hp : %d", m_vecArrow.size()
			/* m_vecArrow[0]->GetPosition().x, m_vecArrow[0]->GetPosition().y, m_vecArrow[0]->GetPosition().z*/, 128);
		RECT rc;
		SetRect(&rc, DEBUG_STARTX, DEBUG_STARTY + 200, DEBUG_STARTX + 250, DEBUG_STARTY + 215);
		font->DrawText(NULL,
			temp,
			128,
			&rc,
			DT_LEFT,
			D3DCOLOR_XRGB(255, 255, 255));
	}
	
}

void cSkeletonArcher::OnActionFinish(cAction * pSender)
{
}
