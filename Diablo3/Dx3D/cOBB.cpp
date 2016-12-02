#include "StdAfx.h"
#include "cOBB.h"
#include "cSkinnedMesh.h"

cOBB::cOBB(void)
{
}


cOBB::~cOBB(void)
{
}

void cOBB::Setup(cSkinnedMesh* pSkinnedMesh)
{
	D3DXVECTOR3 vMin = pSkinnedMesh->GetMin();
	D3DXVECTOR3 vMax = pSkinnedMesh->GetMax();

	Setup(vMin, vMax);
}

void cOBB::Setup(D3DXVECTOR3 vMin, D3DXVECTOR3 vMax)
{
	m_vOrgCenterPos = (vMin + vMax) / 2.f;

	m_vOrgAxisDir[0] = D3DXVECTOR3(1, 0, 0);
	m_vOrgAxisDir[1] = D3DXVECTOR3(0, 1, 0);
	m_vOrgAxisDir[2] = D3DXVECTOR3(0, 0, 1);

	m_fAxisLen[0] = fabs(vMax.x - vMin.x);
	m_fAxisLen[1] = fabs(vMax.y - vMin.y);
	m_fAxisLen[2] = fabs(vMax.z - vMin.z);

	m_fAxisHalfLen[0] = m_fAxisLen[0] / 2.0f;
	m_fAxisHalfLen[1] = m_fAxisLen[1] / 2.0f;
	m_fAxisHalfLen[2] = m_fAxisLen[2] / 2.0f;

	D3DXMatrixIdentity(&m_matWorldTM);
	//D3DXMatrixTranslation(&m_matWorldTM,
	//	m_vOrgCenterPos.x,
	//	m_vOrgCenterPos.y,
	//	m_vOrgCenterPos.z);
}

void cOBB::Update(D3DXMATRIXA16* pmatWorld)
{
	if (pmatWorld)
		m_matWorldTM = *pmatWorld;

	for (int i = 0; i < 3; ++i)
	{
		D3DXVec3TransformNormal(
			&m_vAxisDir[i],
			&m_vOrgAxisDir[i],
			&m_matWorldTM);
	}

	D3DXVec3TransformCoord(
		&m_vCenterPos,
		&m_vOrgCenterPos,
		&m_matWorldTM);
}

void cOBB::DebugRender(D3DCOLOR c)
{
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorldTM);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pD3DDevice->SetTexture(0, NULL);
	std::vector<ST_PC_VERTEX> vecVertex;
	vecVertex.reserve(8);

	ST_PC_VERTEX v;
	v.c = c;
	v.p = D3DXVECTOR3(
		m_vOrgCenterPos.x - m_fAxisLen[0] / 2.0f,
		m_vOrgCenterPos.y - m_fAxisLen[1] / 2.0f,
		m_vOrgCenterPos.z - m_fAxisLen[2] / 2.0f);
	vecVertex.push_back(v);

	v.p = D3DXVECTOR3(
		m_vOrgCenterPos.x - m_fAxisLen[0] / 2.0f,
		m_vOrgCenterPos.y + m_fAxisLen[1] / 2.0f,
		m_vOrgCenterPos.z - m_fAxisLen[2] / 2.0f);
	vecVertex.push_back(v);

	v.p = D3DXVECTOR3(
		m_vOrgCenterPos.x + m_fAxisLen[0] / 2.0f,
		m_vOrgCenterPos.y + m_fAxisLen[1] / 2.0f,
		m_vOrgCenterPos.z - m_fAxisLen[2] / 2.0f);
	vecVertex.push_back(v);

	v.p = D3DXVECTOR3(
		m_vOrgCenterPos.x + m_fAxisLen[0] / 2.0f,
		m_vOrgCenterPos.y - m_fAxisLen[1] / 2.0f,
		m_vOrgCenterPos.z - m_fAxisLen[2] / 2.0f);
	vecVertex.push_back(v);

	v.p = D3DXVECTOR3(
		m_vOrgCenterPos.x - m_fAxisLen[0] / 2.0f,
		m_vOrgCenterPos.y - m_fAxisLen[1] / 2.0f,
		m_vOrgCenterPos.z + m_fAxisLen[2] / 2.0f);
	vecVertex.push_back(v);

	v.p = D3DXVECTOR3(
		m_vOrgCenterPos.x - m_fAxisLen[0] / 2.0f,
		m_vOrgCenterPos.y + m_fAxisLen[1] / 2.0f,
		m_vOrgCenterPos.z + m_fAxisLen[2] / 2.0f);
	vecVertex.push_back(v);

	v.p = D3DXVECTOR3(
		m_vOrgCenterPos.x + m_fAxisLen[0] / 2.0f,
		m_vOrgCenterPos.y + m_fAxisLen[1] / 2.0f,
		m_vOrgCenterPos.z + m_fAxisLen[2] / 2.0f);
	vecVertex.push_back(v);

	v.p = D3DXVECTOR3(
		m_vOrgCenterPos.x + m_fAxisLen[0] / 2.0f,
		m_vOrgCenterPos.y - m_fAxisLen[1] / 2.0f,
		m_vOrgCenterPos.z + m_fAxisLen[2] / 2.0f);
	vecVertex.push_back(v);


	std::vector<WORD> vecIndex;
	vecIndex.reserve(24);

	vecIndex.push_back(0); vecIndex.push_back(1);
	vecIndex.push_back(1); vecIndex.push_back(2);
	vecIndex.push_back(2); vecIndex.push_back(3);
	vecIndex.push_back(3); vecIndex.push_back(0);
	vecIndex.push_back(4); vecIndex.push_back(5);
	vecIndex.push_back(5); vecIndex.push_back(6);
	vecIndex.push_back(6); vecIndex.push_back(7);
	vecIndex.push_back(7); vecIndex.push_back(4);
	vecIndex.push_back(0); vecIndex.push_back(4);
	vecIndex.push_back(1); vecIndex.push_back(5);
	vecIndex.push_back(2); vecIndex.push_back(6);
	vecIndex.push_back(3); vecIndex.push_back(7);

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorldTM);
	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	g_pD3DDevice->DrawIndexedPrimitiveUP(D3DPT_LINELIST,
		0,
		8,
		12,
		&vecIndex[0],
		D3DFMT_INDEX16,
		&vecVertex[0],
		sizeof(ST_PC_VERTEX));
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
}

bool cOBB::IsCollision(cOBB* pOBB1, cOBB* pOBB2)
{
	float cos[3][3];
	float absCos[3][3];
	float dist[3];
	float r0, r1, r;

	const float cutOff = 0.999999f;
	bool existsParallelPair = false;

	D3DXVECTOR3 D = pOBB2->m_vCenterPos - pOBB1->m_vCenterPos;

	// OBB_A 의 3 축을 기준으로 OBB_B 의 3축을 검사
	for (int a = 0; a < 3; a++)
	{
		for (int b = 0; b < 3; b++)
		{
			//OBB_A a 축과 OBB_B b 축의 cos 값
			cos[a][b] = D3DXVec3Dot(&pOBB1->m_vAxisDir[a], &pOBB2->m_vAxisDir[b]);

			//위에서 구한 cos 값의 절대값
			absCos[a][b] = abs(cos[a][b]);


			//한축이 직각이다 
			//이러한경우 AABB 형태가 된다.
			if (absCos[a][b] > cutOff)
				existsParallelPair = true;
		}


		//사각형 끼리의 중심 거리 벡터를 OBB_A 의 a 축으로 투영한 거리
		dist[a] = D3DXVec3Dot(&pOBB1->m_vAxisDir[a], &D);
		r = abs(dist[a]);

		//OBB_A 의 a 축의 길이
		r0 = pOBB1->m_fAxisHalfLen[a];

		//OBB_B 의 꼭지점이 OBB_A 의a 축으로 투영된 길이
		r1 = pOBB2->m_fAxisHalfLen[0] * absCos[a][0] +
			pOBB2->m_fAxisHalfLen[1] * absCos[a][1] +
			pOBB2->m_fAxisHalfLen[2] * absCos[a][2];

		if (r > r0 + r1)
			return false;
	}



	// OBB_B 의 3축을 기준으로 OBB_A 의 3축을 검사
	for (int b = 0; b < 3; b++)
	{
		r = abs(D3DXVec3Dot(&pOBB2->m_vAxisDir[b], &D));
		r0 = pOBB1->m_fAxisHalfLen[0] * absCos[0][b] +
			pOBB1->m_fAxisHalfLen[1] * absCos[1][b] +
			pOBB1->m_fAxisHalfLen[2] * absCos[2][b];

		r1 = pOBB2->m_fAxisHalfLen[b];

		if (r > r0 + r1)
			return false;
	}


	if (existsParallelPair) return true;




	/////////////////////////////////////////////////////////////////

	r = abs(dist[0] * cos[2][0] - dist[2] * cos[0][0]);
	r0 = pOBB1->m_fAxisHalfLen[0] * absCos[2][0] + pOBB1->m_fAxisHalfLen[2] * absCos[0][0];
	r1 = pOBB2->m_fAxisHalfLen[1] * absCos[1][2] + pOBB2->m_fAxisHalfLen[2] * absCos[1][1];
	if (r > r0 + r1)
		return false;

	r = abs(dist[0] * cos[2][1] - dist[2] * cos[0][1]);
	r0 = pOBB1->m_fAxisHalfLen[0] * absCos[2][1] + pOBB1->m_fAxisHalfLen[2] * absCos[0][1];
	r1 = pOBB2->m_fAxisHalfLen[0] * absCos[1][2] + pOBB2->m_fAxisHalfLen[2] * absCos[1][0];
	if (r > r0 + r1)
		return false;

	r = abs(dist[0] * cos[2][2] - dist[2] * cos[0][2]);
	r0 = pOBB1->m_fAxisHalfLen[0] * absCos[2][2] + pOBB1->m_fAxisHalfLen[2] * absCos[0][2];
	r1 = pOBB2->m_fAxisHalfLen[0] * absCos[1][1] + pOBB2->m_fAxisHalfLen[1] * absCos[1][0];
	if (r > r0 + r1)
		return false;


	/////////////////////////////////////////////////////////////////	 


	r = abs(dist[1] * cos[0][0] - dist[0] * cos[1][0]);
	r0 = pOBB1->m_fAxisHalfLen[0] * absCos[1][0] + pOBB1->m_fAxisHalfLen[1] * absCos[0][0];
	r1 = pOBB2->m_fAxisHalfLen[1] * absCos[2][2] + pOBB2->m_fAxisHalfLen[2] * absCos[2][1];
	if (r > r0 + r1)
		return false;

	r = abs(dist[1] * cos[0][1] - dist[0] * cos[1][1]);
	r0 = pOBB1->m_fAxisHalfLen[0] * absCos[1][1] + pOBB1->m_fAxisHalfLen[1] * absCos[0][1];
	r1 = pOBB2->m_fAxisHalfLen[0] * absCos[2][2] + pOBB2->m_fAxisHalfLen[2] * absCos[2][0];
	if (r > r0 + r1)
		return false;


	r = abs(dist[1] * cos[0][2] - dist[0] * cos[1][2]);
	r0 = pOBB1->m_fAxisHalfLen[0] * absCos[1][2] + pOBB1->m_fAxisHalfLen[1] * absCos[0][2];
	r1 = pOBB2->m_fAxisHalfLen[0] * absCos[2][1] + pOBB2->m_fAxisHalfLen[1] * absCos[2][0];
	if (r > r0 + r1)
		return false;


	/////////////////////////////////////////////////////////////////

	r = abs(dist[2] * cos[1][0] - dist[1] * cos[2][0]);
	r0 = pOBB1->m_fAxisHalfLen[1] * absCos[2][0] + pOBB1->m_fAxisHalfLen[2] * absCos[1][0];
	r1 = pOBB2->m_fAxisHalfLen[1] * absCos[0][2] + pOBB2->m_fAxisHalfLen[2] * absCos[0][1];
	if (r > r0 + r1)
		return false;



	r = abs(dist[2] * cos[1][1] - dist[1] * cos[2][1]);
	r0 = pOBB1->m_fAxisHalfLen[1] * absCos[2][1] + pOBB1->m_fAxisHalfLen[2] * absCos[1][1];
	r1 = pOBB2->m_fAxisHalfLen[0] * absCos[0][2] + pOBB2->m_fAxisHalfLen[2] * absCos[0][0];
	if (r > r0 + r1)
		return false;



	r = abs(dist[2] * cos[1][2] - dist[1] * cos[2][2]);
	r0 = pOBB1->m_fAxisHalfLen[1] * absCos[2][2] + pOBB1->m_fAxisHalfLen[2] * absCos[1][2];
	r1 = pOBB2->m_fAxisHalfLen[0] * absCos[0][1] + pOBB2->m_fAxisHalfLen[1] * absCos[0][0];
	if (r > r0 + r1)
		return false;




	return true;
}
