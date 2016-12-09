#pragma once

class cMonster;

class cMonsterGlobalState :
	public iState<cMonster>
{
private:
	cMonsterGlobalState() {}
	virtual ~cMonsterGlobalState() {}

public:
	static cMonsterGlobalState* Instance()
	{
		static cMonsterGlobalState instance;
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

