#pragma once

class cUIImage;

class cItem : public cUIObject
{
private:

	SYNTHESIZE(E_ITEM, m_eItemType, ItemType);		// ������ Ÿ��
	SYNTHESIZE(float, fPlusHp, PlusHp);				// ���� ����
	SYNTHESIZE(float, fPlusMp, PlusMp);				// ���� ����
	SYNTHESIZE(float, fPlusAtk, PlusAtk);			// ���Ⱑ ����
	SYNTHESIZE(float, fPlusSpeed, PlusSpeed);		// ������ ����

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

