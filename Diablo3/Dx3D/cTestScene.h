#pragma once

class cGrid;
class cSkinnedMesh;
class cObj;
class cCube;
class cCamera;

class cTestScene : public cSceneObject
{
private:
	cGrid*					m_pGrid;
	cSkinnedMesh*			m_pMesh;
	cObj*					m_pSword;
	cCamera*				m_pCamera;
	cCube*					m_pCube;
public:
	cTestScene();
	virtual ~cTestScene();

	virtual HRESULT SetUp()	override;
	virtual void Release()	override;
	virtual void Update()	override;
	virtual void Render()	override;
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

};

