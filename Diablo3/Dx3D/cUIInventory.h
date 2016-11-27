#pragma once

class cUIInventory
{
private:
	ST_ITEM			item_;
	cUIObject*		m_pInvenItem;	//격자 아이템 공간
	cUIObject*		m_pInvenInfo;	//캐릭터 아이템 장착 공간
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

