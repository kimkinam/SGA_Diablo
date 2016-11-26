#pragma once

class cGrid;
class cSkinnedMesh;
class cObj;
class cCamera;
class cMtlTex;

class cTestScene : public cSceneObject
{
private:
	cGrid*					m_pGrid;
	cSkinnedMesh*			m_pMesh;
	cObj*					m_pSword;
	cObj*					m_pMap;
	cCamera*				m_pCamera;
public:
	cTestScene();
	virtual ~cTestScene();

	virtual HRESULT SetUp()	override;
	virtual void Release()	override;
	virtual void Update()	override;
	virtual void Render()	override;
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

};

