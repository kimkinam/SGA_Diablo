#pragma once

class cUIImage;

class cItem : public cObject
{
private:

	SYNTHESIZE(E_ITEM, m_eItemType, ItemType);		// ������ Ÿ��
	SYNTHESIZE(float, fPlusHp, PlusHp);				// ���� ����
	SYNTHESIZE(float, fPlusMp, PlusMp);				// ���� ����
	SYNTHESIZE(float, fPlusAtk, PlusAtk);			// ���Ⱑ ����
	SYNTHESIZE(float, fPlusSpeed, PlusSpeed);		// ������ ����
	
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

