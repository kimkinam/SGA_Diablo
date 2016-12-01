#pragma once

class cBoundBox
{
private:
	SYNTHESIZE_PASS_BY_REF(vector<ST_PC_VERTEX>, m_vecVertex, Vertex);
	SYNTHESIZE(bool, m_bIsDraw, IsDraw);
	SYNTHESIZE(D3DXMATRIX, m_matWorld, WorldTM);

public:
	cBoundBox();
	~cBoundBox();

	void Setup(D3DXVECTOR3 vMin, D3DXVECTOR3 vMax, D3DXMATRIX* pMat);
	void Render();
	bool GetRayDistance(D3DXVECTOR3 vRayPosition, D3DXVECTOR3 vDirection, float& distance);
};

