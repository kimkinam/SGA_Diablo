#pragma once

#include "cAction.h"

class cActionMove : public cAction
{
private:
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vFrom, From);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vTo, To);
	SYNTHESIZE(float, m_fSpeed, Speed);

public:
	cActionMove();
	virtual ~cActionMove();

	static cAction* Create();

	// cAction override
	virtual void Start() override;
	virtual void Update() override;
};

