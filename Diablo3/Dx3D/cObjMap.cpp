#include "stdafx.h"
#include "cObjMap.h"
#include "cObjLoader.h"
#include "cGroup.h"

cObjMap::cObjMap()
{
	D3DXMatrixIdentity(&m_mat);
}


cObjMap::~cObjMap()
{
	for each(auto p in m_vecGroup)
	{
		SAFE_DELETE(p);
	}
}

void cObjMap::Load(char * szMap, char * szSurface, D3DXMATRIXA16 *pMat)
{
	
	cObjLoader l;
	l.Load(szMap, m_vecGroup, pMat);


	for (size_t i = 0; i < m_vecGroup.size(); ++i)
	{
		g_pD3DDevice->CreateVertexBuffer(m_vecGroup[i]->GetVertex().size() * sizeof(ST_PNT_VERTEX), 0, ST_PNT_VERTEX::FVF, D3DPOOL_MANAGED, &m_vecGroup[i]->GetVB(), NULL);
	
		void* pVertices;
		m_vecGroup[i]->GetVB()->Lock(0, m_vecGroup[i]->GetVertex().size() * sizeof(ST_PNT_VERTEX), (void**)&pVertices, 0);
	
		memcpy(pVertices, &m_vecGroup[i]->GetVertex()[0], m_vecGroup[i]->GetVertex().size() * sizeof(ST_PNT_VERTEX));
	
		m_vecGroup[i]->GetVB()->Unlock();
	
	}

	l.LoadSurface(szSurface, m_vecSurface, pMat);
}

bool cObjMap::GetHeight(IN float x, OUT float & y, IN float z)
{

	D3DXVECTOR3 vRayPos(x, 1000, z);
	D3DXVECTOR3 vRayDirection(0, -1, 0);

	for (size_t i = 0; i < m_vecSurface.size(); i += 3)
	{
		float u, v, length;

		if (D3DXIntersectTri(&m_vecSurface[i], &m_vecSurface[i + 1], &m_vecSurface[i + 2], &vRayPos, &vRayDirection, &u, &v, &length))
		{
			y = 1000 - length;
			return true;
		}
	}
	return false;
}

void cObjMap::Render()
{
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_mat);
	for each(auto p in m_vecGroup)
	{
		p->Render();
	}
}
