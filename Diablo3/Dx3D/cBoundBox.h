#pragma once

class cBoundBox
{
private:
	SYNTHESIZE_PASS_BY_REF(vector<ST_PC_VERTEX>, m_vecVertex, Vertex);
	SYNTHESIZE(bool, m_bIsDraw, IsDraw);
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIX, m_matLocal, LocalTM);
	SYNTHESIZE(D3DXMATRIX, m_matWorld, WorldTM);
	SYNTHESIZE(D3DXVECTOR3, m_vPosition, Position);
	SYNTHESIZE(D3DXVECTOR3, m_vDirection, Direction);
	SYNTHESIZE(D3DXVECTOR3, m_vUp, Up);
	SYNTHESIZE(D3DXVECTOR3, m_vRight, Right);

public:
	cBoundBox();
	~cBoundBox();

	void Setup(D3DXVECTOR3 vMin, D3DXVECTOR3 vMax, D3DXMATRIX* mat);
	void Setup(D3DXVECTOR3* vMin, D3DXVECTOR3* vMax, D3DXMATRIX* pMat);
	void Render();
	bool GetRayDistance(D3DXVECTOR3 vRayPosition, D3DXVECTOR3 vDirection, float& distance);
};

