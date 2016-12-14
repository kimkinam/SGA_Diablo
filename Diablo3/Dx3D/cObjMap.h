#pragma once
#include "iMap.h"

class cMtlTex;

class cObjMap :
	public iMap
{
private:
	SYNTHESIZE(std::vector<D3DXVECTOR3>, m_vecSurface, Surface);
	SYNTHESIZE(D3DXVECTOR3, m_vPosition, Position);
	SYNTHESIZE(LPD3DXMESH, m_pMesh, Mesh);
	SYNTHESIZE(std::vector<cMtlTex*>, m_vecMtl, Mtl);
	SYNTHESIZE(D3DXMATRIX*, m_matWorld, WorldTM);

	LPD3DXEFFECT m_pEffect;
public:
	cObjMap();
	virtual ~cObjMap();

	virtual void Load(char* szMap, char* szSurface, char* szFolder, D3DXMATRIXA16* pMat = NULL);


	//iMap override
	virtual void Render();
	virtual bool GetHeight(IN float x, OUT float& y, IN float z) override;


};

