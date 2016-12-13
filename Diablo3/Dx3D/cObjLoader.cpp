#include "StdAfx.h"
#include "cObjLoader.h"
#include "cMtlTex.h"
#include "cGroup.h"

cObjLoader::cObjLoader(void)
{
	m_sPath = "";
}

cObjLoader::~cObjLoader(void)
{
}

void cObjLoader::Load(IN char* szFilename, IN char* szFolderName,OUT std::vector<cGroup*>& vecGroup, IN D3DXMATRIX* pmat)
{
	m_mapMtlTex.clear();

	m_sPath = szFolderName;

	std::vector<D3DXVECTOR3> vecV;
	std::vector<D3DXVECTOR3> vecVN;
	std::vector<D3DXVECTOR2> vecVT;
	std::vector<ST_PNT_VERTEX> vecVertex;
	std::string sMtlName;

	FILE* fp = NULL;
	string sFullpath = m_sPath + szFilename;

	fopen_s(&fp, sFullpath.c_str(), "r");
	while (!feof(fp))
	{
		char szBuf[1024] = { '\0', };

		fgets(szBuf, 1024, fp);

		if (strlen(szBuf) == 0)
			continue;

		if (szBuf[0] == '#')
		{
			continue;
		}
		else if (szBuf[0] == 'm')
		{
			char szMtlPath[1024];
			sscanf_s(szBuf, "%*s %s", szMtlPath, 1024);
			LoadMtlLib(szMtlPath);
		}
		else if (szBuf[0] == 'g')
		{
			if (!vecVertex.empty())
			{
				cGroup* pGroup = new cGroup;
				pGroup->SetVertex(vecVertex);
				pGroup->SetMtlTex(m_mapMtlTex[sMtlName]);
				vecGroup.push_back(pGroup);
				vecVertex.clear();
			}
		}
		else if (szBuf[0] == 'v')
		{
			if (szBuf[1] == 't')
			{
				float u, v;
				sscanf_s(szBuf, "%*s %f %f %*f", &u, &v);
				vecVT.push_back(D3DXVECTOR2(u, v));
			}
			else if (szBuf[1] == 'n')
			{
				float x, y, z;
				sscanf_s(szBuf, "%*s %f %f %f", &x, &y, &z);
				vecVN.push_back(D3DXVECTOR3(x, y, z));
			}
			else
			{
				float x, y, z;
				sscanf_s(szBuf, "%*s %f %f %f", &x, &y, &z);
				vecV.push_back(D3DXVECTOR3(x, y, z));
			}
		}
		else if (szBuf[0] == 'u')
		{
			char szMtlName[1024];
			sscanf_s(szBuf, "%*s %s", szMtlName, 1024);
			sMtlName = std::string(szMtlName);
		}
		else if (szBuf[0] == 'f')
		{
			int aIndex[3][3];
			sscanf_s(szBuf, "%*s %d/%d/%d %d/%d/%d %d/%d/%d",
				&aIndex[0][0], &aIndex[0][1], &aIndex[0][2],
				&aIndex[1][0], &aIndex[1][1], &aIndex[1][2],
				&aIndex[2][0], &aIndex[2][1], &aIndex[2][2]);
			for (int i = 0; i < 3; ++i)
			{
				ST_PNT_VERTEX v;
				v.p = vecV[aIndex[i][0] - 1];
				v.t = vecVT[aIndex[i][1] - 1];
				v.n = vecVN[aIndex[i][2] - 1];

				if (pmat)
				{
					D3DXVec3TransformNormal(&v.n, &vecVN[aIndex[i][2] - 1], pmat);
					D3DXVec3TransformCoord(&v.p, &vecV[aIndex[i][0] - 1], pmat);
				}
				vecVertex.push_back(v);
			}
		}
	}
	fclose(fp);

	for each(auto it in m_mapMtlTex)
	{
		SAFE_RELEASE(it.second);
	}
}

LPD3DXMESH cObjLoader::Load(IN char* szFilename, IN char* szFolderName,OUT std::vector<cMtlTex*>& vecMtlTex, IN D3DXMATRIX* pmat /*= NULL*/)
{
	//m_mapMtlTex.clear();

	m_sPath = szFolderName;

	std::vector<D3DXVECTOR3>	vecV;
	std::vector<D3DXVECTOR3>	vecVN;
	std::vector<D3DXVECTOR2>	vecVT;
	std::vector<ST_PNT_VERTEX>	vecVertex;
	std::vector<DWORD>			vecAttr;
	std::string					sMtlName;

	std::vector<ST_PNT_VERTEX>	vecHiddenVertex;
	std::vector<DWORD>			vecHiddenAttr;

	FILE* fp = NULL;
	string sFullpath = m_sPath + szFilename;

	fopen_s(&fp, sFullpath.c_str(), "r");
	while (!feof(fp))
	{
		char szBuf[1024] = { '\0', };

		fgets(szBuf, 1024, fp);

		if (strlen(szBuf) == 0)
			continue;
		else if (szBuf[0] == '#')
		{
			continue;
		}
			
		else if (szBuf[0] == 'm')
		{
			char szMtlPath[1024];
			sscanf_s(szBuf, "%*s %s", szMtlPath, 1024);
			LoadMtlLib(szMtlPath, vecMtlTex);
		}
		else if (szBuf[0] == 'g')
		{
		}
		else if (szBuf[0] == 'v')
		{
			if (szBuf[1] == 't')
			{
				float u, v;
				sscanf_s(szBuf, "%*s %f %f %*f", &u, &v);
				vecVT.push_back(D3DXVECTOR2(u, 1 - v));
			}
			else if (szBuf[1] == 'n')
			{
				float x, y, z;
				sscanf_s(szBuf, "%*s %f %f %f", &x, &y, &z);
				vecVN.push_back(D3DXVECTOR3(x, y, z));
			}
			else
			{
				float x, y, z;
				sscanf_s(szBuf, "%*s %f %f %f", &x, &y, &z);
				vecV.push_back(D3DXVECTOR3(x, y, z));
			}
		}
		else if (szBuf[0] == 'u')
		{
			char szMtlName[1024];
			sscanf_s(szBuf, "%*s %s", szMtlName, 1024);
			sMtlName = std::string(szMtlName);
		}
		else if (szBuf[0] == 'f')
		{
			int aIndex[3][3];
			sscanf_s(szBuf, "%*s %d/%d/%d %d/%d/%d %d/%d/%d",
				&aIndex[0][0], &aIndex[0][1], &aIndex[0][2],
				&aIndex[1][0], &aIndex[1][1], &aIndex[1][2],
				&aIndex[2][0], &aIndex[2][1], &aIndex[2][2]);
			for (int i = 0; i < 3; ++i)
			{
				ST_PNT_VERTEX v;
				v.p = vecV[aIndex[i][0] - 1];
				v.t = vecVT[aIndex[i][1] - 1];
				v.n = vecVN[aIndex[i][2] - 1];

				if (pmat)
				{
					D3DXVec3TransformNormal(&v.n, &vecVN[aIndex[i][2] - 1], pmat);
					D3DXVec3TransformCoord(&v.p, &vecV[aIndex[i][0] - 1], pmat);
				}
				vecVertex.push_back(v);
			}

			vecAttr.push_back(m_mapMtlTex[sMtlName]->GetAttrID());
		}

	}
	fclose(fp);

	LPD3DXMESH pMesh = NULL;
	D3DXCreateMeshFVF(vecVertex.size() / 3,
		vecVertex.size(),
		D3DXMESH_MANAGED | D3DXMESH_32BIT,
		ST_PNT_VERTEX::FVF,
		g_pD3DDevice,
		&pMesh);

	ST_PNT_VERTEX* pV = NULL;
	pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
	memcpy(pV, &vecVertex[0], sizeof(ST_PNT_VERTEX) * vecVertex.size());
	pMesh->UnlockVertexBuffer();

	int* pI = NULL;
	pMesh->LockIndexBuffer(0, (LPVOID*)&pI);
	for (int i = 0; i < vecVertex.size(); ++i)
	{
		pI[i] = i;
	}
	pMesh->UnlockIndexBuffer();

	DWORD* pA = NULL;
	pMesh->LockAttributeBuffer(0, &pA);
	memcpy(pA, &vecAttr[0], sizeof(DWORD) * vecAttr.size());
	pMesh->UnlockAttributeBuffer();

	std::vector<DWORD> vecAdj(vecVertex.size());
	pMesh->GenerateAdjacency(0.0f, &vecAdj[0]);

	pMesh->OptimizeInplace(
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_VERTEXCACHE,
		&vecAdj[0],
		NULL,
		NULL,
		NULL);

	return pMesh;
}

void cObjLoader::Load(IN char * szFilename, IN char * szFolderName, IN D3DXMATRIX* pMat,
	OUT std::vector<cMtlTex*>& vecMtlTex, OUT LPD3DXMESH& mesh, 
	OUT std::vector<cMtlTex*>& vecHiddenMtlTex, OUT std::vector<LPD3DXMESH>& vecHiddenMesh)
{
	//m_mapMtlTex.clear();
	//m_mapHiddenMtlTex.clear();

	m_sPath = szFolderName;

	std::vector<D3DXVECTOR3>	vecV;
	std::vector<D3DXVECTOR3>	vecVN;
	std::vector<D3DXVECTOR2>	vecVT;

	std::string					sMtlName;

	std::vector<ST_PNT_VERTEX>	vecVertex;
	std::vector<DWORD>			vecAttr;

	std::vector<ST_PNT_VERTEX>	vecHiddenVertex;
	std::vector<DWORD>			vecHiddenAttr;


	FILE* fp = NULL;
	string sFullpath = m_sPath + szFilename;

	fopen_s(&fp, sFullpath.c_str(), "r");
	while (!feof(fp))
	{
		char szBuf[1024] = { '\0', };

		fgets(szBuf, 1024, fp);

		if (strlen(szBuf) == 0)
			continue;
		else if (szBuf[0] == '#')
		{
			if (StartsWith(szBuf, "# object"))
			{
				if (vecHiddenVertex.empty()) continue;
				else
				{
					LPD3DXMESH pMesh = NULL;
					D3DXCreateMeshFVF(vecHiddenVertex.size() / 3,
						vecHiddenVertex.size(),
						D3DXMESH_MANAGED | D3DXMESH_32BIT,
						ST_PNT_VERTEX::FVF,
						g_pD3DDevice,
						&pMesh);

					ST_PNT_VERTEX* pV = NULL;
					pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
					memcpy(pV, &vecHiddenVertex[0], sizeof(ST_PNT_VERTEX) * vecHiddenVertex.size());
					pMesh->UnlockVertexBuffer();

					int* pI = NULL;
					pMesh->LockIndexBuffer(0, (LPVOID*)&pI);
					for (int i = 0; i < vecHiddenVertex.size(); ++i)
					{
						pI[i] = i;
					}
					pMesh->UnlockIndexBuffer();

					DWORD* pA = NULL;
					pMesh->LockAttributeBuffer(0, &pA);
					memcpy(pA, &vecHiddenAttr[0], sizeof(DWORD) * vecHiddenAttr.size());
					pMesh->UnlockAttributeBuffer();

					std::vector<DWORD> vecAdj(vecHiddenVertex.size());
					pMesh->GenerateAdjacency(0.0f, &vecAdj[0]);

					pMesh->OptimizeInplace(
						D3DXMESHOPT_COMPACT |
						D3DXMESHOPT_ATTRSORT |
						D3DXMESHOPT_VERTEXCACHE,
						&vecAdj[0],
						NULL,
						NULL,
						NULL);

					vecHiddenMesh.push_back(pMesh);

					vecHiddenAttr.clear();
					vecHiddenVertex.clear();

				}
			}
			else
				continue;
		}

		else if (szBuf[0] == 'm')
		{
			char szMtlPath[1024];
			sscanf_s(szBuf, "%*s %s", szMtlPath, 1024);
			LoadMtlLib(szMtlPath, vecMtlTex, vecHiddenMtlTex);

		}
		else if (szBuf[0] == 'g')
		{
		}
		else if (szBuf[0] == 'v')
		{
			if (szBuf[1] == 't')
			{
				float u, v;
				sscanf_s(szBuf, "%*s %f %f %*f", &u, &v);
				vecVT.push_back(D3DXVECTOR2(u, 1 - v));
			}
			else if (szBuf[1] == 'n')
			{
				float x, y, z;
				sscanf_s(szBuf, "%*s %f %f %f", &x, &y, &z);
				vecVN.push_back(D3DXVECTOR3(x, y, z));
			}
			else
			{
				float x, y, z;
				sscanf_s(szBuf, "%*s %f %f %f", &x, &y, &z);
				vecV.push_back(D3DXVECTOR3(x, y, z));
			}
		}
		else if (szBuf[0] == 'u')
		{
			char szMtlName[1024];
			sscanf_s(szBuf, "%*s %s", szMtlName, 1024);
			sMtlName = std::string(szMtlName);
		}
		else if (szBuf[0] == 'f')
		{
			int aIndex[3][3];
			sscanf_s(szBuf, "%*s %d/%d/%d %d/%d/%d %d/%d/%d",
				&aIndex[0][0], &aIndex[0][1], &aIndex[0][2],
				&aIndex[1][0], &aIndex[1][1], &aIndex[1][2],
				&aIndex[2][0], &aIndex[2][1], &aIndex[2][2]);
			for (int i = 0; i < 3; ++i)
			{
				ST_PNT_VERTEX v;
				v.p = vecV[aIndex[i][0] - 1];
				v.t = vecVT[aIndex[i][1] - 1];
				v.n = vecVN[aIndex[i][2] - 1];

				if (pMat)
				{
					D3DXVec3TransformNormal(&v.n, &vecVN[aIndex[i][2] - 1], pMat);
					D3DXVec3TransformCoord(&v.p, &vecV[aIndex[i][0] - 1], pMat);
				}


				if (StartsWith(StringToChar(sMtlName), "Hidden"))
					vecHiddenVertex.push_back(v);
				else
					vecVertex.push_back(v);

				
			}

			if (StartsWith(StringToChar(sMtlName), "Hidden"))
				vecHiddenAttr.push_back(m_mapHiddenMtlTex[sMtlName]->GetAttrID());
			else
				vecAttr.push_back(m_mapMtlTex[sMtlName]->GetAttrID());
			
		}

	}
	fclose(fp);

	D3DXCreateMeshFVF(vecVertex.size() / 3,
		vecVertex.size(),
		D3DXMESH_MANAGED | D3DXMESH_32BIT,
		ST_PNT_VERTEX::FVF,
		g_pD3DDevice,
		&mesh);

	ST_PNT_VERTEX* pV = NULL;
	mesh->LockVertexBuffer(0, (LPVOID*)&pV);
	memcpy(pV, &vecVertex[0], sizeof(ST_PNT_VERTEX) * vecVertex.size());
	mesh->UnlockVertexBuffer();

	int* pI = NULL;
	mesh->LockIndexBuffer(0, (LPVOID*)&pI);
	for (int i = 0; i < vecVertex.size(); ++i)
	{
		pI[i] = i;
	}
	mesh->UnlockIndexBuffer();

	DWORD* pA = NULL;
	mesh->LockAttributeBuffer(0, &pA);
	memcpy(pA, &vecAttr[0], sizeof(DWORD) * vecAttr.size());
	mesh->UnlockAttributeBuffer();

	std::vector<DWORD> vecAdj(vecVertex.size());
	mesh->GenerateAdjacency(0.0f, &vecAdj[0]);

	mesh->OptimizeInplace(
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_VERTEXCACHE,
		&vecAdj[0],
		NULL,
		NULL,
		NULL);

}

void cObjLoader::LoadMtlLib(char* szFilename)
{
	std::string sMtlName;

	FILE* fp = NULL;
	
	string sRelPath = m_sPath + szFilename;

	fopen_s(&fp, sRelPath.c_str(), "r");
	while (!feof(fp))
	{
		char szBuf[1024] = { '\0', };

		fgets(szBuf, 1024, fp);

		if (strlen(szBuf) == 0)
			continue;

		if (szBuf[0] == '#')
		{
			continue;
		}
		else if (szBuf[0] == 'n')
		{
			char szMtlName[1024];
			sscanf_s(szBuf, "%*s %s", szMtlName, 1024);
			sMtlName = std::string(szMtlName);
			m_mapMtlTex[sMtlName] = new cMtlTex;
		}
		else if (szBuf[0] == 'K')
		{
			if (szBuf[1] == 'a')
			{
				float r, g, b;
				sscanf_s(szBuf, "%*s %f %f %f", &r, &g, &b);
				D3DMATERIAL9& stMtl = m_mapMtlTex[sMtlName]->GetMtl();
				stMtl.Ambient = D3DXCOLOR(r, g, b, 1.0f);
			}
			else if (szBuf[1] == 'd')
			{
				float r, g, b;
				sscanf_s(szBuf, "%*s %f %f %f", &r, &g, &b);
				D3DMATERIAL9& stMtl = m_mapMtlTex[sMtlName]->GetMtl();
				stMtl.Diffuse = D3DXCOLOR(r, g, b, 1.0f);
			}
			else if (szBuf[1] == 's')
			{
				float r, g, b;
				sscanf_s(szBuf, "%*s %f %f %f", &r, &g, &b);
				D3DMATERIAL9& stMtl = m_mapMtlTex[sMtlName]->GetMtl();
				stMtl.Specular = D3DXCOLOR(r, g, b, 1.0f);
			}
		}
		else if (szBuf[0] == 'm')
		{
			char szTexturePath[1024];
			sscanf_s(szBuf, "%*s %s", szTexturePath, 1024);
			string sRelPath = m_sPath + szTexturePath;
			m_mapMtlTex[sMtlName]->SetTexture(g_pTextureManager->GetTexture(sRelPath));
		}
	}
	fclose(fp);
}

void cObjLoader::LoadMtlLib(IN char* szFilename, OUT std::vector<cMtlTex*>& vecMtlTex)
{
	std::string sMtlName;

	FILE* fp = NULL;

	string sRelPath = m_sPath + szFilename;

	fopen_s(&fp, sRelPath.c_str(), "r");

	while (!feof(fp))
	{
		char szBuf[1024] = { '\0', };

		fgets(szBuf, 1024, fp);

		if (strlen(szBuf) == 0)
			continue;

		if (szBuf[0] == '#')
		{
			continue;
		}
		else if (StartsWith(szBuf, "newmtl"))
		{
			char szMtlName[1024];
			sscanf_s(szBuf, "%*s %s", szMtlName, 1024);
			sMtlName = std::string(szMtlName);
			m_mapMtlTex[sMtlName] = new cMtlTex;
			m_mapMtlTex[sMtlName]->SetAttrID(vecMtlTex.size());
			if (StartsWith(StringToChar(sMtlName), "Hidden"))
			{
				m_mapMtlTex[sMtlName]->SetIsHiddenObj(true);
			}
				
			vecMtlTex.push_back(m_mapMtlTex[sMtlName]);

			int a = 0;
		}
		else if (StartsWith(szBuf, "\tKa"))
		{
			float r, g, b;
			sscanf_s(szBuf, "%*s %f %f %f", &r, &g, &b);
			D3DMATERIAL9& stMtl = m_mapMtlTex[sMtlName]->GetMtl();
			stMtl.Ambient = D3DXCOLOR(r, g, b, 1.0f);
		}
		else if (StartsWith(szBuf, "\tKd"))
		{
			float r, g, b;
			sscanf_s(szBuf, "%*s %f %f %f", &r, &g, &b);
			D3DMATERIAL9& stMtl = m_mapMtlTex[sMtlName]->GetMtl();
			stMtl.Diffuse = D3DXCOLOR(r, g, b, 1.0f);
		}
		else if (StartsWith(szBuf, "\tKs"))
		{
			float r, g, b;
			sscanf_s(szBuf, "%*s %f %f %f", &r, &g, &b);
			D3DMATERIAL9& stMtl = m_mapMtlTex[sMtlName]->GetMtl();
			stMtl.Specular = D3DXCOLOR(r, g, b, 1.0f);
		}
		
		else if (StartsWith(szBuf, "\tmap_Kd"))
		{
			char szTexturePath[1024];
			sscanf_s(szBuf, "%*s %s", szTexturePath, 1024);
			string sFileName = ExtractFileName(szTexturePath);
			string relPath = m_sPath + sFileName;
			m_mapMtlTex[sMtlName]->SetTexture(g_pTextureManager->GetTexture(relPath));
		}
	}
	fclose(fp);
}

void cObjLoader::LoadMtlLib(IN char * szFilename, OUT std::vector<cMtlTex*>& vecMtlTex, OUT std::vector<cMtlTex*>& vecHiddenMtlTex)
{
	std::string sMtlName;
	bool hiddenMtl = false;

	FILE* fp = NULL;

	string sRelPath = m_sPath + szFilename;

	fopen_s(&fp, sRelPath.c_str(), "r");

	while (!feof(fp))
	{
		char szBuf[1024] = { '\0', };

		fgets(szBuf, 1024, fp);

		if (strlen(szBuf) == 0)
			continue;

		if (szBuf[0] == '#')
		{
			continue;
		}
		else if (StartsWith(szBuf, "newmtl"))
		{
			char szMtlName[1024];
			sscanf_s(szBuf, "%*s %s", szMtlName, 1024);
			sMtlName = std::string(szMtlName);

			m_mapMtlTex[sMtlName] = new cMtlTex;
			m_mapMtlTex[sMtlName]->SetAttrID(vecMtlTex.size());
			vecMtlTex.push_back(m_mapMtlTex[sMtlName]);
			hiddenMtl = false;

			if (StartsWith(StringToChar(sMtlName), "Hidden"))
			{
				m_mapHiddenMtlTex[sMtlName] = new cMtlTex;
				m_mapHiddenMtlTex[sMtlName]->SetAttrID(vecHiddenMtlTex.size());
				vecHiddenMtlTex.push_back(m_mapHiddenMtlTex[sMtlName]);
				hiddenMtl = true;
			}
		}
		else if (StartsWith(szBuf, "\tKa"))
		{
			float r, g, b;
			sscanf_s(szBuf, "%*s %f %f %f", &r, &g, &b);
			if (hiddenMtl)
			{
				D3DMATERIAL9& stMtl = m_mapHiddenMtlTex[sMtlName]->GetMtl();
				stMtl.Ambient = D3DXCOLOR(r, g, b, 1.0f);
			}
			else
			{
				D3DMATERIAL9& stMtl = m_mapMtlTex[sMtlName]->GetMtl();
				stMtl.Ambient = D3DXCOLOR(r, g, b, 1.0f);
			}
			
			
		}
		else if (StartsWith(szBuf, "\tKd"))
		{
			float r, g, b;
			sscanf_s(szBuf, "%*s %f %f %f", &r, &g, &b);

			if (hiddenMtl)
			{
				D3DMATERIAL9& stMtl = m_mapHiddenMtlTex[sMtlName]->GetMtl();
				stMtl.Diffuse = D3DXCOLOR(r, g, b, 1.0f);
			}
			else
			{
				D3DMATERIAL9& stMtl = m_mapMtlTex[sMtlName]->GetMtl();
				stMtl.Diffuse = D3DXCOLOR(r, g, b, 1.0f);
			}
		}
		else if (StartsWith(szBuf, "\tKs"))
		{
			float r, g, b;
			sscanf_s(szBuf, "%*s %f %f %f", &r, &g, &b);

			if (hiddenMtl)
			{
				D3DMATERIAL9& stMtl = m_mapHiddenMtlTex[sMtlName]->GetMtl();
				stMtl.Specular = D3DXCOLOR(r, g, b, 1.0f);
			}
			else
			{
				D3DMATERIAL9& stMtl = m_mapMtlTex[sMtlName]->GetMtl();
				stMtl.Specular = D3DXCOLOR(r, g, b, 1.0f);
			}
			
		}

		else if (StartsWith(szBuf, "\tmap_Kd"))
		{
			char szTexturePath[1024];
			sscanf_s(szBuf, "%*s %s", szTexturePath, 1024);
			string sFileName = ExtractFileName(szTexturePath);
			string relPath = m_sPath + sFileName;

			if (hiddenMtl)
				m_mapHiddenMtlTex[sMtlName]->SetTexture(g_pTextureManager->GetTexture(relPath));
			else
				m_mapMtlTex[sMtlName]->SetTexture(g_pTextureManager->GetTexture(relPath));
			
		}
	}
	fclose(fp);
}

string cObjLoader::ExtractFileName(string sFullPath)
{
	int lastPathIndex = 0;

	// ���� ���� ��ο��� ���ϸ� �����Ѵ�.

	//�ڿ������� �˻��ؼ� ������ '/'�� ã�´� / ��ã���� -1
	lastPathIndex = sFullPath.find_last_of('/');

	//��ã������ Ȥ ��ΰ� \\�������� ������ �� ���
	if (lastPathIndex == -1)
		lastPathIndex = sFullPath.find_last_of('\\');

	//��ΰ� �߸� �� ���
	assert(lastPathIndex != -1 && "��ΰ� �߸� �ž����ϴ�.");

	//��θ� ã�� ��� ���ϸ� �����Ѵ�.
	//m_sPath = sFullPath.substr(0, lastPathIndex + 1);
	string fileName = sFullPath.substr(lastPathIndex + 1, sFullPath.length());

	return fileName;
}

bool cObjLoader::StartsWith(char * str, char * beginStr)
{
	char* temp = strstr(str, beginStr);
	if (temp == str)
		return true;

	return false;
}
