#pragma once
class cPyramid
{
private:
	std::vector<ST_PN_VERTEX>	m_vecVertex;
	D3DMATERIAL9				m_stMtl;
	LPDIRECT3DVERTEXBUFFER9		m_pVB;

public:
	cPyramid(void);
	~cPyramid(void);

	void Setup(D3DXCOLOR c, D3DXMATRIXA16* pmat = NULL);
	void Render();
};

