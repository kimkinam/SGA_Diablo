#pragma once

class cMonster;

class cMonsterAttack
	: public iState<cMonster>
{
public:
	cMonsterAttack() { }
	virtual ~cMonsterAttack() { }

public:
	static cMonsterAttack* Instance()
	{
		static cMonsterAttack instance;
		return &instance;
	}

public:
	//���¿� ����
	virtual void Enter(cMonster* pOwner);

	//���¿� �����ؼ� ����
	virtual void Execute(cMonster* pOwner);

	//���¿��� ����
	virtual void Exit(cMonster* pOwner);
};

