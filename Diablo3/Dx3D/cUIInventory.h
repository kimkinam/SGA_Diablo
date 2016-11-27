#pragma once

class cUIInventory
{
private:
	ST_ITEM			item_;
	cUIObject*		m_pInvenItem;	//���� ������ ����
	cUIObject*		m_pInvenInfo;	//ĳ���� ������ ���� ����
	cUIObject*		m_pInvenStat;
public:
	cUIInventory();
	~cUIInventory();

	void Setup();
	void Update();
	void Render();

	void SetupInven_Item();
	void SetupInven_Info();
	void SetupInven_Stat();

};

