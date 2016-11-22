#pragma once

class cGrid;
class iMap;
class cUiManager;
class cMonsterManager;
class cPlayerManager;

class cGamingScene : public iScene
{
private:
	cGrid*					m_pGrid;
	iMap*					m_pMap;
	cUiManager*				m_pUIManager;
	cMonsterManager*		m_pMonsterManager;
	cPlayerManager*			m_pPlayerManager;

public:
	cGamingScene();
	~cGamingScene();

	//iScene override;
	virtual void SetUp()	override;
	virtual void Update()	override;
	virtual void Render()	override;
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

