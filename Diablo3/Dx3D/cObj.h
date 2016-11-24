#pragma once

struct ST_MTL_TEX
{
	D3DMATERIAL9 Mtl;
	LPDIRECT3DTEXTURE9 Tex;

	void Release()
	{
		Tex->Release();
	}
};

struct ST_GROUP
{
	std::vector<ST_PNT_VERTEX>	vecVertex;
	std::string					mtlName;
};

class cObj
{
private:
	std::map<std::string, ST_MTL_TEX>	m_mMtl;
	std::vector<D3DXVECTOR3>			m_vecFilePoint;
	std::vector<D3DXVECTOR2>			m_vecFileTexture;
	std::vector<D3DXVECTOR3>			m_vecFileNormal;

	LPDIRECT3DTEXTURE9					m_Tex;
	std::vector<ST_GROUP*>				m_vGroup;
	SYNTHESIZE(D3DXMATRIX*, m_matWorld, WorldTM);
	SYNTHESIZE(std::string, m_sObjName, ObjName);

public:
	cObj();
	~cObj();

	void Render();
	void LoadFile(const char* fileName, const char* path);
	void LoadMtrlFile(const char* fileName, const char* path);
	bool StartsWith(char* str, char* beginStr);

};

