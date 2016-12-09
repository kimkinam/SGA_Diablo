#pragma once

class cGameObject;

template <class T>
class cStateMachine
{
private:
	typedef iState<T>	State;
private:
	T*	m_pOwner;
	SYNTHESIZE(State*, m_pCurState, CurState);
	SYNTHESIZE(State*, m_pPreState, PreState);
<<<<<<< HEAD
	 SYNTHESIZE(State*, m_pGlobalState, GlobalState);
=======
	SYNTHESIZE(iState<cGameObject>*, m_pGlobalState, GlobalState);
>>>>>>> fd0862dac1e580af3d9fcbc3a09f36f02618a17e

public:
	cStateMachine(T* owner)
	{
		m_pOwner = owner;
		m_pCurState = m_pPreState = NULL;
		m_pGlobalState = NULL;
	}

	~cStateMachine()
	{
		SAFE_DELETE(m_pCurState);
		SAFE_DELETE(m_pPreState);
		SAFE_DELETE(m_pGlobalState);
	}

	void Update()
	{
		if (m_pGlobalState) m_pGlobalState->Execute(m_pOwner);
		if (m_pCurState) m_pCurState->Execute(m_pOwner);
	}

	void ChangeState(State* pNewState)
	{
		assert(pNewState && "변경하려는 상태가 NULL입니다.");

		m_pPreState = m_pCurState;
		m_pCurState->Exit(m_pOwner);
		m_pCurState = pNewState;

		m_pCurState->Enter(m_pOwner);
	}

	void RevertToPreState()
	{
		ChangeState(m_pPreState);
	}

	bool IsInState(const State& st)
	{
		if (m_pCurState == st) return true;

		return false;
	}

	bool HandleMessage(const Telegram& msg)
	{
		// 현재 상태가 유효하고 메시지를 처리할 수 있는지 알아본다
		if (m_pCurState && m_pCurState->OnMessage(m_pOwner, msg)) return true;
		// 처리할 수 없고 전역 상태가 설치되어 있다면, 메시지를 전역 상태로 보낸다
		if (m_pGlobalState && m_pGlobalState->OnMessage(m_pOwner, msg)) return true;
	
		return false;
	}
};