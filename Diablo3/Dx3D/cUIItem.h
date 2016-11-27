#pragma once
class cUIItem : public iButtonDelegate, public cUIObject
{
protected:
	enum Item_state
	{
		Item_normal, // �������� �κ��丮�� ������
		Item_picked, // �������� ��ŷ ������
		Item_equiped // �������� ������ �Ǿ����� 
	};

	SYNTHESIZE(bool, m_ItemClick, Is_Item_Click);
	SYNTHESIZE(iButtonDelegate*, m_IDelegate, I_Delegate);
	SYNTHESIZE(ST_ITEM, m_stItemInfo, ItemInfo);
public:
	cUIItem();
	~cUIItem();
private:
	RECT				rc_item;
	Item_state			item_state;
	ST_PLAYET_STAT      ST_player_stat;
	LPDIRECT3DTEXTURE9 Item_img;
	D3DXIMAGE_INFO Item_img_info;


	void SetItem(char* szNor, float Hp, float Mp, float Atk, float Speed);
	void Update();
	void Render(LPD3DXSPRITE pSprite);
	virtual void OnClick(cUIButton * pSender) override;
};

