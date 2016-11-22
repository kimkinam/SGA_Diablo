#include "StdAfx.h"
#include "cGroup.h"
#include "cMtlTex.h"

cGroup::cGroup(void)
	: m_pMtlTex(NULL)
	, m_pVB(NULL)
{
}

cGroup::~cGroup(void)
{
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pMtlTex);
}

void cGroup::Render()
{
	g_pD3DDevice->SetMaterial(&m_pMtlTex->GetMtl());
	//g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	g_pD3DDevice->SetTexture(0, m_pMtlTex->GetTexture());
	g_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(ST_PNT_VERTEX));
	g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
	g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_vecVertex.size()/3);

	/*g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
		m_vecVertex.size() / 3,
		&m_vecVertex[0],
		sizeof(ST_PNT_VERTEX));*/
}
