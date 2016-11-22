#pragma once

class cUIButton 
	: public cUIObject
	, public iButtonDelegate
{
protected:
	enum eButtonState
	{
		E_NORMAL,
		E_OVER,
		E_CLICKED,
	};
	eButtonState				m_emButtonState;
	vector<LPDIRECT3DTEXTURE9>	m_vecTexture;
	SYNTHESIZE(bool, m_bIsClick, IsClick);
	SYNTHESIZE(iButtonDelegate*, m_pDelegate, Delegate);
public:
	cUIButton(void);
	virtual ~cUIButton(void);

	virtual void SetTexture(char* szNor, char* szOvr, char* szSel);
	// cUIObject override
	virtual void Update() override;
	virtual void Render(LPD3DXSPRITE pSprite) override;
	virtual void OnClick(cUIButton * pSender) override;
};

