#pragma once

class cMtlTex;
class cGroup;

class cObjLoader
{
private:
	std::map<std::string, cMtlTex*>		m_mapMtlTex;
	string								m_sPath;
public:
	cObjLoader(void);
	~cObjLoader(void);

	void Load(IN char* szFilename, IN char* szFolderName, OUT std::vector<cGroup*>& vecGroup, IN D3DXMATRIXA16* pmat = NULL);
	LPD3DXMESH Load(IN char* szFilename, IN char* szFolderName, OUT std::vector<cMtlTex*>& vecMtlTex, IN D3DXMATRIXA16* pmat = NULL);
	void LoadMtlLib(char* szFilename);
	void LoadMtlLib(IN char* szFilename, OUT std::vector<cMtlTex*>& vecMtlTex);

	string ExtractFileName(string sFullPath);
	bool StartsWith(char* str, char* beginStr);
};

