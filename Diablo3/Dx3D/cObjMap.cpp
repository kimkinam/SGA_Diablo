#include "stdafx.h"
#include "cObjMap.h"
#include "cMtlTex.h"
#include "cObjLoader.h"


cObjMap::cObjMap()
	: m_matWorld(NULL)
	, m_pMesh(NULL)
	, m_vPosition(0, 0, 0)
	, m_pEffect(NULL)
{

}


cObjMap::~cObjMap()
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pEffect);

	for each(auto c in m_vecMtl)
	{
		SAFE_RELEASE(c);
	}
}

void cObjMap::Load(char * szMap, char * szSurface, char * szFolder, D3DXMATRIXA16 * pMat)
{
	cObjLoader loader;
	m_pMesh = loader.Load(szMap, szFolder, m_vecMtl, pMat);

	m_pEffect = LoadEffect("./Resources/Shaders/heatHaze.fx");

	std::vector<D3DXVECTOR3> vecV;

	FILE* fp = NULL;
	string sFullPath = string(szFolder) + string(szSurface);
	fopen_s(&fp, sFullPath.c_str(), "r");

	while (!feof(fp))
	{
		char szBuf[1024] = { '\0', };

		fgets(szBuf, 1024, fp);

		if (strlen(szBuf) == 0)
			continue;

		if (szBuf[0] == '#')
		{
			continue;
		}
		else if (szBuf[0] == 'v' && szBuf[1] < 33)
		{
			float x, y, z;
			sscanf_s(szBuf, "%*s %f %f %f", &x, &y, &z);
			vecV.push_back(D3DXVECTOR3(x, y, z));
		}
		else if (szBuf[0] == 'f')
		{
			int aIndex[3];
			sscanf_s(szBuf, "%*s %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d",
				&aIndex[0], &aIndex[1], &aIndex[2]);

			for (int i = 0; i < 3; ++i)
			{
				D3DXVECTOR3 p = vecV[aIndex[i] - 1];
				if (pMat)
				{
					D3DXVec3TransformCoord(&p, &p, pMat);
				}
				m_vecSurface.push_back(p);
			}
		}
	}
	fclose(fp);

	D3DXMATRIX matS, matR, matT, matW;
	D3DXMatrixScaling(&matS, 0.5f, 0.5f, 0.5f);
	//D3DXMatrixIdentity(&matR);
	D3DXMatrixRotationY(&matR, D3DXToRadian(90));
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	matW = matS * matR * matT;

	for (size_t i = 0; i < m_vecSurface.size(); ++i)
	{
		D3DXVec3TransformCoord(&m_vecSurface[i], &m_vecSurface[i], &matW);
	}


}

void cObjMap::Render()
{
	g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	
	ULONGLONG tickCount = GetTickCount64();
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

		for (size_t i = 0; i < m_vecMtl.size(); ++i)
		{
			g_pD3DDevice->SetMaterial(&m_vecMtl[i]->GetMtl());
			g_pD3DDevice->SetTexture(0, m_vecMtl[i]->GetTexture());

			if (m_vecMtl[i]->GetIsHiddenObj())
			{
				g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
				g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
				g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
				g_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
				//g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

			}

			//if (i == 8) continue;
			if (i == 8)
			{
				ULONGLONG tick = GetTickCount64();
			
				g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
				g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
				g_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_SUBTRACT);
				g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			
				D3DXMATRIXA16 matView, matProj;
				g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
				g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);
			
				m_pEffect->SetMatrix("matWorld", &matW);
				m_pEffect->SetMatrix("matWorldViewProjection", &(matW * matView * matProj));
				m_pEffect->SetFloat("fSpeed", 0.5f);
				m_pEffect->SetFloat("uvspeed", -0.2f);
				m_pEffect->SetFloat("fTime", tickCount/1000.0f);
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
				g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

			}
		}
	}
}

bool cObjMap::GetHeight(IN float x, OUT float & y, IN float z)
{
	D3DXVECTOR3 vRayPos(x, 1000, z);
	D3DXVECTOR3 vRayDir(0, -1, 0);

	for (size_t i = 0; i < m_vecSurface.size(); i += 3)
	{
		float u, v, d;
		if (D3DXIntersectTri(&m_vecSurface[i + 0],
			&m_vecSurface[i + 1],
			&m_vecSurface[i + 2],
			&vRayPos,
			&vRayDir,
			&u, &v, &d))
		{
			y = 1000 - d;
			return true;
		}
	}
	return false;
}
