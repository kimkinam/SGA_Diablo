#pragma once

class cMtlTex;
class cGroup;

class cObjLoader
{
private:
	std::map<std::string, cMtlTex*>		m_mapMtlTex;
	std::map<std::string, cMtlTex*>		m_mapHiddenMtlTex;
	std::map<std::string, cMtlTex*>		m_mapComMtlTex;
	string								m_sPath;
	
public:
	cObjLoader(void);
	~cObjLoader(void);

	void Load(IN char* szFilename, IN char* szFolderName, OUT std::vector<cGroup*>& vecGroup, IN D3DXMATRIX* pmat = NULL);
	LPD3DXMESH Load(IN char* szFilename, IN char* szFolderName, OUT std::vector<cMtlTex*>& vecMtlTex, IN D3DXMATRIX* pmat = NULL);
	
	void Load(IN char* szFilename, IN char* szFolderName, IN D3DXMATRIX* pMat,
		OUT std::vector<cMtlTex*>& vecMtlTex, OUT LPD3DXMESH& mesh,
		OUT std::vector<cMtlTex*>& vecHiddenMtlTex, OUT std::vector<LPD3DXMESH>& vecHiddenMesh);

	void LoadMtlLib(char* szFilename);
	void LoadMtlLib(IN char* szFilename, OUT std::vector<cMtlTex*>& vecMtlTex);
	void LoadMtlLib(IN char* szFilename, OUT std::vector<cMtlTex*>& vecMtlTex, OUT std::vector<cMtlTex*>& vecHiddenMtlTex);

	string ExtractFileName(string sFullPath);
	bool StartsWith(char* str, char* beginStr);
};

