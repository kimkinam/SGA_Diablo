#pragma once
#include "cObject.h"

class cGameObject;
class iActionDelegate;

class cAction : public cObject
{
protected:
	SYNTHESIZE(D3DXVECTOR3, m_vDirection, Directoin);
	SYNTHESIZE(float, m_fPassedTime, PassedTime);
	SYNTHESIZE(float, m_fActionTime, ActionTime);
	SYNTHESIZE(float, m_fDistance, Distance);
	SYNTHESIZE(cGameObject*, m_pTarget, Target);
	SYNTHESIZE(iActionDelegate*, m_pDelegate, Delegate);


public:
	cAction(void);
	virtual ~cAction(void);

	virtual void Start();
	virtual void Update();

	static cAction* Create();
};

