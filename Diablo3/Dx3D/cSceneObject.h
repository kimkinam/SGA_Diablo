#pragma once

class cSceneObject : public cObject
{
public:
	cSceneObject();
	virtual ~cSceneObject();

	virtual HRESULT SetUp() = 0;
	virtual void Release() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;

};

