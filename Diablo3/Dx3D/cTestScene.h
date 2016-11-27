#pragma once

class cGrid;
class cSkinnedMesh;
class cObj;
class cCamera;
class cMtlTex;

class cTestScene : public cSceneObject, public iButtonDelegate
{
private:
	cGrid*						m_pGrid;
	cCamera*					m_pCamera;
	
	//�÷��̾�
	cSkinnedMesh*				m_pMesh;
	cObj*						m_pSword;

	//���� ������Ʈ
	cObj*						m_pCurObj;

	//������Ʈ ����
	std::vector<cObj*>			m_vecObj;

	//�� ��ġ ���� Ÿ��
	std::vector<ST_PC_VERTEX>	m_vecTiles;
	bool						m_bIsSetMap;

	//UI
	LPD3DXSPRITE				m_pSprite;
	cUIObject*					m_pUIRoot;
public:
	cTestScene();
	virtual ~cTestScene();

	//SceneObject override
	virtual HRESULT SetUp()	override;
	virtual void Release()	override;
	virtual void Update()	override;
	virtual void Render()	override;
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	//iButtonDelegate override
	virtual void OnClick(cUIButton* pSender) override;

};

