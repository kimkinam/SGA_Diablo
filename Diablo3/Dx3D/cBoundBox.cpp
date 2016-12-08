#include "stdafx.h"
#include "cBoundBox.h"


cBoundBox::cBoundBox()
	: m_bIsDraw(true)
	, m_vPosition(0, 0, 0)
	, m_vDirection(0, 0, 1)
	, m_vRight(1, 0, 0)
	, m_vUp(0, 1, 0)
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matLocal);
}


cBoundBox::~cBoundBox()
{
}

void cBoundBox::Setup(D3DXVECTOR3 vMin, D3DXVECTOR3 vMax, D3DXMATRIX* mat)
{
	MakeBoundBox(vMin, vMax, m_vecVertex);

	
	m_matLocal._11 = m_vRight.x;		m_matLocal._12 = m_vRight.y;		m_matLocal._13 = m_vRight.z;
	m_matLocal._21 = m_vUp.x;			m_matLocal._22 = m_vUp.y;			m_matLocal._23 = m_vUp.z;
	m_matLocal._31 = m_vDirection.x;	m_matLocal._32 = m_vDirection.y;	m_matLocal._33 = m_vDirection.z;
	m_matLocal._41 = 0;					m_matLocal._42 = 0;					m_matLocal._43 = 0;

	if(mat)
		m_matWorld = *mat;

	m_matWorld = m_matLocal * m_matWorld;

	for (size_t i = 0; i < m_vecVertex.size(); ++i)
	{
		D3DXVec3TransformCoord(&m_vecVertex[i].p, &m_vecVertex[i].p, &m_matWorld);
	}


}

void cBoundBox::Setup(D3DXVECTOR3 * vMin, D3DXVECTOR3 * vMax, D3DXMATRIX * pMat)
{
	MakeBoundBox(*vMin, *vMax, m_vecVertex);

	if (pMat)
		m_matWorld *= *pMat;

	for (size_t i = 0; i < m_vecVertex.size(); ++i)
	{
		D3DXVec3TransformCoord(&m_vecVertex[i].p, &m_vecVertex[i].p, &m_matWorld);
	}
}

void cBoundBox::Render()
{
	//
	////D3DXMatrixIdentity(&m_matWorld);
	//
	//m_matWorld = m_matLocal * m_matWorld;
	D3DXMatrixIdentity(&m_matWorld);
	//D3DXMatrixTranslation(&m_matWorld, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	DWORD dLightMode = 0;
	DWORD dFillMode = 0;
	DWORD dCullMode = 0;

	g_pD3DDevice->GetRenderState(D3DRS_LIGHTING, &dLightMode);
	g_pD3DDevice->GetRenderState(D3DRS_FILLMODE, &dFillMode);
	g_pD3DDevice->GetRenderState(D3DRS_CULLMODE, &dCullMode);

	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);

	for (size_t i = 0; i < m_vecVertex.size(); ++i)
	{
		g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
			m_vecVertex.size() / 3,
			&m_vecVertex[0],
			sizeof(ST_PC_VERTEX));
	}

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, dLightMode);
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, dFillMode);
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, dCullMode);
}

bool cBoundBox::GetRayDistance(D3DXVECTOR3 vRayPosition, D3DXVECTOR3 vDirection, float& distance)
{
	D3DXVec3Normalize(&vDirection, &vDirection);

	for (size_t i = 0; i < m_vecVertex.size(); i += 3)
	{
		float u, v;

		if (D3DXIntersectTri(&m_vecVertex[i].p,
			&m_vecVertex[i + 1].p,
			&m_vecVertex[i + 2].p,
			&(vRayPosition),
			&vDirection,
			&u, &v, vDirection))
		{
  			return true;
		}
	}

	return false;
}
