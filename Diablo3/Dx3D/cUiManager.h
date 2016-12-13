#pragma once

class cUISkill;
class cUIImage;
class cInventory;


#define HP_SPHERE_SCALE	 2
#define HP_SPHERE_SIZE_X 48 * HP_SPHERE_SCALE
#define HP_SPHERE_SIZE_Y 49 * HP_SPHERE_SCALE

struct ST_HPSPHERE
{
	int nCount;
	int nIndex;
	DWORD dwAniTime;
	DWORD dwOldAniTime;
	std::vector<RECT> pRect;
	std::vector<D3DXVECTOR3> pCenter;
};


class cUiManager : public iButtonDelegate
{
private:
	cPlayer*			m_pPlayer;
	//cInventory*			Inventory;
	cUIObject*			m_pHpBar;
	//cUIObject*			m_pInven;
	cUIObject*			HP_sphere;
	cUIObject*			MP_sphere;
	cUISkill*			pBaba_skill_1;
	POINT				m_ptClickPoint;
	SYNTHESIZE(LPD3DXSPRITE, m_pSprite, Sprite);
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

	//-----------------------
	LPD3DXSPRITE				m_pAniSprite;
	ST_HPSPHERE					m_Hp;
	LPDIRECT3DTEXTURE9			m_pHpTex;
	ST_HPSPHERE					m_Mp;
	LPDIRECT3DTEXTURE9			m_pMpTex;
	float						m_fCurHp;
	float						m_fMinusHp;
	float						m_fMaxHp;




	char* Skill_FileName;
public:
	cUiManager();
	~cUiManager();

	void SetUp();
	void SetUpHpBar(RECT rc);
	void Update();
	void Render();

	bool InCollider(cUIObject* pUI);

	//player관련

	void HpSphereUpdate();
	void HpSphereRender();
	void SetSphere(ST_HPSPHERE& sphere);
	void SetAddressLink(cPlayer* player) {m_pPlayer = player;}
	//iButtonDelegate override
	virtual void OnClick(cUIButton* pSender) override;

	
};

