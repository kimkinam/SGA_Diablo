#pragma once

#include "iMap.h"

class cGroup;

class cObjMap : public iMap
{
private:
	std::vector<D3DXVECTOR3>	m_vecSurface;
	std::vector<cGroup*>		m_vecGroup;
	D3DXMATRIXA16				m_mat;
	
public:
	cObjMap();
	virtual ~cObjMap();

	virtual void Load(char* szMap, char* szSurface, D3DXMATRIXA16* pMat = NULL);

	//iMap override
	virtual bool GetHeight(IN float x, OUT float& y, IN float z) override;
	virtual void Render() override;

};

