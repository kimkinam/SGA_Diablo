#pragma once

class cMtlTex;
class cGroup;

class cObjLoader
{
private:
	std::map<std::string, cMtlTex*> m_mapMtlTex;

public:
	cObjLoader(void);
	~cObjLoader(void);

	void Load(IN char* szFilename, OUT std::vector<cGroup*>& vecGroup, IN D3DXMATRIXA16* pMat = NULL);
	void LoadSurface(IN char* szFilename, OUT std::vector<D3DXVECTOR3>& vecSurface, IN D3DXMATRIXA16* pMat = NULL);
	void LoadMtlLib(char* szFilename);
};

