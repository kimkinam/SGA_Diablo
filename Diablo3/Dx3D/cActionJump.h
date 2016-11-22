#pragma once

#include "cAction.h"

class iMap;

class cActionJump : public cAction
{
private:
	D3DXVECTOR3	m_vStartPosition;
	D3DXVECTOR3	m_vPrevPosition;
	D3DXVECTOR3	m_vDirection;
	float		m_fGravity;
	float		m_fJumpPower;
	float		m_fPrevMapY;
	float		m_fTargetX;
	float		m_fTargetZ;
	float		m_fTargetY;

	SYNTHESIZE(iMap*, m_pMap, Map);
	SYNTHESIZE(D3DXVECTOR3, m_vEndPosition, EndPosition);
	SYNTHESIZE(iActionDelegate*, m_pDelegate, Delegate);


public:
	cActionJump(void);
	virtual ~cActionJump(void);

	virtual void Start() override;
	virtual void Update() override;

};

