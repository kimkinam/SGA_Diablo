#pragma once
#include "cMonster.h"
class cFetish :
	public cMonster
{
public:
	cFetish();
	virtual ~cFetish();

	virtual void Setup(D3DXVECTOR3* vLookAt = NULL);
	virtual void Setup(ST_SAVEOBJECT wObj);
	virtual void Update();
	virtual void Render();

};

