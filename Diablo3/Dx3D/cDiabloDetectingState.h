#pragma once

class cMonster;

class cDiabloDetectingState :
	public iState<cMonster>
{

public:
	cDiabloDetectingState() {};
	virtual  ~cDiabloDetectingState() {};


public:
	static cDiabloDetectingState* Instance()
	{
		static cDiabloDetectingState instance;
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

