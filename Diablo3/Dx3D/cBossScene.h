#pragma once

class cGrid;
class cBoss;
class cPlayer;
class cObj;

class cBossScene :
	public cSceneObject
{
private:
	cGrid*			m_pGrid;
	cObj*			m_pMap;
	cBoss*			m_pBoss; // º¸½º
	cPlayer*		m_pPlayer;

	std::vector<ST_PC_VERTEX>	m_vecTiles;
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

