#pragma once
class cActionTrace : public cAction
{
private:
	SYNTHESIZE(D3DXVECTOR3*, m_tFrom, From);
	SYNTHESIZE(D3DXVECTOR3*, m_tTo, To);
	SYNTHESIZE(D3DXVECTOR3*, m_vPosition, Position);
	SYNTHESIZE(float, m_fSpeed, Speed);
	SYNTHESIZE(float, m_fDistance, Distance);
	SYNTHESIZE(float, m_fTraceTime, TraceTime);

public:
	cActionTrace();
	virtual~cActionTrace();

	static cAction* Create();
	
	virtual void Start() override;
	virtual void Update() override;

};

