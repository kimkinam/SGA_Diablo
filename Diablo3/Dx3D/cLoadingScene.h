#pragma once


class cLoadingScene : public iScene
{
private:

public:
	cLoadingScene();
	~cLoadingScene();

	
	//iScene override
	virtual void SetUp() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

