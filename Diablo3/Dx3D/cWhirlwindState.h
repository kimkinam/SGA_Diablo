#pragma once

class cPlayer;
class cMonster;

class cWhirlwindState :
	public iState<cPlayer>
{
private:
	std::vector<cGameObject*>	m_vecMonster;
	bool						m_bIsAtk;
private:
	cWhirlwindState() {}
	virtual ~cWhirlwindState() {}


public:
	static cWhirlwindState* Instance()
	{
		static cWhirlwindState instance;
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

