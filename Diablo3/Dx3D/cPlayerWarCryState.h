#pragma once

class cPlayer;

class cPlayerWarCryState : public iState<cPlayer>
{
private:
	cPlayerWarCryState() {}
	virtual ~cPlayerWarCryState() {}


public:
	static cPlayerWarCryState* Instance()
	{
		static cPlayerWarCryState instance;
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

