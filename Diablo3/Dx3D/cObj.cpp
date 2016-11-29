#include "stdafx.h"
#include "cObj.h"
#include "cMtlTex.h"
#include "cObjLoader.h"


cObj::cObj()
	: m_matWorld(NULL)
	, m_pMesh(NULL)
	, m_vPosition(0, 0, 0)
{
	m_sSumNailName = m_sObjName = "";

}


cObj::~cObj()
{
	SAFE_RELEASE(m_pMesh);

	for each(auto c in m_vecMtl)
	{
		SAFE_RELEASE(c);
	}

	for each(auto c in m_vecHiddenMtl)
	{
		SAFE_RELEASE(c);
	}

	for each(auto c in m_vecHiddenObj)
	{
		SAFE_RELEASE(c);
	}
}

void cObj::SetUp(char * szFileName, char* szFolderName)
{
	cObjLoader loader;
	m_sObjName = szFileName;
	//m_pMesh = loader.Load(szFileName, szFolderName, m_vecMtl, NULL);
	loader.Load(szFileName, szFolderName, NULL,
		m_vecMtl, m_pMesh, m_vecHiddenMtl, m_vecHiddenObj);

	D3DXVECTOR3 vMin, vMax;

	m_vecBound.resize(m_vecHiddenObj.size());
	for (size_t i = 0; i < m_vecBound.size(); ++i)
	{
		GetBoundMinMax(m_vecHiddenObj[i], vMin, vMax);

		MakeBoundBox(vMin, vMax, m_vecBound[i]);
	}
	
	m_vecHiddenDraw.resize(m_vecHiddenObj.size());
	for (size_t i = 0; i < m_vecHiddenObj.size(); ++i)
		m_vecHiddenDraw[i] = (false);

	if (m_vecHiddenObj.empty())
	{
		m_vecBound.resize(1);
		GetBoundMinMax(m_pMesh, vMin, vMax);
		MakeBoundBox(vMin, vMax, m_vecBound[0]);
		
	}
	
	
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
		//D3DXMatrixIdentity(&matR);
		*m_matWorld = matR * (*m_matWorld);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, m_matWorld);
	}
	else
	{
		D3DXMATRIX matS, matR, matT, matW;
		D3DXMatrixScaling(&matS, 0.5f, 0.5f, 0.5f);
		D3DXMatrixRotationY(&matR, -D3DXToRadian(90));
		D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
		matW = matS * matR * matT;
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matW);

	}


	
	D3DXMATRIX matW;
	D3DXMatrixIdentity(&matW);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matW);
	for (size_t i = 0; i < m_vecBound.size(); ++i)
	{
		for (size_t j = 0; j < m_vecBound[i].size(); ++j)
		{
			D3DMATERIAL9 mtl;
			ZeroMemory(&mtl, sizeof(D3DMATERIAL9));
			g_pD3DDevice->SetMaterial(&mtl);
			g_pD3DDevice->SetTexture(0, NULL);
			g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
			g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
				m_vecBound[i].size() / 3,
				&m_vecBound[i][0],
				sizeof(ST_PC_VERTEX));
		}
	//	g_pD3DDevice->SetMaterial(&m_stMtl);
	//	g_pD3DDevice->SetTransform(D3DTS_WORLD &m_matWorld);
	//	g_pD3DDevice->SetTexture(0, m_pTexture);
		
	}
		

	//if (m_pMesh)
	//{
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		for (size_t i = 0; i < m_vecMtl.size(); ++i)
		{
			g_pD3DDevice->SetMaterial(&m_vecMtl[i]->GetMtl());
			g_pD3DDevice->SetTexture(0, m_vecMtl[i]->GetTexture());
			m_pMesh->DrawSubset(i);
		}

		if (!m_vecHiddenObj.empty())
		{
			for (size_t j = 0; j < m_vecHiddenObj.size(); ++j)
			{
				if (m_vecHiddenDraw[j]) continue;
				for (size_t i = 0; i < m_vecHiddenMtl.size(); ++i)
				{
					g_pD3DDevice->SetMaterial(&m_vecHiddenMtl[i]->GetMtl());
					g_pD3DDevice->SetTexture(0, m_vecHiddenMtl[i]->GetTexture());

					m_vecHiddenObj[j]->DrawSubset(i);
				}

			}
		}
		
	//}

}

float cObj::GetRayDistance(int index, std::vector<ST_PC_VERTEX> vecVertex)
{

	return 0.0f;
}
