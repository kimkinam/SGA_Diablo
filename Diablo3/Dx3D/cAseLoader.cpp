#include "stdafx.h"
#include "cAseLoader.h"
#include "Asciitok.h"
#include "cMtlTex.h"
#include "cNode.h"


cAseLoader::cAseLoader()
	: m_fp(NULL)
	, m_vMinimize(0, 0, 0)
	, m_vMaximize(0, 0, 0)
{
	memset(&m_stFrameInfo, 0, sizeof(ST_FRAME_INFO));

}


cAseLoader::~cAseLoader()
{
}

ST_FRAME_INFO cAseLoader::Load(IN const char * szFilename, OUT cNode** pRoot)
{
	fopen_s(&m_fp, szFilename, "r");

	if (!m_fp) return m_stFrameInfo;

	while(!feof(m_fp))
	{
		char* szToken = Token();

		if (szToken == NULL) break;
		if (strcmp(szToken, ID_SCENE) == 0)
			MakeScene(m_stFrameInfo);
		if (strcmp(szToken, ID_MATERIAL_LIST) == 0)
			MakeMaterialList();
		else if (strcmp(szToken, ID_GEOMETRY) == 0)
		{
			cNode* pNode = MakeObject();
			if (*pRoot == NULL) *pRoot = pNode;
		}
	}

	ST_BOUNDING_SPHERE stBounding(D3DXVECTOR3(0, 0, 0), 0.0f);

	stBounding.radius = D3DXVec3Length(&(m_vMaximize - m_vMinimize)) / 2;
	stBounding.center = (m_vMaximize + m_vMinimize)/2;

	D3DXVec3TransformCoord(&stBounding.center, &stBounding.center, &(*pRoot)->GetWorldTM());
	(*pRoot)->SetBounding(stBounding);

	fclose(m_fp);

	for each (auto p in m_vecMtlTex)
	{
		SAFE_RELEASE(p);
	}

	return m_stFrameInfo;
}


void cAseLoader::MakeMaterialList()
{
	char* szToken = Token();

	int nCnt = 0;

	if (strcmp(szToken, "{") == 0) nCnt++;

	while (nCnt > 0)
	{
		szToken = Token();
		if		(strcmp(szToken, "{") == 0) nCnt++;
		else if (strcmp(szToken, "}") == 0) nCnt--;
		else if (strcmp(szToken, ID_MATERIAL_COUNT) == 0)
		{
			for each(auto p in m_vecMtlTex)
			{
				SAFE_RELEASE(p);
			}

			m_vecMtlTex.resize(atoi(Token()));
		}
		else if (strcmp(szToken, ID_MATERIAL) == 0)
		{
			m_vecMtlTex[(atoi(Token()))] = new cMtlTex;
			
			MakeMaterial(m_vecMtlTex[atoi(szToken)]);
		}
	}
}

void cAseLoader::MakeMaterial(cMtlTex* pMtlTex)
{
	D3DMATERIAL9 Mtl;
	ZeroMemory(&Mtl, sizeof(Mtl));

	char* szToken = Token();

	int nCnt = 0;

	if (strcmp(szToken, "{") == 0) nCnt++;

	while (nCnt > 0)
	{
		szToken = Token();
		if		(strcmp(szToken, "{") == 0) nCnt++;
		else if (strcmp(szToken, "}") == 0) nCnt--;
		else if (strcmp(szToken, ID_AMBIENT) == 0)
		{
			Mtl.Ambient.r = (float)atof(Token());
			Mtl.Ambient.g = (float)atof(Token());
			Mtl.Ambient.b = (float)atof(Token());
			Mtl.Ambient.a = 1.0f;
		}
		else if (strcmp(szToken, ID_DIFFUSE) == 0)
		{
			Mtl.Diffuse.r = (float)atof(Token());
			Mtl.Diffuse.g = (float)atof(Token());
			Mtl.Diffuse.b = (float)atof(Token());
			Mtl.Diffuse.a = 1.0f;
		}
		else if (strcmp(szToken, ID_SPECULAR) == 0)
		{
			Mtl.Specular.r = (float)atof(Token());
			Mtl.Specular.g = (float)atof(Token());
			Mtl.Specular.b = (float)atof(Token());
			Mtl.Specular.a = 1.0f;
		}
		else if (strcmp(szToken, ID_MAP_DIFFUSE) == 0)
		{
			char* szToken = Token();

			int nCnt = 0;

			if (strcmp(szToken, "{") == 0) nCnt++;

			while (nCnt > 0)
			{
				szToken = Token();
				if		(strcmp(szToken, "{") == 0) nCnt++;
				else if (strcmp(szToken, "}") == 0) nCnt--;
				else if (strcmp(szToken, ID_BITMAP) == 0)
				{
					szToken = Token();
					pMtlTex->SetTexture(g_pTextureManager->GetTexture(szToken));
				}
			}
		}
	}

	pMtlTex->SetMtl(Mtl);
}

cNode* cAseLoader::MakeObject()
{
	cNode* pNode = new cNode;

	char* szToken = Token();

	int nCnt = 0;

	if (strcmp(szToken, "{") == 0) nCnt++;

	while (nCnt > 0)
	{
		szToken = Token();
		if (strcmp(szToken, "{") == 0) nCnt++;
		else if (strcmp(szToken, "}") == 0) nCnt--;
		else if (strcmp(szToken, ID_NODE_NAME) == 0)
			m_mapNode[Token()] = pNode;
		else if (strcmp(szToken, ID_NODE_PARENT) == 0)
			m_mapNode[Token()]->AddChild(pNode);
		else if (strcmp(szToken, ID_NODE_TM) == 0)
			MakeNodeTM(pNode);
		else if (strcmp(szToken, ID_MESH) == 0)
			MakeMesh(pNode);
		else if (strcmp(szToken, ID_TM_ANIMATION) == 0)
			MakeTmAnimation(pNode);
		else if (strcmp(szToken, ID_MATERIAL_REF) == 0)
			pNode->SetMtlTex(m_vecMtlTex[atoi(Token())]);
	}

	return pNode;
}

void cAseLoader::MakeNodeTM(OUT cNode* pNode)
{
	D3DXMATRIXA16 mat;
	D3DXMatrixIdentity(&mat);

	char* szToken = Token();

	int nCnt = 0;

	if (strcmp(szToken, "{") == 0) nCnt++;

	while (nCnt > 0)
	{
		szToken = Token();
		if		(strcmp(szToken, "{") == 0) nCnt++;
		else if (strcmp(szToken, "}") == 0) nCnt--;
		else if (strcmp(szToken, ID_TM_ROW0) == 0)
		{
			mat._11 = atof(Token());
			mat._13 = atof(Token());
			mat._12 = atof(Token());
			mat._14 = 0.0f;
		}
		else if (strcmp(szToken, ID_TM_ROW1) == 0)
		{
			mat._31 = atof(Token());
			mat._33 = atof(Token());
			mat._32 = atof(Token());
			mat._34 = 0.0f;
		}
		else if (strcmp(szToken, ID_TM_ROW2) == 0)
		{
			mat._21 = atof(Token());
			mat._23 = atof(Token());
			mat._22 = atof(Token());
			mat._24 = 0.0f;
		}
		else if (strcmp(szToken, ID_TM_ROW3) == 0)
		{
			mat._41 = atof(Token());
			mat._43 = atof(Token());
			mat._42 = atof(Token());
			mat._44 = 1.0f;
		}
	}


	pNode->SetWorldTM(mat);
}

void cAseLoader::MakeMesh(OUT cNode* pNode)
{
	std::vector<D3DXVECTOR3>	vecV;
	std::vector<D3DXVECTOR2>	vecVT;
	std::vector<ST_PNT_VERTEX>	vecVertex;
	
	char* szToken = Token();

	int nCnt = 0;

	if (strcmp(szToken, "{") == 0) nCnt++;

	while (nCnt > 0)
	{
		szToken = Token();
		if		(strcmp(szToken, "{") == 0) nCnt++;
		else if (strcmp(szToken, "}") == 0) nCnt--;
		else if (strcmp(szToken, ID_MESH_NUMVERTEX) == 0)
			vecV.resize(atoi(Token()));
		else if (strcmp(szToken, ID_MESH_NUMFACES) == 0)
			vecVertex.resize(atoi(Token()) * 3);
		else if (strcmp(szToken, ID_MESH_VERTEX_LIST) == 0)
			MakeMeshVertexList(vecV);
		else if (strcmp(szToken, ID_MESH_FACE_LIST) == 0)
			MakeMeshFaceList(vecV, vecVertex);
		else if (strcmp(szToken, ID_MESH_NUMTVERTEX) == 0)
			vecVT.resize(atoi(Token()));
		else if (strcmp(szToken, ID_MESH_TVERTLIST) == 0)
			MakeMeshTVertextList(vecVT);
		else if (strcmp(szToken, ID_MESH_TFACELIST) == 0)
			MakeMeshTFaceList(vecVT, vecVertex);
		else if (strcmp(szToken, ID_MESH_NORMALS) == 0)
			MakeMeshNormals(vecVertex);
	}

	D3DXMATRIXA16 matInvWorld;
	
	D3DXMatrixInverse(&matInvWorld, 0, &pNode->GetWorldTM());

	for (size_t i = 0; i < vecVertex.size(); ++i)
	{
		D3DXVec3TransformCoord(&vecVertex[i].p, &vecVertex[i].p, &matInvWorld);
		D3DXVec3TransformNormal(&vecVertex[i].n, &vecVertex[i].n, &matInvWorld);

		D3DXVec3Minimize(&m_vMinimize, &m_vMinimize, &vecVertex[i].p);
		D3DXVec3Maximize(&m_vMaximize, &m_vMaximize, &vecVertex[i].p);
	}

	pNode->SetVertex(vecVertex);
}

void cAseLoader::MakeTmAnimation(OUT cNode * pNode)
{
	std::vector<ST_CONTROL_POS> vecPos;
	std::vector<ST_CONTROL_ROT> vecRot;

	char* szToken = Token();

	int nCnt = 0;

	if (strcmp(szToken, "{") == 0) nCnt++;

	while (nCnt > 0)
	{
		szToken = Token();
		if (strcmp(szToken, "{") == 0) nCnt++;
		else if (strcmp(szToken, "}") == 0) nCnt--;
		else if (strcmp(szToken, ID_POS_TRACK) == 0)
			MakeControlPosTrack(vecPos);
		else if (strcmp(szToken, ID_ROT_TRACK) == 0)
			MakeControlRotTrack(vecRot);
	}
	pNode->SetPos(vecPos);
	pNode->SetRot(vecRot);

}

void cAseLoader::MakeControlPosTrack(OUT std::vector<ST_CONTROL_POS>& vecPos)
{
	ST_CONTROL_POS p;

	int nLevel = 0;

	do
	{
		char* szToken = Token();
		if (strcmp(szToken, "{") == 0)
		{
			++nLevel;
		}
		else if (strcmp(szToken, "}") == 0)
		{
			--nLevel;
		}
		else if (strcmp(szToken, ID_POS_SAMPLE) == 0)
		{
			p.keyFrame = atoi(Token());
			p.v.x = (float)atof(Token());
			p.v.z = (float)atof(Token());
			p.v.y = (float)atof(Token());
			vecPos.push_back(p);
		}
	} while (nLevel > 0);
}

void cAseLoader::MakeControlRotTrack(OUT std::vector<ST_CONTROL_ROT>& vecRot)
{
	ST_CONTROL_ROT rot;

	D3DXVECTOR3 v;
	float angle;

	int nLevel = 0;

	do
	{
		char* szToken = Token();

		if (strcmp(szToken, "{") == 0) nLevel++;
		else if (strcmp(szToken, "}") == 0) nLevel--;
		else if (strcmp(szToken, ID_ROT_SAMPLE) == 0)
		{
			rot.keyFrame = atoi(Token());
			v.x = (float)atof(Token());
			v.z = (float)atof(Token());
			v.y = (float)atof(Token());
			angle = (float)atof(Token());

			rot.q.x = v.x * sinf(angle / 2.0f);
			rot.q.y = v.y * sinf(angle / 2.0f);
			rot.q.z = v.z * sinf(angle / 2.0f);
			rot.q.w = cosf(angle / 2.0f);

			if (vecRot.empty())
				vecRot.push_back(rot);
			else
			{
				D3DXQuaternionMultiply(&rot.q, &vecRot.back().q, &rot.q);

				vecRot.push_back(rot);
			}
		}
	} while (nLevel > 0);	
}

void cAseLoader::MakeMeshVertexList(OUT std::vector<D3DXVECTOR3>& vecV)
{
	char* szToken = Token();

	int nCnt = 0;

	if (strcmp(szToken, "{") == 0) nCnt++;

	while (nCnt > 0)
	{
		szToken = Token();
		if		(strcmp(szToken, "{") == 0) nCnt++;
		else if (strcmp(szToken, "}") == 0) nCnt--;
		else if (strcmp(szToken, ID_MESH_VERTEX) == 0)
		{
			int idx = atoi(Token());
			vecV[idx].x = atof(Token());
			vecV[idx].z = atof(Token());
			vecV[idx].y = atof(Token());
		}
	}
}

void cAseLoader::MakeMeshFaceList(IN std::vector<D3DXVECTOR3>& vecV, OUT std::vector<ST_PNT_VERTEX>& vecVertex)
{
	char* szToken = Token();

	int nCnt = 0;

	if (strcmp(szToken, "{") == 0) nCnt++;

	while (nCnt > 0)
	{
		szToken = Token();
		if		(strcmp(szToken, "{") == 0) nCnt++;
		else if (strcmp(szToken, "}") == 0) nCnt--;
		else if (strcmp(szToken, ID_MESH_FACE) == 0)
		{
			int idx = atoi(Token());
			Token();
			vecVertex[idx * 3 + 0].p = vecV[atoi(Token())];
			Token();
			vecVertex[idx * 3 + 2].p = vecV[atoi(Token())];
			Token();
			vecVertex[idx * 3 + 1].p = vecV[atoi(Token())];
		}
	}
}

void cAseLoader::MakeMeshTVertextList(OUT std::vector<D3DXVECTOR2>& vecVT)
{
	char* szToken = Token();

	int nCnt = 0;

	if (strcmp(szToken, "{") == 0) nCnt++;

	while (nCnt > 0)
	{
		szToken = Token();
		if		(strcmp(szToken, "{") == 0) nCnt++;
		else if (strcmp(szToken, "}") == 0) nCnt--;
		else if (strcmp(szToken, ID_MESH_TVERT) == 0)
		{
			int idx = atoi(Token());
			vecVT[idx].x = atof(Token());
			vecVT[idx].y = 1.0f - atof(Token());
		}
	}

}

void cAseLoader::MakeMeshTFaceList(IN std::vector<D3DXVECTOR2>& vecVT, OUT std::vector<ST_PNT_VERTEX>& vecVerext)
{
	char* szToken = Token();

	int nCnt = 0;

	if (strcmp(szToken, "{") == 0) nCnt++;

	while (nCnt > 0)
	{
		szToken = Token();
		if		(strcmp(szToken, "{") == 0) nCnt++;
		else if (strcmp(szToken, "}") == 0) nCnt--;
		else if (strcmp(szToken, ID_MESH_TFACE) == 0)
		{
			int idx = atoi(Token());
			vecVerext[idx * 3 + 0].t = vecVT[atoi(Token())];
			vecVerext[idx * 3 + 2].t = vecVT[atoi(Token())];
			vecVerext[idx * 3 + 1].t = vecVT[atoi(Token())];
		}
	}
}

void cAseLoader::MakeMeshNormals(OUT std::vector<ST_PNT_VERTEX>& vecVertex)
{
	char* szToken = Token();

	int nCnt = 0;
	int nFaceIndex = 0;
	int nLineCnt = 0;

	if (strcmp(szToken, "{") == 0) nCnt++;

	while (nCnt > 0)
	{
		szToken = Token();
		if		(strcmp(szToken, "{") == 0) nCnt++;
		else if (strcmp(szToken, "}") == 0) nCnt--;
		else if (strcmp(szToken, ID_MESH_FACENORMAL) == 0)
		{
			nFaceIndex = atoi(Token());
			nLineCnt = 0;
		}
		else if (strcmp(szToken, ID_MESH_VERTEXNORMAL) == 0)
		{
			int idx = atoi(Token());
			
			D3DXVECTOR3 n;
			n.x = atof(Token());
			n.z = atof(Token());
			n.y = atof(Token());
			vecVertex[nFaceIndex * 3 + nLineCnt].n = n;

			if (nLineCnt == 0) nLineCnt = 2;
			else if (nLineCnt == 2) nLineCnt = 1;
			else nLineCnt = 0;
		}
	}

}

//----------------------------------------------------------------------------

char* cAseLoader::Token()
{
	int Idx = 0;
	bool IsQuatation = false;

	while (true)
	{
		char c = fgetc(m_fp);

		if (feof(m_fp)) break;
		if (c == '\"')
		{
			if (IsQuatation) break;
			IsQuatation = true;
			continue;
		}
		if (!IsQuatation && c < 33)
		{
			if (Idx == 0) continue;
			else break;
		}
		m_szBuf[Idx++] = c;
	}

	if (Idx == 0) return NULL;

	m_szBuf[Idx] = '\0';

	return m_szBuf;
}

void cAseLoader::MakeScene(OUT ST_FRAME_INFO& fInfo)
{
	char* szToken = Token();

	int nCnt = 0;

	if (strcmp(szToken, "{") == 0) nCnt++;

	while (nCnt > 0)
	{
		szToken = Token();
		if (strcmp(szToken, "{") == 0) nCnt++;
		else if (strcmp(szToken, "}") == 0) nCnt--;
		else if (strcmp(szToken, ID_FIRSTFRAME) == 0)
			fInfo.nFistFrame = atoi(Token());
		else if (strcmp(szToken, ID_LASTFRAME) == 0)
			fInfo.nLastFrame = atoi(Token());
		else if (strcmp(szToken, ID_FRAMESPEED) == 0)
			fInfo.nFrameSpeed = atoi(Token());
		else if (strcmp(szToken, ID_TICKSPERFRAME) == 0)
			fInfo.nTickPerFrame = atoi(Token());
	}
}
