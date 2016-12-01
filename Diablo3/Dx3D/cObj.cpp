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


	int a = 0;

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
		//D3DXMatrixIdentity(&matR);
		D3DXMatrixRotationY(&matR, D3DXToRadian(90));
		D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
		matW = matS * matR * matT;
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matW);

	}

	for (size_t i = 0; i < m_vecMtl.size(); ++i)
	{
		g_pD3DDevice->SetMaterial(&m_vecMtl[i]->GetMtl());
		g_pD3DDevice->SetTexture(0, m_vecMtl[i]->GetTexture());
		m_pMesh->DrawSubset(i);
	}
	
	//D3DXMATRIX matW;
	//D3DXMatrixIdentity(&matW);
	//g_pD3DDevice->SetTransform(D3DTS_WORLD, &matW);
	
		

	//if (m_pMesh)
	//{
	

}

float cObj::GetRayDistance(int index, std::vector<ST_PC_VERTEX> vecVertex)
{

	return 0.0f;
}
