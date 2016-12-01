#pragma once


class cLoadingScene : public cSceneObject
{
private:

public:
	cLoadingScene();
	virtual ~cLoadingScene();

	
	//iScene override
	virtual HRESULT SetUp()	override;
	virtual void Update()	override;
	virtual void Render()	override;
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

