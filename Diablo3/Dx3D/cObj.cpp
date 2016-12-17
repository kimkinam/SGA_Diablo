#include "stdafx.h"
#include "cObj.h"
#include "cMtlTex.h"
#include "cObjLoader.h"
#include "cCamera.h"

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

	/*for each(auto c in m_vecHiddenMtl)
	{
		SAFE_RELEASE(c);
	}

	for each(auto c in m_vecHiddenObj)
	{
		SAFE_RELEASE(c);
	}*/


	SAFE_RELEASE(m_pEffect);
	SAFE_RELEASE(m_pEffect_Normal);
	SAFE_RELEASE(m_nTexture);

}

void cObj::SetUp(char * szFileName, char* szFolderName)
{
	cObjLoader loader;
	m_sObjName = szFileName;
	m_pMesh = loader.Load(szFileName, szFolderName, m_vecMtl, NULL);

	m_pEffect = LoadEffect("./Resources/Shaders/Haze.fx");
	m_pEffect_Normal = LoadEffect("./Resources/Shaders/map_normal.fx");
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
	D3DXCreateTextureFromFile(g_pD3DDevice, "./Resources/Shaders/map_normal.png", &m_nTexture);

	m_Camera = new cCamera;
	m_Camera->Setup();
	
}

void cObj::Update()
{
	//m_Camera->Update();
}

void cObj::Render()
{
	m_Camera->Render();
	ULONGLONG tick = GetTickCount64();

	g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
	//g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

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

<<<<<<< HEAD
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
=======

		D3DXVECTOR4 vLightPosition(0, 5000, 0, 1.0f);
		D3DXVECTOR4 vViewPosition(m_Camera->GetEye().x, m_Camera->GetEye().y, m_Camera->GetEye().z, 1.0f);
	

		for (size_t i = 0; i < m_vecMtl.size(); ++i)
>>>>>>> 4c3f7497ba3430f60885f40f70be1eabd4869b59
		{
			g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
			g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
			g_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			//g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

<<<<<<< HEAD
		}

		if (i == 8)
		{
			ULONGLONG tick = GetTickCount64();

			g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
			g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
			g_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			
			m_pEffect->SetFloat("fSpeed", 0.5f);
			m_pEffect->SetFloat("uvspeed", -1.0f);
			m_pEffect->SetFloat("fTime", g_pTimeManager->GetDeltaTime());
			m_pEffect->SetTexture("DiffuseMap_Tex", m_vecMtl[i]->GetTexture());

			UINT numPasses = 0;
			m_pEffect->Begin(&numPasses, NULL);

			for (UINT j = 0; j < numPasses; j++)
			{
				m_pEffect->BeginPass(j);
				m_pMesh->DrawSubset(i);
				m_pEffect->EndPass();

=======
			if (m_vecMtl[i]->GetIsHiddenObj())
			{
				//g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
				g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
				g_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
				//g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

			}	
			if(i == 8)
			{
				ULONGLONG tick = GetTickCount64();
				g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
				D3DXMATRIXA16 matView, matProj;
				g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
				g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);
			
				m_pEffect->SetMatrix("matWorld", &matW);
				m_pEffect->SetMatrix("matWorldViewProjection", &(matW * matView * matProj));
				m_pEffect->SetFloat("fSpeed", 0.5f);
				m_pEffect->SetFloat("uvspeed", -0.25f);
				m_pEffect->SetFloat("fTime", tick/1000.0f);
				m_pEffect->SetTexture("DiffuseMap_Tex", m_vecMtl[i]->GetTexture());
			
				UINT numPasses = 0;
				m_pEffect->Begin(&numPasses, NULL);
			
				for (UINT j = 0; j < numPasses; j++)
				{
					m_pEffect->BeginPass(j);
					m_pMesh->DrawSubset(i);
					m_pEffect->EndPass();
			
				}
				g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
				g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
				m_pEffect->End();

			}
			else
			{

				D3DXMATRIXA16 matView, matProj;
				g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
				g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);


				m_pEffect_Normal->SetVector("vViewPosition", &vViewPosition);
				m_pEffect_Normal->SetVector("vLightPosition", &vLightPosition);
				m_pEffect_Normal->SetMatrix("matWorld", &matW);
				m_pEffect_Normal->SetMatrix("matWorldViewProjection", &(matW * matView * matProj));
				m_pEffect_Normal->SetTexture("DiffuseMap_Tex", m_vecMtl[i]->GetTexture());
				m_pEffect_Normal->SetTexture("Normal_Tex", m_nTexture);

				//g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
				UINT numPasses = 0;
				m_pEffect_Normal->Begin(&numPasses, NULL);

				for (UINT j = 0; j < numPasses; j++)
				{
					m_pEffect_Normal->BeginPass(j);
					m_pMesh->DrawSubset(i);
					m_pEffect_Normal->EndPass();
				}
				//g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
				m_pEffect_Normal->End();
			}
				if (m_vecMtl[i]->GetIsHiddenObj())
				{
					g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
					g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
					g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
				}
>>>>>>> 4c3f7497ba3430f60885f40f70be1eabd4869b59
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



float cObj::GetRayDistance(int index, std::vector<ST_PC_VERTEX> vecVertex)
{

	return 0.0f;
}
