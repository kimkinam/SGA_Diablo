#pragma once

class cItem;

class cInventory
{
private:
	cUIObject*			m_pInvenItem;	//���� ������ ����
	cUIObject*			m_pInvenInfo;	//ĳ���� ������ ���� ����
	cUIObject*			m_pInvenStat;	//ĳ���� ���� ǥ�� ����

	std::vector<cItem*>	m_vecItem;
	cItem*				m_pCurItem;
	LPD3DXSPRITE		m_pSprite;
public:
	cInventory();
	~cInventory();

	void Setup();
	void Update();
	void Render();

	void SetupInven_Item();
	void SetupInven_Info();
	void SetupInven_Stat();
};

