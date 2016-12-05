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
	//상태에 진입
	virtual void Enter(cMonster* pOwner);

	//상태에 진입해서 갱신
	virtual void Execute(cMonster* pOwner);

	//상태에서 퇴장
	virtual void Exit(cMonster* pOwner);
};

