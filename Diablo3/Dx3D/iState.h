#pragma once

template <class T>
class iState
{
public:
	virtual ~iState(){ }

	//상태에 진입
	virtual void Enter(T* object) PURE;
	//상태에 진입해서 갱신
	virtual void Execute(T* object) PURE;
	//상태에서 퇴장
	virtual void Exit(T* object) PURE;
	//GameObject의 HandleMessage로부터 메시지를 접수하면 이 부분이 실행
	virtual bool OnMessage(T* object, const Telegram& msg) PURE;
};

