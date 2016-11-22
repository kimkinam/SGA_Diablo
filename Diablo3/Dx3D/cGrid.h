#pragma once

class cPyramid;

class cGrid
{
private:
	std::vector<ST_PC_VERTEX>	m_vecVertex;
	std::vector<cPyramid*>		m_vecPyramid;
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
public:
	cGrid(void);
	~cGrid(void);

	void Setup(int nLine);
	void Render();
};

