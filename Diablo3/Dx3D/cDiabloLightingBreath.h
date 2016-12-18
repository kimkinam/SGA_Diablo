#pragma once

class cMonster;

class cDiabloLightingBreath :
	public iState<cMonster>
{
private:
	float	m_fSkillTime;
	bool	m_bIsBreath;
private:
	cDiabloLightingBreath() {}
	virtual ~cDiabloLightingBreath() {}

public:
	static cDiabloLightingBreath* Instance()
	{
		static cDiabloLightingBreath instance;
		return &instance;
	}

public:
	//���¿� ����
	virtual void Enter(cMonster* pOwner);

	//���¿� �����ؼ� ����
	virtual void Execute(cMonster* pOwner);

	//���¿��� ����
	virtual void Exit(cMonster* pOwner);

	//GameObject�� HandleMessage�κ��� �޽����� �����ϸ� �� �κ��� ����
	virtual bool OnMessage(cMonster* pOwner, const Telegram& msg);
};
