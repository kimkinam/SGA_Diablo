#pragma once

class cPlayerManager;

class cUiManager : public iButtonDelegate
{
private:
	cUIObject*			m_pHpBar;
	cUIObject*			m_pInven;
	cUIObject*			HP_sphere;
	cUIObject*			MP_sphere;

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

