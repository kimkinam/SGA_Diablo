#pragma once




class cUIObject 
	: public cObject
{
public : 
	enum Ui_Tag
	{
		Normal,
		Skill_1, // 힐윈드
		Button_1, // 인벤토리 엑스와이
		Inven_head,
		Inven_shoulder,
		Inven_chest,
		Inven_boots,
		Inven_pants,
		Inven_belt,
		Inven_arm,
		Inven_wrist,
		Inven_sword
	};
protected:
	std::vector<cUIObject*>	m_vecChild;
	D3DXMATRIXA16			m_matW;

	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vPosition, Position);
	SYNTHESIZE(cUIObject*, m_pParent, Parent);
	SYNTHESIZE(bool, m_bIsDrawBorder, DrawBorder);
	SYNTHESIZE(bool, m_bIsDraw, IsDraw);
	SYNTHESIZE(Ui_Tag, m_nTag, Tag);
	SYNTHESIZE_PASS_BY_REF(RECT, m_rcDraw, DrawRc); // 렉트 정보 
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

	virtual cUIObject* FindChildByTag(cUIObject::Ui_Tag nTag);
};

