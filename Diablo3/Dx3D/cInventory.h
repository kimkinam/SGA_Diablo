#pragma once

class cItem;
class cUIImage;

class cInventory : public iButtonDelegate,public cUIObject
{
private:
	cUIObject*			m_pInvenItem;	//격자 아이템 공간
	cUIObject*			m_pInvenInfo;	//캐릭터 아이템 장착 공간
	cUIObject*			m_pInvenStat;	//캐릭터 스텟 표시 공간

	std::vector<cItem*>	m_vecItem;
	cItem*				m_pCurItem;
	LPD3DXSPRITE		m_pSprite;

	cUIImage* Inven_head;
	cUIImage* Inven_chest;
	cUIImage* Inven_boots;
	cUIImage* Inven_pants;
	cUIImage* Inven_belt;
	cUIImage* Inven_arm;
	cUIImage* Inven_wrist;
	cUIImage* Inven_sword;
	cUIImage* Inven_shoulder;

	std::vector<vector<cUIImage*>>		rc_inven;



public:
	cInventory();
	~cInventory();

	void Setup();
	void Update();
	void Render();

	void SetupInven_Item(); // 격자 아이템 공간
	void SetupInven_Info(); // 아이템 장착 공간
	void SetupInven_Stat(); // 스텟표시 공간

	virtual void OnClick(cUIButton* pSender) override;
};

