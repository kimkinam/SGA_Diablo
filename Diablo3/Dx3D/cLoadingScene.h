#pragma once

class cGrid;
class cBoss;

class cLoadingScene : public cSceneObject
{
private:
	cGrid*			m_pGrid;
	cBoss*			m_pBoss;
public:
	cLoadingScene();
	virtual ~cLoadingScene();

	
	//iScene override
	virtual HRESULT SetUp()	override;
	virtual void Update()	override;
	virtual void Render()	override;
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

