#pragma once

class cPlayerManager;
class cUISkill;

class cUiManager : public iButtonDelegate
{
private:
	cUIObject*			m_pHpBar;
	cUIObject*			m_pInven;
	cUIObject*			HP_sphere;
	cUIObject*			MP_sphere;
	cUISkill*			pBaba_skill_1;
	LPD3DXSPRITE		m_pSprite;
	POINT				m_ptClickPoint;
	cPlayerManager*		m_pPlayerManager;
	SYNTHESIZE(bool,	m_IsClick, IsClick);


	char* Skill_FileName;
public:
	cUiManager();
	~cUiManager();

	void SetUp();
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
};

