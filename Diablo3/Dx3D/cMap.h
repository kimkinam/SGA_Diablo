#pragma once

class cMtlTex;

class cMap : public cObject
{
private:
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vPosition, Position);

	//�� ��¥ �Ž�
	SYNTHESIZE_PASS_BY_REF(LPD3DXMESH, m_pMesh, Mesh);
	SYNTHESIZE_PASS_BY_REF(std::vector<cMtlTex*>, m_vecMtl, Mtl);

	//�� ����� �� �ִ� �Ž�
	SYNTHESIZE_PASS_BY_REF(std::vector<LPD3DXMESH>, m_vecHiddenObj, HiddenObj);
	SYNTHESIZE_PASS_BY_REF(std::vector<cMtlTex*>, m_vecHiddenMtl, HiddenMtl);
	SYNTHESIZE_PASS_BY_REF(std::vector<bool>, m_vecHiddenDraw, HiddenDraw);
	
	//�ٿ�� �ڽ�
	SYNTHESIZE_PASS_BY_REF(std::vector<vector<ST_PC_VERTEX>>, m_vecBound, BoundBox);

	SYNTHESIZE(D3DXMATRIX, m_matWorld, WorldTM);
	SYNTHESIZE(string, m_sSumNailName, SumNailName);
	SYNTHESIZE(string, m_sObjName, ObjName);

public:
	cMap();
	~cMap();

	void Setup(char* szFileName, char* szForlderName);
	void Render();
	
};

