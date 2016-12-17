#pragma once

class cSceneObject;

class cMainGame
{
public:
	bool	m_bIsLoad;
public:
	cMainGame(void);
	~cMainGame(void);

	void Setup();
	void Update();
	void Render();
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void SetLight();

	static void LoadScenes(LPVOID IpParam);
};

