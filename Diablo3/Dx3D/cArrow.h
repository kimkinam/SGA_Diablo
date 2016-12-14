#pragma once
#include "cGameObject.h"

class cMtlTex;

class cArrow :
	public cGameObject
{
private:
	SYNTHESIZE_PASS_BY_REF(LPD3DXMESH, m_pArrowMesh, ArrowMesh);
	SYNTHESIZE_PASS_BY_REF(std::vector<cMtlTex*>, m_vecMtl, Mtl);
public:
	cArrow();
	virtual ~cArrow();

	//static UINT GetNextID() { return m_INextID; }

	virtual void Setup(D3DXVECTOR3* vLook = NULL);
	virtual void Update();
	virtual void Render();
};

