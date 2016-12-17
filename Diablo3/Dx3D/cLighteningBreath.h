#pragma once
class cTrailRenderer;
class cLighteningBreath
{
	enum LB_TYPE
	{
		BREATH_YELLOW,
		BREATH_BLUE,
		BREATH_FORK,
		BREATH_TRAIL
	};
private:
	int m_nType;

	LPDIRECT3DTEXTURE9 m_pLighteningTex;
	ST_PT_VERTEX m_stPTV[4];
	DWORD		m_nIb[6];

	int m_nAnimationIndex;


	D3DXVECTOR3 m_vScail;
	D3DXVECTOR3	m_vPos;
	D3DXVECTOR3 m_vUp;
	D3DXVECTOR3 m_vRight;
	D3DXVECTOR3 m_vDirection;
	D3DXVECTOR3 m_vMoveDir;

	
	D3DXMATRIXA16 m_matLocal;
	D3DXMATRIXA16 m_matWorld;

	float m_fFrameRate;
	float m_fLifeTime;
	float m_fAlphaFactor;

	bool m_bIsDrawLine;
	cTrailRenderer* m_pTrailRenderer;
	SYNTHESIZE_PASS_BY_REF(LPD3DXEFFECT, m_pEffect, Effect);
	
public:
	cLighteningBreath();
	~cLighteningBreath();
	void SetUp(int type,D3DXVECTOR3 vPos,D3DXVECTOR3 vMoveDir,int animIndx);
	void TrailSetUp();
	void Release();
	
	void Update();
	void LocalUpdate();
	void WorldUpdate();

	void BreathUpdate();
	void ForkUpdate();
	void TrailUpdate();


	void Render();
	void BreathRender();
	void ForkRender();
	void TrailRender();

	bool IsDestroyTime();
};

