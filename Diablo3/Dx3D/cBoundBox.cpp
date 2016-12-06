#include "stdafx.h"
#include "cBoundBox.h"


cBoundBox::cBoundBox()
	: m_bIsDraw(true)
	, m_vPosition(0, 0, 0)
{
	D3DXMatrixIdentity(&m_matWorld);
}


cBoundBox::~cBoundBox()
{
}

void cBoundBox::Setup(D3DXVECTOR3 vMin, D3DXVECTOR3 vMax, D3DXVECTOR3 vPosition)
{
	MakeBoundBox(vMin, vMax, m_vecVertex);

	m_vPosition = vPosition;
	
	D3DXMATRIXA16 matT;
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	for (size_t i = 0; i < m_vecVertex.size(); ++i)
	{
		D3DXVec3TransformCoord(&m_vecVertex[i].p, &m_vecVertex[i].p, &matT);
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
	D3DXMatrixIdentity(&m_matWorld);
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
			&vRayPosition,
			&vDirection,
			&u, &v, vDirection))
		{
			return true;
		}
	}

	return false;
}
