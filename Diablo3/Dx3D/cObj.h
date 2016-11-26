#pragma once

class cMtlTex;

class cObj
{
private:
	SYNTHESIZE(D3DXMATRIX*, m_matWorld, WorldTM);
	SYNTHESIZE(LPD3DXMESH, m_pMesh, Mesh);
	SYNTHESIZE_PASS_BY_REF(std::vector<cMtlTex*>, m_vecMtl, Mtl);
public:
	cObj();
	~cObj();

	void SetUp(char* szFileName, char* szFolderName);
	void Render();

};

