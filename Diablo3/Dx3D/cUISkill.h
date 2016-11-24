#pragma once


class cUISkill : public iButtonDelegate, public cUIObject
{
protected: 
	enum SkillState
	{
		S_normal, // 스킬 대기
		S_CoolTime, // 스킬 쿨타임
		S_Clicked, // 스킬 시전
	};
	SkillState				m_SkillState;
	vector<LPDIRECT3DTEXTURE9> m_SvecTextur;
	SYNTHESIZE(bool, m_skillClick, isClick);
	SYNTHESIZE(iButtonDelegate*, m_sDelegate, s_Delegate);


private:
	RECT rc_cooltime;
public:
	cUISkill();
	~cUISkill();

	void SetSkillTexture(char* szNor, char* szCtm, char* szSel);
	void Update();
	void Render();
	virtual void OnClick(cUIButton * pSender) override;
};

