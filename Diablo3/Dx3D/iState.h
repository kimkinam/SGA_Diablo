#pragma once

template <class T>
class iState
{
public:
	virtual ~iState(){ }

	//���¿� ����
	virtual void Enter(T* object) PURE;
	//���¿� �����ؼ� ����
	virtual void Execute(T* object) PURE;
	//���¿��� ����
	virtual void Exit(T* object) PURE;
};
