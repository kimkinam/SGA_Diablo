#include "stdafx.h"
#include "cObj.h"
#include "cMtlTex.h"
#include "cObjLoader.h"


cObj::cObj()
	: m_matWorld(NULL)
	, m_pMesh(NULL)
{
}


cObj::~cObj()
{
	SAFE_RELEASE(m_pMesh);

	for each(auto c in m_vecMtl)
	{
		SAFE_RELEASE(c);
	}
}

void cObj::SetUp(char * szFileName, char* szFolderName)
{
	cObjLoader loader;
	m_pMesh = loader.Load(szFileName, szFolderName, m_vecMtl, NULL);
}

void cObj::Render()
{
	g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (m_matWorld)
	{
		D3DXMATRIX matR;
		D3DXMatrixRotationX(&matR, D3DXToRadian(180));
		*m_matWorld = matR * (*m_matWorld);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, m_matWorld);
	}

		

	if (m_pMesh)
	{
		for (size_t i = 0; i < m_vecMtl.size(); ++i)
		{
			g_pD3DDevice->SetMaterial(&m_vecMtl[i]->GetMtl());
			g_pD3DDevice->SetTexture(0, m_vecMtl[i]->GetTexture());
			m_pMesh->DrawSubset(i);
		}
	}
}
