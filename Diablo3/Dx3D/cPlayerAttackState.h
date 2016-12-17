#pragma once

class cPlayer;

class cPlayerAttackState :
	public iState<cPlayer>
{
private:
	cPlayerAttackState() {}
	virtual ~cPlayerAttackState() {}


public:
	static cPlayerAttackState* Instance()
	{
		static cPlayerAttackState instance;
		return &instance;
	}

	//���¿� ����
	virtual void Enter(cPlayer* pOwner);

	//���¿� �����ؼ� ����
	virtual void Execute(cPlayer* pOwner);

	//���¿��� ����
	virtual void Exit(cPlayer* pOwner);

	//GameObject�� HandleMessage�κ��� �޽����� �����ϸ� �� �κ��� ����
	virtual bool OnMessage(cPlayer* pOwner, const Telegram& msg);
};