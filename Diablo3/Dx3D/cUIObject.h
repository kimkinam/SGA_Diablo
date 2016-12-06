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
		Inven_sword,

		thumbnail_Monster,
		thumbnail_Object
	};

protected:
	std::vector<cUIObject*>	m_vecChild;
	D3DXMATRIXA16			m_matW;

	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vPosition, Position);
	SYNTHESIZE_PASS_BY_REF(RECT, m_rcDraw, DrawRc); // 렉트 정보 
	SYNTHESIZE_PASS_BY_REF(ST_COLLIDER, m_stCollider, Collider);
	SYNTHESIZE(bool, m_bIsPicked, IsPicked);
	SYNTHESIZE(bool, m_Isempty, IsEmpty);
	SYNTHESIZE(cUIObject*, m_pParent, Parent);
	SYNTHESIZE(bool, m_bIsDrawBorder, DrawBorder);
	SYNTHESIZE(bool, m_bIsDraw, IsDraw);
	SYNTHESIZE(Ui_Tag, m_nTag, Tag);
	SYNTHESIZE(D3DXMATRIX, m_matS, matS);

public:
	static UINT objCount;
	static UINT monsterCount;
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
	virtual cUIObject* FindPtIn();
	virtual POINT GetStartPos();
	virtual void DeleteChildByTag(cUIObject::Ui_Tag nTag);
};

