#pragma once

class cMtlTex;

class cMap : public cObject
{
private:
	

	//맵 통짜 매쉬
	SYNTHESIZE_PASS_BY_REF(LPD3DXMESH, m_pComMesh, ComMesh);
	SYNTHESIZE_PASS_BY_REF(std::vector<cMtlTex*>, m_vecComMtl, ComMtl);

	//사라지고 난 매쉬
	SYNTHESIZE_PASS_BY_REF(LPD3DXMESH, m_pMesh, Mesh);
	SYNTHESIZE_PASS_BY_REF(std::vector<cMtlTex*>, m_vecMtl, Mtl);

	//맵 사라질 수 있는 매쉬
	SYNTHESIZE_PASS_BY_REF(std::vector<LPD3DXMESH>, m_vecHiddenObj, HiddenObj);
	SYNTHESIZE_PASS_BY_REF(std::vector<cMtlTex*>, m_vecHiddenMtl, HiddenMtl);

	//바운드 박스
	SYNTHESIZE_PASS_BY_REF(std::vector<cBoundBox*>, m_vecBoundBox, BoundBox);
	SYNTHESIZE_PASS_BY_REF(std::vector<bool>, m_vecHiddenDraw, HiddenDraw);

	SYNTHESIZE(D3DXMATRIX, m_matLocal, LocalTM);
	SYNTHESIZE(D3DXMATRIX, m_matWorld, WorldTM);
	SYNTHESIZE(D3DXVECTOR3, m_vPosition, Position);
	SYNTHESIZE(D3DXVECTOR3, m_vScale, Scale);
	SYNTHESIZE(D3DXVECTOR3, m_vForward, Forward);
	SYNTHESIZE(D3DXVECTOR3, m_vUp, Up);
	SYNTHESIZE(D3DXVECTOR3, m_vRight, Right);

	SYNTHESIZE(string, m_sSumNailName, SumNailName);
	SYNTHESIZE(string, m_sObjName, ObjName);
	SYNTHESIZE(string, m_sFolderName, FolderName);
	

public:
	cMap();
	virtual ~cMap();

	void Setup(char* szFileName, char* szForlderName);
	void Setup(ST_SAVEOBJECT wObj);
	void Render();
	void RenerComplete();
	void RenderBoundBox();
	void SetLocalBoundBox(D3DXVECTOR3* vPos);

	void CloneMap(cMap* map);

	void SetRefMtl(cMap* map);
	void SetRefObj(cMap* map);
	

};

