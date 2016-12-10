#pragma once

class cPlayer;

class cPlayerMoveState :
	public iState<cPlayer>
{
private:
	cPlayerMoveState() {}
	virtual ~cPlayerMoveState() {}

public:
	static cPlayerMoveState* Instance()
	{
		static cPlayerMoveState instance;
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


