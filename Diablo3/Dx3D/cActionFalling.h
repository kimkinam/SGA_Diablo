#pragma once

#include "cAction.h"

class iMap;

class cActionFalling : public cAction
{
private:
	D3DXVECTOR3	m_vStartPosition;
	D3DXVECTOR3	m_vPrevPosition;
	D3DXVECTOR3	m_vDirection;
	float		m_fGravity;
	float		m_fVelY;
	float		m_fPrevMapY;
	float		m_fDeltaX;
	float		m_fDeltaZ;

	SYNTHESIZE(iMap*, m_pMap, Map);
	SYNTHESIZE(D3DXVECTOR3, m_vEndPosition, EndPosition);
public:
	cActionFalling();
	virtual ~cActionFalling();

	virtual void Start() override;
	virtual void Update() override;
};

