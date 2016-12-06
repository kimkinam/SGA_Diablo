#pragma once

class cGrid;
class iMap;
class cUiManager;
class cMonsterManager;
class cPlayerManager;
class cCamera;
class cBoss;
class cMap;

class cGamingScene : public cSceneObject
{
private:
	cGrid*					m_pGrid;
	iMap*					m_pMap;
	cUiManager*				m_pUIManager;
	cMonsterManager*		m_pMonsterManager;
	cPlayerManager*			m_pPlayerManager;

	cBoss*					Boss_diablo;
	std::vector<cMap*>		m_vecMap;

public:
	cGamingScene();
	virtual ~cGamingScene();

	void LoadMap(string fileName);
	//cScene override;
	virtual HRESULT SetUp()	override;
	virtual void Update()	override;
	virtual void Render()	override;
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

