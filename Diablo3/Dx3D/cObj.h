#pragma once

class cMtlTex;
class cCamera;

class cObj : public cObject
{
private:
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vPosition, Position);
	SYNTHESIZE_PASS_BY_REF(LPD3DXMESH, m_pMesh, Mesh);
	SYNTHESIZE_PASS_BY_REF(std::vector<cMtlTex*>, m_vecMtl, Mtl);

	SYNTHESIZE_PASS_BY_REF(std::vector<vector<ST_PC_VERTEX>>, m_vecBound, BoundBox);
	SYNTHESIZE(D3DXMATRIX*, m_matWorld, WorldTM);
	SYNTHESIZE(string, m_sSumNailName, SumNailName);
	SYNTHESIZE(string, m_sObjName, ObjName);

	cCamera*			m_Camera;
	LPD3DXEFFECT		m_pEffect;
	LPDIRECT3DTEXTURE9	m_nTexture;
	
public:
	cObj();
	virtual ~cObj();

	void SetUp(char* szFileName, char* szFolderName);
	void Render();

	float GetRayDistance(int index, std::vector<ST_PC_VERTEX> vecVertex);

};

