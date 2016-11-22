#pragma once

class cNode;
class cMtlTex;

class cAseLoader
{
private:
	FILE*							m_fp;
	std::vector<cMtlTex*>			m_vecMtlTex;
	char							m_szBuf[1024];
	std::map<std::string, cNode*>	m_mapNode;
	ST_FRAME_INFO					m_stFrameInfo;
	D3DXVECTOR3						m_vMinimize;
	D3DXVECTOR3						m_vMaximize;

public:
	cAseLoader();
	~cAseLoader();

	ST_FRAME_INFO Load(IN const char* szFilename, OUT cNode** pRoot);

	char* Token();
	void MakeScene(OUT ST_FRAME_INFO& fInfo);
	void MakeMaterialList();
	void MakeMaterial(cMtlTex* pMtlTex);
	cNode* MakeObject();
	void MakeNodeTM(OUT cNode* pNode);
	void MakeMesh(OUT cNode* pNode);
	void MakeTmAnimation(OUT cNode* pNode);
	void MakeControlPosTrack(OUT std::vector<ST_CONTROL_POS>& vecPos);
	void MakeControlRotTrack(OUT std::vector<ST_CONTROL_ROT>& vecRot);
	void MakeMeshVertexList(OUT std::vector<D3DXVECTOR3>& vecV);
	void MakeMeshFaceList(IN std::vector<D3DXVECTOR3>& vecV, OUT std::vector<ST_PNT_VERTEX>& vecVertex);
	void MakeMeshTVertextList(OUT std::vector<D3DXVECTOR2>& vecVT);
	void MakeMeshTFaceList(IN std::vector<D3DXVECTOR2>& vecVT, OUT std::vector<ST_PNT_VERTEX>& vecVerext);
	void MakeMeshNormals(OUT std::vector<ST_PNT_VERTEX>& vecVertex);
};

