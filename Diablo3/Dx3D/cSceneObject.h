#pragma once

class cCamera;

class cSceneObject
{
private:
	SYNTHESIZE(bool, m_bIsLoad, IsLoad);
	cCamera*	m_pCamera;

public:
	cSceneObject();
	virtual ~cSceneObject();

	virtual HRESULT SetUp();
	virtual HRESULT Reset();
	virtual void Release();
	virtual void Update();
	virtual void Render();
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;

};

