#pragma once

class cMtlTex;

class cMap : public cObject
{
private:
	

	//�� ��¥ �Ž�
	SYNTHESIZE_PASS_BY_REF(LPD3DXMESH, m_pComMesh, ComMesh);
	SYNTHESIZE_PASS_BY_REF(std::vector<cMtlTex*>, m_vecComMtl, ComMtl);

	//������� �� �Ž�
	SYNTHESIZE_PASS_BY_REF(LPD3DXMESH, m_pMesh, Mesh);
	SYNTHESIZE_PASS_BY_REF(std::vector<cMtlTex*>, m_vecMtl, Mtl);

	//�� ����� �� �ִ� �Ž�
	SYNTHESIZE_PASS_BY_REF(std::vector<LPD3DXMESH>, m_vecHiddenObj, HiddenObj);
	SYNTHESIZE_PASS_BY_REF(std::vector<cMtlTex*>, m_vecHiddenMtl, HiddenMtl);

	//�ٿ�� �ڽ�
	SYNTHESIZE_PASS_BY_REF(std::vector<cBoundBox*>, m_vecBoundBox, BoundBox);
	SYNTHESIZE_PASS_BY_REF(std::vector<bool>, m_vecHiddenDraw, HiddenDraw);

	SYNTHESIZE_PASS_BY_REF(D3DXMATRIX, m_matLocal, LocalTM);
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIX, m_matWorld, WorldTM);
	SYNTHESIZE(D3DXVECTOR3, m_vPosition, Position);
	SYNTHESIZE(D3DXVECTOR3, m_vScale, Scale);
	SYNTHESIZE(D3DXVECTOR3, m_vDirection, Direction);
	SYNTHESIZE(D3DXVECTOR3, m_vUp, UpVector);
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
	void SetLocalBoundBox(D3DXMATRIX* mat);

	void CloneMap(cMap* map);

	void SetRefMtl(cMap* map);
	void SetRefObj(cMap* map);

	void SetNewDirection(D3DXVECTOR3 vDirection);
	void ChangeLocal();

};

