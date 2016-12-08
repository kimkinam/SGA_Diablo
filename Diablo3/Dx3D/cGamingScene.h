#pragma once

class cGrid;
class cUiManager;
class cCamera;
class cMap;
class cBoundBox;
class cMonster;
class cPlayer;



class cGamingScene : public cSceneObject
{
private:
	cGrid*					m_pGrid;
	cPlayer*				m_pPlayer;
	std::vector<cMap*>		m_vecMap;
	std::vector<cOBB*>		m_vecBoundBox;
	std::vector<cMonster*>	m_vecMonster;

	std::vector<ST_PC_VERTEX>	m_vecTiles;

public:
	cGamingScene();
	virtual ~cGamingScene();

	void LoadMap(string fileName);
	void PlayerMoveTest();
	bool CollisionTest();

	//cScene override;
	virtual HRESULT SetUp()	override;
	virtual void Update()	override;
	virtual void Render()	override;
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

