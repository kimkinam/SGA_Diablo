#pragma once
#include "cObject.h"
#include "cAction.h"
#include "iActionDelegate.h"

class cGameObject : public cObject, public iActionDelegate
{
protected:
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vPosition, Position);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vDirection, Direction);
	SYNTHESIZE_ADD_REF(cAction*, m_pAction, Action);
	SYNTHESIZE(float, m_fAngle, Angle);


public:
	cGameObject();
	virtual ~cGameObject();
	
	virtual void OnActionFinish(cAction* pSender) override;

	D3DXVECTOR3* GetPtPosition(){ return &m_vPosition; }
	
};

