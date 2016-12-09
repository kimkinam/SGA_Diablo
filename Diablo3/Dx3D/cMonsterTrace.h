#pragma once

class cMonster;

class cMonsterTrace :
	public iState<cMonster>
{
public:
	cMonsterTrace(){ }
	virtual ~cMonsterTrace() { }

public:
	static cMonsterTrace* Instance()
	{
		static cMonsterTrace instance;
		return &instance;
	}

	//���¿� ����
	virtual void Enter(cMonster* pOwner);

	//���¿� �����ؼ� ����
	virtual void Execute(cMonster* pOwner);

	//���¿��� ����
	virtual void Exit(cMonster* pOwner);

	//GameObject�� HandleMessage�κ��� �޽����� �����ϸ� �� �κ��� ����
	virtual bool OnMessage(cMonster* pOwner, const Telegram& msg);
};

