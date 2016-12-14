#pragma once
#include "cMonster.h"

class cArrow;

class cSkeletonArcher :
	public cMonster
{
private:
	SYNTHESIZE_PASS_BY_REF(std::vector<cArrow*>, m_vecArrow, Arrow);
	SYNTHESIZE(bool, m_bIsFire, IsFire);
public:
	cSkeletonArcher();
	virtual ~cSkeletonArcher();

	virtual void Setup(D3DXVECTOR3* vLookAt = NULL);
	virtual void Setup(ST_SAVEOBJECT wObj);
	virtual void Update();
	virtual void Render();

	virtual void OnActionFinish(cAction* pSender) override;
};

