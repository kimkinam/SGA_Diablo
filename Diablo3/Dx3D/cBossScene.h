#pragma once

class cGrid;
class cBoss;
class cPlayer;
class cObj;
class cLighting;
class cUiManager;

class cBossScene :
	public cSceneObject
{
private:
	cGrid*			m_pGrid;
	cObj*			m_pMap;
	cBoss*			m_pBoss; // º¸½º
	cPlayer*		m_pPlayer;
	cUiManager*		m_pUI;
	std::vector<ST_PC_VERTEX>	m_vecTiles;
	D3DXVECTOR3					m_vpickPos;

	float						m_fLifeTime;
	float						m_fLifeTimeDecrease;

	float						m_fTotalFireTime;
	float						m_fFireInterval;

	bool						m_bIsCutScene;

	float						m_fCutSceneTimer;

	int							m_nBossTellCount;
	D3DXMATRIX*					PointLightWorldTM;



public:
	cBossScene();
	virtual ~cBossScene();

	virtual HRESULT SetUp()	override;
	virtual HRESULT Reset()	override;
	virtual void Update()	override;
	virtual void Render()	override;
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;



	void SetLight();
	void SetPointLight();
	void PlayerMove();

	bool CutScene();
	bool CutScene2();
	void UISetting();
};

