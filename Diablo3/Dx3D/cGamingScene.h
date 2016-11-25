#pragma once

class cGrid;
class iMap;
class cUiManager;
class cMonsterManager;
class cPlayerManager;
class cCamera;

class cGamingScene : public cSceneObject
{
private:
	cCamera*				m_pCamera;
	cGrid*					m_pGrid;
	iMap*					m_pMap;
	cUiManager*				m_pUIManager;
	cMonsterManager*		m_pMonsterManager;
	cPlayerManager*			m_pPlayerManager;

public:
	cGamingScene();
	virtual ~cGamingScene();

	//iScene override;
	virtual HRESULT SetUp()	override;
	virtual void Release()	override;
	virtual void Update()	override;
	virtual void Render()	override;
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

