#pragma once

class cPlayer;

class cPlayerIdleState :
	public iState<cPlayer>
{
private:
	float	m_fLastHittedTime;
private:
	cPlayerIdleState() {}
	virtual ~cPlayerIdleState() {}

public:
	static cPlayerIdleState* Instance()
	{
		static cPlayerIdleState instance;
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

