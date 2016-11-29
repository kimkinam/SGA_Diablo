#pragma once

class cUIImage;

class cItem : public cUIObject
{
private:

	SYNTHESIZE(E_ITEM, m_eItemType, ItemType);		// 아이템 타입
	SYNTHESIZE(float, fPlusHp, PlusHp);				// 방어구가 증가
	SYNTHESIZE(float, fPlusMp, PlusMp);				// 방어구가 증가
	SYNTHESIZE(float, fPlusAtk, PlusAtk);			// 무기가 증가
	SYNTHESIZE(float, fPlusSpeed, PlusSpeed);		// 부츠가 증가

	SYNTHESIZE(ST_ITEM, St_item, st_item);
	SYNTHESIZE(POINT, m_ptArea, item_position);
private:
	char*		m_ImageName;
	char*		m_SumNailName;
	LPDIRECT3DTEXTURE9	m_pSumNail;
	LPDIRECT3DTEXTURE9	m_pImage;

public:
	cItem();
	~cItem();

	void Setup(char* szImageName, char* sumNailName, float fhp, float fmp, float fAtk, float fSpeed, E_ITEM Item_type);
	void Update();
	void Render(LPD3DXSPRITE pSprite);
};

