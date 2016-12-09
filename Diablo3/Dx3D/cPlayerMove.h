#pragma once

class cPlayer;

class cPlayerMove :
	public iState<cPlayer>
{
private:
	cPlayerMove() {}
	virtual ~cPlayerMove() {}

public:
	static cPlayerMove* Instance()
	{
		static cPlayerMove instance;
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


