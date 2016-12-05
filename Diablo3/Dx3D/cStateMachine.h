#pragma once

template <class T>
class cStateMachine
{
private:
	typedef iState<T>	State;
private:
	T*	m_pOwner;
	SYNTHESIZE(State*, m_pCurState, CurState);
	SYNTHESIZE(State*, m_pPreState, PreState);
	SYNTHESIZE(State*, m_pGlobalState, GlobalState);

public:
	cStateMachine(T* owner)
	{
		m_pOwner = owner;
		m_pCurState = m_pPreState = m_pGlobalState = NULL;
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
};


