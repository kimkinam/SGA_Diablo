#pragma once

class cMonster;
class cGameObject;

class cGameObjectGlobalState :
	public iState<cGameObject>
{
private:
	cGameObjectGlobalState() {}
	virtual ~cGameObjectGlobalState() {}

public:
	static cGameObjectGlobalState* Instance()
	{
		static cGameObjectGlobalState instance;
		return &instance;
	}

	//상태에 진입
	virtual void Enter(cGameObject* pOwner);

	//상태에 진입해서 갱신
	virtual void Execute(cGameObject* pOwner);

	//상태에서 퇴장
	virtual void Exit(cGameObject* pOwner);

	//GameObject의 HandleMessage로부터 메시지를 접수하면 이 부분이 실행
	virtual bool OnMessage(cGameObject* pOwner, const Telegram& msg);
};

