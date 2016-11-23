#pragma once



class cMainGame
{
private:
	iScene*		m_pCurScene;
	iScene*		m_pGamingScene;
	iScene*		m_pLoadingScene;

public:
	cMainGame(void);
	~cMainGame(void);

	void Setup();
	void Update();
	void Render();
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void SetLight();
};

