#include "stdafx.h"
#include "cMap.h"
#include "cMtlTex.h"
#include "cObjLoader.h"

cMap::cMap()
	: m_pMesh(NULL)
	, m_pComMesh(NULL)
	, m_vPosition(0, 0, 0)
	, m_pEffect(NULL)
	//, m_bIsDrawBound(false)
{
	m_sSumNailName = m_sObjName = "";

	D3DXMatrixIdentity(&m_matWorld);
}


cMap::~cMap()
{
	//통제 매쉬 해제
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pComMesh);

	//통짜 mtl 해제
	for each(auto c in m_vecMtl)
	{
		SAFE_RELEASE(c);
	}

	for each(auto c in m_vecComMtl)
	{
		SAFE_RELEASE(c);
	}

	//사라질 mtl 해제
	for each(auto c in m_vecHiddenMtl)
	{
		SAFE_RELEASE(c);
	}



	//사라질 매쉬 해제
	for each(auto c in m_vecHiddenObj)
	{
		SAFE_RELEASE(c);
	}

	for each(auto c in m_vecBoundBox)
	{
		SAFE_DELETE(c);
	}

	SAFE_RELEASE(m_pEffect);

}

void cMap::Setup(char * szFileName, char * szForlderName)
{
	m_sObjName = szFileName;

	cObjLoader loader;
	D3DXMATRIXA16 matS, matR, matW;
	D3DXMatrixScaling(&matS, 0.5f, 0.5f, 0.5f);
	D3DXMatrixRotationY(&matR, D3DXToRadian(90));
	matW = matS * matR;
	//loader.Load(szFileName, szForlderName, &matW,
	//	m_vecMtl, m_pMesh, m_vecHiddenMtl, m_vecHiddenObj);


	loader.Load(szFileName, szForlderName, &matW, 
		m_vecMtl, m_pMesh,
		m_vecHiddenMtl, m_vecHiddenObj);

	cObjLoader loader2;
	m_pComMesh = loader2.Load(szFileName, szForlderName, m_vecComMtl, &matW);

	m_vecHiddenDraw.resize(m_vecHiddenObj.size());
	for (size_t i = 0; i < m_vecHiddenObj.size(); ++i)
		m_vecHiddenDraw[i] = (false);

	m_pEffect = LoadEffect("shader_map.fx");
}

void cMap::Render()
{
	g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	D3DXMatrixTranslation(&m_matWorld, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);

	//g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	//D3DXMATRIXA16 matView, matProjection, matViewProjection;
	//g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	//g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProjection);
	//
	//matViewProjection = matView * matProjection;
	//
	//D3DXMATRIXA16 mView, mInvView;
	//g_pD3DDevice->GetTransform(D3DTS_VIEW, &mView);
	//D3DXMatrixInverse(&mInvView, 0, &mView);
	//D3DXVECTOR3 vEye = D3DXVECTOR3(0, 0, 0);
	//D3DXVec3TransformCoord(&vEye, &vEye, &mInvView);
	//
	//// 쉐이더 전역변수들을 설정
	//m_pEffect->SetMatrix("matWorld", &m_matWorld);
	//m_pEffect->SetMatrix("matViewProjection", &matViewProjection);
	//m_pEffect->SetVector("gLightColor", &D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	//m_pEffect->SetVector("vViewPosition", &D3DXVECTOR4(vEye, 1.0f));
	//m_pEffect->SetVector("vLightPosition", &D3DXVECTOR4(500.0f, 500.0f, 500.0f, 1.0f));
	//
	//UINT numPasses = 0;
	//m_pEffect->Begin(&numPasses, NULL);

	for (size_t i = 0; i < m_vecMtl.size(); ++i)
	{
		g_pD3DDevice->SetMaterial(&m_vecMtl[i]->GetMtl());
		g_pD3DDevice->SetTexture(0, m_vecMtl[i]->GetTexture());

		//for (UINT j = 0; j < numPasses; ++j)
		//{
			//m_pEffect->BeginPass(j);
			m_pMesh->DrawSubset(i);
			//m_pEffect->EndPass();
		//}
		
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

	m_pEffect->End();

	

	//RenderBoundBox();
}

void cMap::RenerComplete()
{
	g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	D3DXMatrixTranslation(&m_matWorld, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);

	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	for (size_t i = 0; i < m_vecComMtl.size(); ++i)
	{
		g_pD3DDevice->SetMaterial(&m_vecComMtl[i]->GetMtl());
		g_pD3DDevice->SetTexture(0, m_vecComMtl[i]->GetTexture());
		m_pComMesh->DrawSubset(i);
	}
}

void cMap::RenderBoundBox()
{
	for (size_t i = 0; i < m_vecBoundBox.size(); ++i)
	{
		if (m_vecBoundBox[i]->GetIsDraw())
			m_vecBoundBox[i]->Render();
	}
}

void cMap::SetLocalBoundBox()
{
	m_vecBoundBox.resize(m_vecHiddenObj.size());

	D3DXVECTOR3 vMin, vMax;
	D3DXMATRIX matT;
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	for (size_t i = 0; i < m_vecBoundBox.size(); ++i)
	{
		GetBoundMinMax(m_vecHiddenObj[i], vMin, vMax);
		m_vecBoundBox[i] = new cBoundBox;
		m_vecBoundBox[i]->Setup(vMin, vMax, &matT);
	}
}

void cMap::SetRefMtl(cMap* map)
{
	for (size_t i = 0; i < map->GetMtl().size(); ++i)
	{
		map->GetMtl()[i]->AddRef();
	}

	m_vecMtl = map->GetMtl();
	
	for (size_t i = 0; i < map->GetHiddenMtl().size(); ++i)
	{
		map->GetHiddenMtl()[i]->AddRef();
	}

	m_vecHiddenMtl = map->GetHiddenMtl();
	
	for (size_t i = 0; i < map->GetComMtl().size(); ++i)
	{
		map->GetComMtl()[i]->AddRef();
	}

	m_vecComMtl = map->GetComMtl();
	
	
}

void cMap::SetRefObj(cMap * map)
{
	for (size_t i = 0; i < map->GetHiddenObj().size(); ++i)
	{
		map->GetHiddenObj()[i]->AddRef();
	}

	m_vecHiddenObj = map->GetHiddenObj();
}



