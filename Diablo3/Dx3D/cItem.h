#pragma once

class cUIImage;

class cItem : public cObject
{
private:

	SYNTHESIZE(E_ITEM, m_eItemType, ItemType);		// 아이템 타입
	SYNTHESIZE(float, fPlusHp, PlusHp);				// 방어구가 증가
	SYNTHESIZE(float, fPlusMp, PlusMp);				// 방어구가 증가
	SYNTHESIZE(float, fPlusAtk, PlusAtk);			// 무기가 증가
	SYNTHESIZE(float, fPlusSpeed, PlusSpeed);		// 부츠가 증가
	
private:
	char*		m_ImageName;
	char*		m_SumNailName;
	cUIImage*	m_pSumNail;
	cUIImage*	m_pImage;
	POINT		m_ptArea;

public:
	cItem();
	~cItem();

	void Setup(char* szFolder, char* szImageName, char* sumNailName);
};

