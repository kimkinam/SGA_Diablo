#pragma once

class cMonster;

class cDiabloStom :
	public iState<cMonster>
{
private:
	cDiabloStom() {}
	virtual ~cDiabloStom() {}

public:
	static cDiabloStom* Instance()
	{
		static cDiabloStom instance;
		return &instance;
	}

public:
	//상태에 진입
	virtual void Enter(cMonster* pOwner);

	//상태에 진입해서 갱신
	virtual void Execute(cMonster* pOwner);

	//상태에서 퇴장
	virtual void Exit(cMonster* pOwner);

	//GameObject의 HandleMessage로부터 메시지를 접수하면 이 부분이 실행
	virtual bool OnMessage(cMonster* pOwner, const Telegram& msg);
};
