#include "stdafx.h"
#include "cUIObject.h"



cUIObject::cUIObject()
	: m_vPosition(0, 0, 0)
	, m_pParent(NULL)
	, m_bIsDrawBorder(false)
	, m_bIsDraw(true)
	, m_nTag(Normal)
{
	m_stCollider = ST_COLLIDER();
	D3DXMatrixIdentity(&m_matW);
	m_stClickInfo = ST_CLICKED();
}


cUIObject::~cUIObject()
{
}

void cUIObject::Destroy()
{
	for each(auto c in m_vecChild)
	{
		c->Destroy();
	}
	this->Release();
}

void cUIObject::Update()
{
	D3DXMatrixIdentity(&m_matW);
	
	m_matW._41 = m_vPosition.x;
	m_matW._42 = m_vPosition.y;
	m_matW._43 = m_vPosition.z;

	if (m_pParent)
	{
		m_matW._41 += m_pParent->m_matW._41;
		m_matW._42 += m_pParent->m_matW._42;
		m_matW._43 += m_pParent->m_matW._43;
	}

	

	for each(auto c in m_vecChild)
	{
		c->Update();
	}
		
}

void cUIObject::Render(LPD3DXSPRITE pSprite)
{
	if (!m_bIsDraw) return;

	if (m_bIsDrawBorder)
		DrawBorder();

	for each(auto c in m_vecChild)
	{
		c->Render(pSprite);
	}
}

void cUIObject::DrawBorder()
{
	vector<ST_RHWC_VERTEX> vecVertex;
	vecVertex.reserve(8);

	ST_RHWC_VERTEX v;
	D3DXCOLOR c;
	c = D3DXCOLOR(0, 1, 0, 1);

	v.c = c;
	v.p = D3DXVECTOR4(m_matW._41, m_matW._42, 0, 1);
	vecVertex.push_back(v);

	v.p = D3DXVECTOR4(m_matW._41 + m_stCollider.nWidth, m_matW._42, 0, 1);
	vecVertex.push_back(v);

	v.p = D3DXVECTOR4(m_matW._41 + m_stCollider.nWidth, m_matW._42, 0, 1);
	vecVertex.push_back(v);

	v.p = D3DXVECTOR4(m_matW._41 + m_stCollider.nWidth, m_matW._42 + m_stCollider.nHeight, 0, 1);
	vecVertex.push_back(v);

	v.p = D3DXVECTOR4(m_matW._41 + m_stCollider.nWidth, m_matW._42 + m_stCollider.nHeight, 0, 1);
	vecVertex.push_back(v);

	v.p = D3DXVECTOR4(m_matW._41, m_matW._42 + m_stCollider.nHeight, 0, 1);
	vecVertex.push_back(v);

	v.p = D3DXVECTOR4(m_matW._41, m_matW._42 + m_stCollider.nHeight, 0, 1);
	vecVertex.push_back(v);

	v.p = D3DXVECTOR4(m_matW._41, m_matW._42, 0, 1);
	vecVertex.push_back(v);

	g_pD3DDevice->SetFVF(ST_RHWC_VERTEX::FVF);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST,
		4, &vecVertex[0], sizeof(ST_RHWC_VERTEX));

}

void cUIObject::AddChild(cUIObject * pChild)
{
	SAFE_ADDREF(pChild);
	if (!pChild) return;
	pChild->SetParent(this);
	m_vecChild.push_back(pChild);
}

void cUIObject::SetPosition(float x, float y, float z)
{
	m_vPosition.x = x;
	m_vPosition.y = y;
	m_vPosition.z = z;
}

void cUIObject::SetDraw(bool isDraw)
{
	for each(auto c in m_vecChild)
	{
		c->SetDraw(isDraw);
	}

	m_bIsDraw = isDraw;
}

cUIObject* cUIObject::FindChildByTag(cUIObject::Ui_Tag nTag)
{
	if (m_nTag == nTag)
		return this;

	for each(auto c in m_vecChild)
	{
		cUIObject* p = c->FindChildByTag(nTag);
		if (p) return p;
	}

	return NULL;
}

