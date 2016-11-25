#pragma once

class cGrid;
class cSkinnedMesh;
class cObj;
class cCube;

class cTestScene : public iScene
{
private:
	cGrid*					m_pGrid;
	cSkinnedMesh*			m_pMesh;
	cObj*					m_pSword;

	cCube*					m_pCube;
public:
	cTestScene();
	~cTestScene();

	virtual void SetUp()	override;
	virtual void Update()	override;
	virtual void Render()	override;
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

};

