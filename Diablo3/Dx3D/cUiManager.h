#pragma once

class cPlayerManager;

class cUiManager : public iButtonDelegate
{
private:
	cUIObject*			m_pHpBar;
	cUIObject*			m_pInven;

	LPD3DXSPRITE		m_pSprite;
	POINT				m_ptClickPoint;
	cPlayerManager*		m_pPlayerManager;
	SYNTHESIZE(bool,	m_IsClick, IsClick);
public:
	cUiManager();
	~cUiManager();

	void SetUp();
	void Update();
	void Render();
	
	void SetAddressLink(cPlayerManager* pm) 
	{
		assert(pm != NULL && "�÷��̾�Ŵ��� ����.");
		m_pPlayerManager = pm; 
	}

	//void UITranslation(cUIObject* pRoot);
	
	bool InCollider(cUIObject* pUI);

	virtual void OnClick(cUIButton* pSender) override;
};
