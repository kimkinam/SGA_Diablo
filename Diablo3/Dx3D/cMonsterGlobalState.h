#pragma once

class cMonster;
class cGameObject;

class cMonsterGlobalState :
	public iState<cGameObject>
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
	virtual void Enter(cGameObject* pOwner);

	//���¿� �����ؼ� ����
	virtual void Execute(cGameObject* pOwner);

	//���¿��� ����
	virtual void Exit(cGameObject* pOwner);

	//GameObject�� HandleMessage�κ��� �޽����� �����ϸ� �� �κ��� ����
	virtual bool OnMessage(cGameObject* pOwner, const Telegram& msg);
};

