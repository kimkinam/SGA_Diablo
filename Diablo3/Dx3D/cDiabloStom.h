#pragma once

class cMonster;

class cDiabloStom :
	public iState<cMonster>
{
private:
	cDiabloStom() {}
	virtual ~cDiabloStom() {}

public:
	static cDiabloStom* Instance()
	{
		static cDiabloStom instance;
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
