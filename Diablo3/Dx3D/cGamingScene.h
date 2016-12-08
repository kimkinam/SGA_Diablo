#pragma once

class cGrid;
class cUiManager;
class cCamera;
class cMap;
class cBoundBox;

class cGamingScene : public cSceneObject
{
private:
	cGrid*					m_pGrid;
	cUiManager*				m_pUIManager;
	std::vector<cMap*>		m_vecMap;
	std::vector<cBoundBox*>	m_vecBoundBox;

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

