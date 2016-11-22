#include "StdAfx.h"
#include "cPyramid.h"


cPyramid::cPyramid(void)
	: m_pVB(NULL)
{
}


cPyramid::~cPyramid(void)
{
	SAFE_RELEASE(m_pVB);
}

void cPyramid::Setup(D3DXCOLOR c, D3DXMATRIXA16* pmat /*= NULL*/)
{
	ST_PN_VERTEX v;
	
	v.p = D3DXVECTOR3( 0,  0,  0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3( 1, -1, -1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1, -1, -1); m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3( 0,  0,  0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3( 1, -1,  1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3( 1, -1, -1); m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3( 0,  0,  0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1, -1,  1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3( 1, -1,  1); m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3( 0,  0,  0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1, -1, -1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1, -1,  1); m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3( 1, -1, -1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3( 1, -1,  1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1, -1, -1); m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3(-1, -1,  1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1, -1, -1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3( 1, -1,  1); m_vecVertex.push_back(v);


	if(pmat)
	{	
		for(size_t i = 0; i < m_vecVertex.size(); ++i)
		{
			D3DXVec3TransformCoord(&m_vecVertex[i].p,
				&m_vecVertex[i].p,
				pmat); 
		}
	}

	for(size_t i = 0; i < m_vecVertex.size(); i += 3)
	{
		D3DXVECTOR3 n;
		D3DXVECTOR3 v01 = m_vecVertex[i + 1].p - m_vecVertex[i].p;
		D3DXVECTOR3 v02 = m_vecVertex[i + 2].p - m_vecVertex[i].p;
		D3DXVec3Cross(&n, &v01, &v02); 
		D3DXVec3Normalize(&n, &n);
		m_vecVertex[i].n = m_vecVertex[i + 1].n = m_vecVertex[i + 2].n = n;
	}

	g_pD3DDevice->CreateVertexBuffer(m_vecVertex.size() * sizeof(ST_PN_VERTEX), 0, ST_PN_VERTEX::FVF, D3DPOOL_MANAGED, &m_pVB, NULL);

	void* pVertices;
	m_pVB->Lock(0, m_vecVertex.size() * sizeof(ST_PN_VERTEX), (void**)&pVertices, 0);

	memcpy(pVertices, &m_vecVertex[0], m_vecVertex.size() * sizeof(ST_PN_VERTEX));
	m_pVB->Unlock();


	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
	m_stMtl.Ambient = m_stMtl.Diffuse = m_stMtl.Specular = c;
}

void cPyramid::Render()
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	g_pD3DDevice->SetMaterial(&m_stMtl);
	g_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(ST_PN_VERTEX));
	g_pD3DDevice->SetFVF(ST_PN_VERTEX::FVF);
	for(int i = 0; i < 6; ++i)
		g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, i * 3, 1);
}
