#pragma once
class cActionTrace : public cAction
{
private:
	SYNTHESIZE(D3DXVECTOR3*, m_tFrom, From);
	SYNTHESIZE(D3DXVECTOR3*, m_tTo, To);
	SYNTHESIZE(D3DXVECTOR3*, m_vPosition, Position);
	SYNTHESIZE(float, m_fSpeed, Speed);
<<<<<<< HEAD
	//������Ÿ�
	SYNTHESIZE(float, m_fTraceRange, TraceRange);
	//���ݻ�Ÿ�
	SYNTHESIZE(float, m_fAttackRange, AttackRange);
	SYNTHESIZE(bool, m_bIsAtk, IsAtk);

	//Ÿ�ٰ��� �Ÿ�
	SYNTHESIZE(float, m_fDistance, Distance);
=======
	SYNTHESIZE(float, m_fDistance, Distance);
	SYNTHESIZE(float, m_fTraceTime, TraceTime);
>>>>>>> 65ab56b83d0f8d2a0d2f65015c3637a9a04789e0

public:
	cActionTrace();
	virtual~cActionTrace();

	static cAction* Create();

	virtual void Start() override;
	virtual void Update() override;

};

