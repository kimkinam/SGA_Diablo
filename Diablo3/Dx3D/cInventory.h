#pragma once

class cItem;
class cUIImage;

class cInventory : public iButtonDelegate, public cUIObject
{
private:
	cUIObject*			m_pInvenItem;	//���� ������ ����
	cUIObject*			m_pInvenInfo;	//ĳ���� ������ ���� ����
	cUIObject*			m_pInvenStat;	//ĳ���� ���� ǥ�� ����

	std::vector<cItem*>		m_vecItem; // ������ ���� ���� ��� ����
	std::vector<cUIImage*>	m_Itemimg; // ������ �̹��� ����
	cItem*					m_pCurItem;
	cUIImage*				test_item; // Ȱ
	cUIImage*				test_item_1; // ����
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

	std::vector<vector<cUIImage*>>		rc_inven; // �����κ� ����

	SYNTHESIZE(bool, Creat_item, CREAT_ITEM); // ������ ���� 

	bool m_isClick; //������ Ŭ�� �̺�Ʈ
	POINT m_ptClickPoint;


public:
	cInventory();
	~cInventory();

	void Setup();
	void Update();
	void Render();

	void SetupInven_Item(); // ���� ������ ����
	void SetupInven_Info(); // ������ ���� ����
	void SetupInven_Stat(); // ����ǥ�� ����

	virtual void OnClick(cUIButton* pSender) override;

	bool isPicked(cUIImage* item); // �������� �ȉ糪 �ȉ糪

	void invenPicked(cUIImage* item);
};

