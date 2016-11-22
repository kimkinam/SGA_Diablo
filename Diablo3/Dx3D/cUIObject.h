#pragma once

class cUIObject 
	: public cObject
{
protected:
	std::vector<cUIObject*>	m_vecChild;
	D3DXMATRIXA16			m_matW;

	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vPosition, Position);
	SYNTHESIZE(cUIObject*, m_pParent, Parent);
	SYNTHESIZE(bool, m_bIsDrawBorder, DrawBorder);
	SYNTHESIZE(bool, m_bIsDraw, IsDraw);
	SYNTHESIZE(int, m_nTag, Tag);
	SYNTHESIZE(RECT, m_rcDraw, DrawRc);
	SYNTHESIZE(D3DXMATRIX, m_matS, matS);
	SYNTHESIZE_PASS_BY_REF(ST_CLICKED, m_stClickInfo, ClickInfo);
	SYNTHESIZE_PASS_BY_REF(ST_COLLIDER, m_stCollider, Collider);

	
public:
	cUIObject();
	virtual ~cUIObject();

	virtual void AddChild(cUIObject* pChild);
	virtual void Update();
	virtual void Render(LPD3DXSPRITE pSprite);
	virtual void Destroy();
	virtual void SetPosition(float x, float y, float z = 0);
	virtual void DrawBorder();
	virtual void SetDraw(bool isDraw);

	virtual cUIObject* FindChildByTag(int nTag);
};

