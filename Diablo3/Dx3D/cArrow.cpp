#include "stdafx.h"
#include "cArrow.h"
#include "cActionMove.h"
#include "cObjLoader.h"
#include "cMtlTex.h"

cArrow::cArrow()
	: m_pArrowMesh(NULL)
{
}


cArrow::~cArrow()
{
	SAFE_RELEASE(m_pArrowMesh);

	for each(auto c in m_vecMtl)
	{
		SAFE_RELEASE(c);
	}
}


void cArrow::Setup(D3DXVECTOR3* vLook)
{
	//cGameObject::Setup(vLook);

	cObjLoader l;
	m_pArrowMesh = l.Load("arrow.objobj", "./Resources/Object/", m_vecMtl, NULL);
	
	int a = 0;
	
	m_matWorld = m_matLocal;

}

void cArrow::Update()				  
{
	
	if (m_pAction)
		m_pAction->Update();
}

void cArrow::Render()					  
{ 
	cGameObject::Render();
	D3DXMATRIXA16 matT;
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	
	m_matWorld *= matT;
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	for (size_t i = 0; i < m_vecMtl.size(); ++i)
	{
		g_pD3DDevice->SetMaterial(&m_vecMtl[i]->GetMtl());
		g_pD3DDevice->SetTexture(0, m_vecMtl[i]->GetTexture());
		m_pArrowMesh->DrawSubset(i);
	}
}
