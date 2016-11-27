#pragma once

class cPlayerManager;
class cUISkill;
class cUIImage;

class cUiManager : public iButtonDelegate
{
private:
	cUIObject*			m_pHpBar;
	cUIObject*			m_pInven;
	cUIObject*			HP_sphere;
	cUIObject*			MP_sphere;
	cUISkill*			pBaba_skill_1;
	SYNTHESIZE(LPD3DXSPRITE, m_pSprite, Sprite);
	POINT				m_ptClickPoint;
	cPlayerManager*		m_pPlayerManager;
	SYNTHESIZE(bool,	m_IsClick, IsClick);

	//==인벤 무기이미지==//
	/*	Inven_shoulder,
		Inven_chest,
		Inven_boots,
		Inven_pants,
		Inven_belt,
		Inven_arm,
		Inven_wrist
		Inven_sword */
	cUIImage* Inven_head;
	cUIImage* Inven_chest;
	cUIImage* Inven_boots;
	cUIImage* Inven_pants;
	cUIImage* Inven_belt;
	cUIImage* Inven_arm;
	cUIImage* Inven_wrist;
	cUIImage* Inven_sword;
	cUIImage* Inven_shoulder;


	std::vector<cUIImage*> Image_vec;
	//==================//



	char* Skill_FileName;
public:
	cUiManager();
	~cUiManager();

	void SetUp();
	void SetUpHpBar(RECT rc);
	void Update();
	void Render();
	
	void SetAddressLink(cPlayerManager* pm) 
	{
		assert(pm != NULL && "플레이어매니저 없다.");
		m_pPlayerManager = pm; 
	}

	//void UITranslation(cUIObject* pRoot);
	
	bool InCollider(cUIObject* pUI);

	virtual void OnClick(cUIButton* pSender) override;

	void Itemswap(cUIImage* Findimage);
	char* imageName();
	
};

