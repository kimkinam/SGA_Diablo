#pragma once

class cItem;

class cInventory
{
private:
	cUIObject*			m_pInvenItem;	//격자 아이템 공간
	cUIObject*			m_pInvenInfo;	//캐릭터 아이템 장착 공간
	cUIObject*			m_pInvenStat;	//캐릭터 스텟 표시 공간

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

