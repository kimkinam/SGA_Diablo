#pragma once

class cGrid;
class cUiManager;
class cCamera;
class cMap;
class cBoundBox;
class cMonster;
class cPlayer;
class cShaderManager;
class cParticleEmitter;

class cGamingScene : public cSceneObject
{
private:
	cGrid*								m_pGrid;
	cPlayer*							m_pPlayer;
	std::vector<cMap*>					m_vecMap;
	std::vector<cOBB*>					m_vecBoundBox;
	//std::vector<cMonster*>				m_vecMonster;
	std::vector<cMonster*>::iterator	m_vecMonsterIter;
	cUiManager*							m_pUI;
	cMonster*							m_pCurMonster;

	LONG								enemyBarRight;
	LONG								enemyBarLeft;

	std::vector<ST_PC_VERTEX>			m_vecTiles;

	SYNTHESIZE(std::vector<cMonster*>, m_vecMonster, vecMonster);

	cShaderManager*					   m_Cloud;
	D3DXVECTOR3              CloudTranselation;
	D3DXVECTOR3					  CloudScaling;

	D3DXMATRIX*						   PointLightWorldTM;


	cParticleEmitter*				m_ptcGuideLine;

public:
	cGamingScene();
	virtual ~cGamingScene();

	void LoadMap(string fileName);
	void UISetting();
	void PlayerMoveTest();
	bool CollisionTest();
	void SetLight(); //����
	void SetPointLight();
	
	void GuidePtcSetUp();
	void GuidePtcUpdate();
	void GuidePtcRender();



	//cScene override;
	virtual HRESULT SetUp()	override;
	virtual void Update()	override;
	virtual void Render()	override;
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;


};

