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
		assert(pNewState && "�����Ϸ��� ���°� NULL�Դϴ�.");

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
		// ���� ���°� ��ȿ�ϰ� �޽����� ó���� �� �ִ��� �˾ƺ���
		if (m_pCurState && m_pCurState->OnMessage(m_pOwner, msg)) return true;
		// ó���� �� ���� ���� ���°� ��ġ�Ǿ� �ִٸ�, �޽����� ���� ���·� ������
		if (m_pGlobalState && m_pGlobalState->OnMessage(m_pOwner, msg)) return true;
	
		return false;
	}
};