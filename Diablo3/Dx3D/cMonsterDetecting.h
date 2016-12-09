#pragma once

class cMonster;

class cMonsterDetecting :
	public iState<cMonster>
{
private:
	cMonsterDetecting() {}
	virtual ~cMonsterDetecting() {}

public:
	static cMonsterDetecting* Instance()
	{
		static cMonsterDetecting instance;
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


