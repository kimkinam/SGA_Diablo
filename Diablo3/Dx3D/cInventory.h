#pragma once

class cItem;
class cUIImage;

class cInventory : public iButtonDelegate, public cUIObject
{
private:
	cUIObject*			m_pInvenItem;	//격자 아이템 공간
	cUIObject*			m_pInvenInfo;	//캐릭터 아이템 장착 공간
	cUIObject*			m_pInvenStat;	//캐릭터 스텟 표시 공간

	std::vector<cItem*>		m_vecItem; // 아이템 벡터 지금 사용 안함
	std::vector<cUIImage*>	m_Itemimg; // 아이템 이미지 벡터
	cItem*					m_pCurItem;
	cUIImage*				test_item; // 활
	cUIImage*				test_item_1; // 바지
	LPD3DXSPRITE			m_pSprite;

	cUIImage* Inven_head;
	cUIImage* Inven_chest;
	cUIImage* Inven_boots;
	cUIImage* Inven_pants;
	cUIImage* Inven_belt;
	cUIImage* Inven_arm;
	cUIImage* Inven_wrist;
	cUIImage* Inven_sword;
	cUIImage* Inven_shoulder;

	std::vector<vector<cUIImage*>>		rc_inven; // 격자인벤 벡터

	SYNTHESIZE(bool, Creat_item, CREAT_ITEM); // 아이템 생성 

	bool m_isClick; //아이템 클릭 이벤트
	POINT m_ptClickPoint;


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

	bool isPicked(cUIImage* item); // 아이템이 픽됬나 안됬나

	void invenPicked(cUIImage* item);
};

