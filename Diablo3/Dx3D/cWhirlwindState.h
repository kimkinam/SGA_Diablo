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


	//상태에 진입
	virtual void Enter(cPlayer* pOwner);

	//상태에 진입해서 갱신
	virtual void Execute(cPlayer* pOwner);

	//상태에서 퇴장
	virtual void Exit(cPlayer* pOwner);

	//GameObject의 HandleMessage로부터 메시지를 접수하면 이 부분이 실행
	virtual bool OnMessage(cPlayer* pOwner, const Telegram& msg);
};

