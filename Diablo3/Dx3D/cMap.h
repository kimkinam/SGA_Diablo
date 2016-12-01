#pragma once

class cMtlTex;

class cMap : public cObject
{
private:
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vPosition, Position);

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

	SYNTHESIZE(D3DXMATRIX, m_matWorld, WorldTM);
	SYNTHESIZE(string, m_sSumNailName, SumNailName);
	SYNTHESIZE(string, m_sObjName, ObjName);



public:
	cMap();
	virtual ~cMap();

	void Setup(char* szFileName, char* szForlderName);
	void Render();
	void RenerComplete();
	void RenderBoundBox();
	void SetLocalBoundBox();

	void SetRefMtl(cMap* map);
	void SetRefObj(cMap* map);
	

};

