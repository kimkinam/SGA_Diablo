#pragma once

class cMtlTex;

class cObj : public cObject
{
private:
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vPosition, Position);

	SYNTHESIZE_PASS_BY_REF(LPD3DXMESH, m_pMesh, Mesh);
	SYNTHESIZE_PASS_BY_REF(std::vector<cMtlTex*>, m_vecMtl, Mtl);

	//SYNTHESIZE_PASS_BY_REF(std::vector<LPD3DXMESH>, m_vecHiddenObj, HiddenObj);
	//SYNTHESIZE_PASS_BY_REF(std::vector<bool>, m_vecHiddenDraw, HiddenDraw);

	//SYNTHESIZE_PASS_BY_REF(std::vector<cMtlTex*>, m_vecHiddenMtl, HiddenMtl);

	SYNTHESIZE_PASS_BY_REF(std::vector<vector<ST_PC_VERTEX>>, m_vecBound, BoundBox);
	SYNTHESIZE(D3DXMATRIX*, m_matWorld, WorldTM);
	SYNTHESIZE(string, m_sSumNailName, SumNailName);
	SYNTHESIZE(string, m_sObjName, ObjName);

	LPD3DXEFFECT m_pEffect;

public:
	cObj();
	virtual ~cObj();

	void SetUp(char* szFileName, char* szFolderName);
	void Render();

	float GetRayDistance(int index, std::vector<ST_PC_VERTEX> vecVertex);

};

