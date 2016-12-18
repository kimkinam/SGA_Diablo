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

<<<<<<< HEAD
	bool						m_bIsCutScene;

	float						m_fCutSceneTimer;

	int							m_nBossTellCount;
=======
	D3DXMATRIX*					PointLightWorldTM;



>>>>>>> 2b6b7f0461585e1bec4175a4b088e0cb696d50c6
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
<<<<<<< HEAD

	bool CutScene();
	bool CutScene2();
=======
	void UISetting();
>>>>>>> 2b6b7f0461585e1bec4175a4b088e0cb696d50c6
};

