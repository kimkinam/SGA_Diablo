#pragma once

class cMonster;

class cMonsterAttack
	: public iState<cMonster>
{
private:
	float	m_fAttackRange;
	float	m_fDelay;
	float	m_fRotationDelay;

	float	m_fAttackTime;
public:
	cMonsterAttack() { }
	virtual ~cMonsterAttack() { }

public:
	static cMonsterAttack* Instance()
	{
		static cMonsterAttack instance;
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

