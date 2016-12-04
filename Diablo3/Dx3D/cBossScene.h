#pragma once

class cGrid;
class cBoss;
class cSkeleton;
class cPlayer;
class cActionTrace;

class cBossScene :
	public cSceneObject
{
private:
<<<<<<< HEAD
	cGrid*			m_pGrid;
	cBoss*			m_pBoss; // 보스
	cPlayer*		m_pPlayer;
	cSkeleton*		m_pSkeleton;
=======
	cGrid*		   m_pGrid;
	cBoss*		   Boss_diablo; // 보스
	cPlayer*	   m_pPlayer;


>>>>>>> 65ab56b83d0f8d2a0d2f65015c3637a9a04789e0

	std::vector<ST_PC_VERTEX>	m_vecTiles;
	D3DXVECTOR3					m_vpickPos;
	bool						m_isAnim;
	bool						m_isTurn;

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

