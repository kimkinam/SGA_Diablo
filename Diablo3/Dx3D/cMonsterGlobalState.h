#pragma once

class cMonster;

class cMonsterGlobalState :
	public iState<cMonster>
{
private:
	cMonsterGlobalState() {}
	virtual ~cMonsterGlobalState() {}

public:
	static cMonsterGlobalState* Instance()
	{
		static cMonsterGlobalState instance;
		return &instance;
	}

	//상태에 진입
	virtual void Enter(cMonster* pOwner);

	//상태에 진입해서 갱신
	virtual void Execute(cMonster* pOwner);

	//상태에서 퇴장
	virtual void Exit(cMonster* pOwner);
};

