#pragma once

class cPlayer;

class cPlayerWarCryState : public iState<cPlayer>
{
private:
	cPlayerWarCryState() {}
	virtual ~cPlayerWarCryState() {}


public:
	static cPlayerWarCryState* Instance()
	{
		static cPlayerWarCryState instance;
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

