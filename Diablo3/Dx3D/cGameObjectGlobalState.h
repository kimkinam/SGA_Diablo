#pragma once

class cMonster;
class cGameObject;

class cGameObjectGlobalState :
	public iState<cGameObject>
{
private:
	cGameObjectGlobalState() {}
	virtual ~cGameObjectGlobalState() {}

public:
	static cGameObjectGlobalState* Instance()
	{
		static cGameObjectGlobalState instance;
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

