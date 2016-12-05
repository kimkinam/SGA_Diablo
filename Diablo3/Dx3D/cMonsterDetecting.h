#pragma once

class cMonster;

class cMonsterDetecting :
	public iState<cMonster>
{
private:
	cMonsterDetecting() {}
	virtual ~cMonsterDetecting() {}

public:
	static cMonsterDetecting* Instance()
	{
		static cMonsterDetecting instance;
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


