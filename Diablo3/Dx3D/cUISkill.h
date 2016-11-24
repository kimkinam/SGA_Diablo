#pragma once


class cUISkill : public iButtonDelegate, public cUIObject
{
protected: 
	enum SkillState
	{
		S_normal, // 스킬 대기
		S_Clicked, // 스킬 시전
	};
	SkillState				m_SkillState;
	vector<LPDIRECT3DTEXTURE9> m_SvecTextur;
	float m_fUsedTime;
	SYNTHESIZE(bool, m_skillClick, isClick);
	SYNTHESIZE(iButtonDelegate*, m_sDelegate, s_Delegate);
	SYNTHESIZE(float, m_fCoolTime, CoolTime);


	
	


private:
	RECT rc_cooltime;


	LPDIRECT3DTEXTURE9 CoolTime_img;
	D3DXIMAGE_INFO CoolTime;
	LPD3DXSPRITE Cool_Bar;

	float Skill_CoolTime;
public:
	cUISkill();
	~cUISkill();

	void SetSkillTexture(char* szNor, char* szSel);
	void Update();
	void Render(LPD3DXSPRITE pSprite);
	virtual void OnClick(cUIButton * pSender) override;
};

