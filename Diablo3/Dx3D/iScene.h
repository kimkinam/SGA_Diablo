#pragma once

class cCamera;

class iScene
{
protected:
	cCamera*		m_pCamera;

public:
	iScene();
	virtual ~iScene();

	virtual void SetUp()	= 0;
	virtual void Update()	= 0;
	virtual void Render()	= 0;
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;

};

