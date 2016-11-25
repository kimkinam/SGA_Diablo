#include "stdafx.h"
#include "cObj.h"


cObj::cObj()
	: m_matWorld(NULL)
{
	m_sPath = "";
}


cObj::~cObj()
{
	for (size_t i = 0; i < m_vGroup.size(); ++i)
	{
		SAFE_RELEASE(m_mMtl[m_vGroup[i]->mtlName].Tex);
	}
}

void cObj::Render()
{
	g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);

	if (m_matWorld)
	{
		D3DXMATRIX matR;
		D3DXMatrixRotationX(&matR, D3DXToRadian(180));
		*m_matWorld = matR * (*m_matWorld);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, m_matWorld);
	}

	else
	{
		D3DXMATRIX mat;
		D3DXMatrixIdentity(&mat);
		D3DXMatrixTranslation(&mat, 1, 1, 0);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &mat);
	}
	for (size_t i = 0; i < m_vGroup.size(); ++i)
	{
		g_pD3DDevice->SetMaterial(&m_mMtl[m_vGroup[i]->mtlName].Mtl);
		g_pD3DDevice->SetTexture(0, m_mMtl[m_vGroup[i]->mtlName].Tex);
		g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
			m_vGroup[i]->vecVertex.size() / 3,
			&m_vGroup[i]->vecVertex[0],
			sizeof(ST_PNT_VERTEX));
	}
}

void cObj::LoadFile(const char * fileName, const char* path)
{
	m_sObjName = fileName;

	FILE* fp = NULL;

	char p[1024];
	memset(p, '\0', sizeof(path));

	sprintf_s(p, "%s%s", path, fileName);

	fopen_s(&fp, p, "r");

	assert(fp != NULL && "파일이 열리지 않았습니다.");

	m_sPath = path;

	while (!feof(fp))
	{
		char szBuf[1024];
		memset(szBuf, '\0', sizeof(szBuf));

		fgets(szBuf, sizeof(szBuf), fp);

		if (szBuf[0] == '#') continue;
		else if (StartsWith(szBuf, "mtllib"))
		{
			char szMtlFileName[1024];
			memset(szMtlFileName, '\0', sizeof(szMtlFileName));

			sscanf_s(szBuf, "%*s  %s", szMtlFileName, sizeof(szMtlFileName));

			LoadMtrlFile(szMtlFileName, path);

			int a = 0;
		}
		else if (StartsWith(szBuf, "v "))
		{
			float x, y, z;
			sscanf_s(szBuf, "%*s %f %f %f", &x, &y, &z);
			m_vecFilePoint.push_back(D3DXVECTOR3(x, y, z));
		}
		else if (StartsWith(szBuf, "vn"))
		{
			float nx, ny, nz;
			sscanf_s(szBuf, "%*s %f %f %f", &nx, &ny, &nz);
			m_vecFileNormal.push_back(D3DXVECTOR3(nx, ny, nz));
		}
		else if (StartsWith(szBuf, "vt"))
		{
			float u, v;
			sscanf_s(szBuf, "%*s %f %f %*f", &u, &v);
			m_vecFileTexture.push_back(D3DXVECTOR2(u, 1 - v));
		}

		else if (StartsWith(szBuf, "usemtl"))
		{
			char szMtlFileName[1024];
			memset(szMtlFileName, '\0', sizeof(szMtlFileName));

			sscanf_s(szBuf, "%*s  %s", szMtlFileName, sizeof(szMtlFileName));

			ST_GROUP* pGroup = new ST_GROUP;
			pGroup->mtlName = szMtlFileName;
			m_vGroup.push_back(pGroup);
		}
		else if (StartsWith(szBuf, "f"))
		{
			int Idx[3][3];
			sscanf_s(szBuf, "%*s %d/%d/%d %d/%d/%d %d/%d/%d",
				&Idx[0][0], &Idx[0][1], &Idx[0][2],
				&Idx[1][0], &Idx[1][1], &Idx[1][2],
				&Idx[2][0], &Idx[2][1], &Idx[2][2], sizeof(Idx));

			for (int i = 0; i < 3; i++)
			{
				ST_PNT_VERTEX v;
				v.p = m_vecFilePoint[Idx[i][0] - 1];
				v.t = m_vecFileTexture[Idx[i][1] - 1];
				v.n = m_vecFileNormal[Idx[i][2] - 1];
				m_vGroup.back()->vecVertex.push_back(v);
			}
		}

	}
	fclose(fp);

}

void cObj::LoadMtrlFile(const char * fileName, const char* path)
{
	FILE* fp = NULL;

	char p[1024];
	memset(p, '\0', sizeof(path));

	sprintf_s(p, "%s/%s", path, fileName);

	fopen_s(&fp, p, "r");

	if (fp == NULL) return;

	char szBuf[1024];
	memset(szBuf, '\0', sizeof(szBuf));

	char szMtlName[1024];
	memset(szMtlName, '\0', sizeof(szMtlName));

	while (!feof(fp))
	{
		fgets(szBuf, sizeof(szBuf), fp);

		if (szBuf[0] == '#') continue;
		else if (StartsWith(szBuf, "newmtl"))
		{
			sscanf_s(szBuf, "%*s  %s", szMtlName, sizeof(szMtlName));
			int a = 0;
		}
		else if (StartsWith(szBuf, "\tKa"))
		{
			float fR, fG, fB;
			sscanf_s(szBuf, "%*s  %f %f %f", &fR, &fG, &fB);

			m_mMtl[szMtlName].Mtl.Ambient.r = fR;
			m_mMtl[szMtlName].Mtl.Ambient.g = fG;
			m_mMtl[szMtlName].Mtl.Ambient.b = fB;
			m_mMtl[szMtlName].Mtl.Ambient.a = 1.0f;

			//int a = 0;
		}
		else if (StartsWith(szBuf, "\tKd"))
		{
			float fR, fG, fB;
			sscanf_s(szBuf, "%*s  %f %f %f", &fR, &fG, &fB);
			m_mMtl[szMtlName].Mtl.Diffuse.r = fR;
			m_mMtl[szMtlName].Mtl.Diffuse.g = fG;
			m_mMtl[szMtlName].Mtl.Diffuse.b = fB;
			m_mMtl[szMtlName].Mtl.Diffuse.a = 1.0f;
		}
		else if (StartsWith(szBuf, "\tKs"))
		{
			float fR, fG, fB;
			sscanf_s(szBuf, "%*s  %f %f %f", &fR, &fG, &fB);
			m_mMtl[szMtlName].Mtl.Specular.r = fR;
			m_mMtl[szMtlName].Mtl.Specular.g = fG;
			m_mMtl[szMtlName].Mtl.Specular.b = fB;
			m_mMtl[szMtlName].Mtl.Specular.a = 1.0f;

		}
		else if (StartsWith(szBuf, "\tmap_Kd"))
		{
			char szTextureName[1024];
			sscanf_s(szBuf, "%*s  %s", szTextureName, sizeof(szTextureName));

			// 파일의 절대경로에서 이름만 가지고 온다.
			// 상대경로를 계산한다.
			string relPath = m_sPath + ExtractFileName(string(szTextureName));

			D3DXCreateTextureFromFile(g_pD3DDevice, relPath.c_str(), &m_Tex);

			if (!m_mMtl[szMtlName].Tex && m_Tex)
			{
				m_mMtl[szMtlName].Tex = m_Tex;
				m_mMtl[szMtlName].Tex->AddRef();
			}
			SAFE_RELEASE(m_Tex);
		}
		else if (StartsWith(szBuf, "\tmap_Ks"))
		{
			char szTextureName[1024];
			sscanf_s(szBuf, "%*s  %s", szTextureName, sizeof(szTextureName));

			char path[1024];
			sprintf_s(path, "%s", szTextureName);

			D3DXCreateTextureFromFile(g_pD3DDevice, path, &m_Tex);

			if (!m_mMtl[szMtlName].Tex && m_Tex)
			{
				m_mMtl[szMtlName].Tex = m_Tex;
				m_mMtl[szMtlName].Tex->AddRef();
			}
			SAFE_RELEASE(m_Tex);
		}
	}
	fclose(fp);
}

bool cObj::StartsWith(char * str, char * beginStr)
{
	char* temp = strstr(str, beginStr);
	if (temp == str)
		return true;

	return false;
}

string cObj::ExtractFileName(string sFullPath)
{
	int lastPathIndex = 0;

	// 파일 절대 경로에서 파일명만 추출한다.

	//뒤에서부터 검사해서 마지막 '/'를 찾는다 / 못찾으면 -1
	lastPathIndex = sFullPath.find_last_of('/');

	//못찾았지만 혹 경로가 \\형식으로 나오게 될 경우
	if (lastPathIndex == -1)
		lastPathIndex = sFullPath.find_last_of('\\');

	//경로가 잘못 된 경우
	assert(lastPathIndex != -1 && "경로가 잘못 돼었습니다.");

	//경로를 찾은 경우 파일명만 추출한다.
	string path = sFullPath.substr(0, lastPathIndex + 1);
	string fileName = sFullPath.substr(lastPathIndex + 1, sFullPath.length());

	return fileName;
}
