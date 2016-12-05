#pragma once

class cMonster;

class cMonsterTrace :
	public iState<cMonster>
{
public:
	cMonsterTrace(){ }
	virtual ~cMonsterTrace() { }

public:
	static cMonsterTrace* Instance()
	{
		static cMonsterTrace instance;
		return &instance;
	}

	//상태에 진입
	virtual void Enter(cMonster* pOwner);

	//상태에 진입해서 갱신
	virtual void Execute(cMonster* pOwner);

	//상태에서 퇴장
	virtual void Exit(cMonster* pOwner);
};

