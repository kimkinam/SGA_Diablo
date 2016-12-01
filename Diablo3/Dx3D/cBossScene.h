#pragma once

class cGrid;
class cBoss;

class cBossScene :
	public cSceneObject
{
private:
	cGrid*		m_pGrid;
	//cBoss*		Boss_diablo;
public:
	cBossScene();
	virtual ~cBossScene();

	virtual HRESULT SetUp()	override;
	virtual HRESULT Reset()	override;
	virtual void Update()	override;
	virtual void Render()	override;
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

