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

	SAFE_RELEASE(m_pEffect);

	/*for each(auto c in m_vecHiddenMtl)
	{
		SAFE_RELEASE(c);
	}

	for each(auto c in m_vecHiddenObj)
	{
		SAFE_RELEASE(c);
	}*/

}

void cObj::SetUp(char * szFileName, char* szFolderName)
{
	cObjLoader loader;
	m_sObjName = szFileName;
	m_pMesh = loader.Load(szFileName, szFolderName, m_vecMtl, NULL);

	m_pEffect = LoadEffect("./Resources/Shaders/heatHaze.fx");
	//loader.Load(szFileName, szFolderName, NULL,
	//	m_vecMtl, m_pMesh, m_vecHiddenMtl, m_vecHiddenObj);

	/*D3DXVECTOR3 vMin, vMax;

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
		
	}*/
	
	
}

void cObj::Render()
{
	ULONGLONG tick = GetTickCount64();
	DWORD prevLight, prevAlphaBlend, prevCullmode, prevFVF, prevTextureFactor, prevZBuffer;
	D3DXMATRIXA16 prevWorld;
	g_pD3DDevice->GetRenderState(D3DRS_LIGHTING, &prevLight);
	g_pD3DDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &prevAlphaBlend);
	g_pD3DDevice->GetRenderState(D3DRS_CULLMODE, &prevCullmode);
	g_pD3DDevice->GetFVF(&prevFVF);
	g_pD3DDevice->GetRenderState(D3DRS_TEXTUREFACTOR, &prevTextureFactor);
	g_pD3DDevice->GetRenderState(D3DRS_ZWRITEENABLE, &prevZBuffer);
	g_pD3DDevice->GetTransform(D3DTS_WORLD, &prevWorld);


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

		D3DXMATRIXA16 matView, matProj;
		g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
		g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);

			m_pEffect->SetMatrix("matWorld", &matW);
			m_pEffect->SetMatrix("matWorldViewProjection", &(matW * matView * matProj));

	}

	for (size_t i = 0; i < m_vecMtl.size(); ++i)
	{
		g_pD3DDevice->SetMaterial(&m_vecMtl[i]->GetMtl());
		g_pD3DDevice->SetTexture(0, m_vecMtl[i]->GetTexture());

		if (m_vecMtl[i]->GetIsHiddenObj())
		{
			g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
			g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
			g_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			//g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
		
		}
		
		//if (i == 8)	 continue;
		if(i == 8)
		{
			
			g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
			g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
			g_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		
			
			m_pEffect->SetFloat("fSpeed", 0.5f);
			m_pEffect->SetFloat("uvspeed", -0.15f);
			m_pEffect->SetFloat("fTime", tick / 1000.0f);
			m_pEffect->SetTexture("DiffuseMap_Tex", m_vecMtl[i]->GetTexture());
		
			UINT numPasses = 0;
			m_pEffect->Begin(&numPasses, NULL);
		
			for (UINT j = 0; j < numPasses; j++)
			{
				m_pEffect->BeginPass(j);
				m_pMesh->DrawSubset(i);
				m_pEffect->EndPass();
		
			}
			m_pEffect->End();
		}
		else
			m_pMesh->DrawSubset(i);

		if (m_vecMtl[i]->GetIsHiddenObj())
		{
			//g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
			g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
			g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
		}
	}

	g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, prevLight);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, prevAlphaBlend);
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, prevCullmode);
	g_pD3DDevice->SetFVF(prevFVF);
	g_pD3DDevice->SetRenderState(D3DRS_TEXTUREFACTOR, prevTextureFactor);
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, prevZBuffer);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &prevWorld);

}

float cObj::GetRayDistance(int index, std::vector<ST_PC_VERTEX> vecVertex)
{

	return 0.0f;
}
