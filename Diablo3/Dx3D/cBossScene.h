#pragma once

class cGrid;
class cBoss;
class cPlayer;
class cActionTrace;

class cBossScene :
	public cSceneObject
{
private:
	cGrid*		   m_pGrid;
	cBoss*		   Boss_diablo; // º¸½º
	cPlayer*	   m_pPlayer;

	cActionTrace*  m_pAction;

	std::vector<ST_PC_VERTEX>	m_vecTiles;
	bool						m_bIsSetMap;
	D3DXVECTOR3					m_vpickPos;

public:
	cBossScene();
	virtual ~cBossScene();

	virtual HRESULT SetUp()	override;
	virtual HRESULT Reset()	override;
	virtual void Update()	override;
	virtual void Render()	override;
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;


	void BossMoveTest();
	void PlayerMove();
};

