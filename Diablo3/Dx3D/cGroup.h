#pragma once
#include "cMtlTex.h"

class cGroup
{
private:
	SYNTHESIZE_PASS_BY_REF(std::vector<ST_PNT_VERTEX>, m_vecVertex, Vertex);
	SYNTHESIZE_ADD_REF(cMtlTex*, m_pMtlTex, MtlTex);
	SYNTHESIZE_PASS_BY_REF(LPDIRECT3DVERTEXBUFFER9, m_pVB, VB);

public:
	cGroup(void);
	~cGroup(void);

	void Render();
};

