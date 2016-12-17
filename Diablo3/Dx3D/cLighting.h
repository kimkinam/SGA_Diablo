#pragma once

struct ST_LIGHT_STAT
{
	float	fAlpha;
	float	fLifeTime;
	float	fFrameRate;
	float	fAmbientMax;

	ST_LIGHT_STAT()
		: fLifeTime(0.0f), fAlpha(0.0f), fFrameRate(0.0f), fAmbientMax(0.0f)
	{
	};

	ST_LIGHT_STAT(float _fLifeTime, float _fAlpha, float _fFrameRate, float _fAmbientMax)
	{
		fLifeTime = _fLifeTime;
		fAlpha = _fAlpha;
		fFrameRate = _fFrameRate;
		fAmbientMax = _fAmbientMax;
	}
};

class cActionMove;

class cLighting
{
private:
	LPD3DXEFFECT			m_pEffect;
	LPDIRECT3DTEXTURE9		m_pTexture;
	
	SYNTHESIZE_PASS_BY_REF(ST_LIGHT_STAT, m_stInfo, Info);
	SYNTHESIZE(D3DXVECTOR3, m_vPosition, Position);
	SYNTHESIZE(D3DXVECTOR3, m_vDirection, Direction);
	SYNTHESIZE(D3DXVECTOR3, m_vUp, UpVector);
	SYNTHESIZE(D3DXVECTOR3, m_vRight, Right);
	SYNTHESIZE(D3DXVECTOR3, m_vScale, Scale);

	D3DXMATRIX				m_matLocal;
	D3DXMATRIX				m_matWorld;

	cActionMove*			m_pAction;
	int						m_nFrameCount;
	float					m_fCurAmbient;
	float					m_fCurFrameRate;
	float					m_ffLifeTimeDecrease;

	std::vector<ST_PT_VERTEX>	m_vecVertex;
	SYNTHESIZE(float, m_fSpeed, Speed);

	std::vector<cLighting*>		m_vecTrail;

	float					m_fTrailInterval;

public:
	cLighting();
	~cLighting();


	void Setup(char* szTexture, char* szShader, int nStartFrame = 0);
	void SetLightingStat(float fLifeTime, float fLifeTimeDecrease, int nFrameCount, float fFrameRate, float fAmbientMax);
	void SetLocalMatrix(D3DXVECTOR3 vDirection, D3DXVECTOR3 vSacling, DWORD dwLightNum);
	void Update();
	void Render();
	void Release();

};

