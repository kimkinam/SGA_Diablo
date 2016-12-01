#pragma once

class cGrid;
class cSkinnedMesh;
class cObj;
class cMap;
class cPlayer;
class cMonster;

class cTestScene : public cSceneObject, public iButtonDelegate
{
private:
	cGrid*						m_pGrid;

	cPlayer*					m_pPlayer;
	cMonster*					m_pMonster;

	//현재 오브젝트
	cMap*						m_pCurObj;

	//오브젝트 벡터
	std::vector<cMap*>			m_vecObj;
	std::vector<cMap*>			m_vecMap;


	//맵 위치 정할 타일
	std::vector<ST_PC_VERTEX>	m_vecTiles;
	bool						m_bIsSetMap;
	D3DXVECTOR3					m_vpickPos;

	//UI
	LPD3DXSPRITE				m_pSprite;
	std::vector<cUIObject*>		m_vecObjUI;
	cUIObject*					m_pUIRoot;
public:
	cTestScene();
	virtual ~cTestScene();

	//SceneObject override
	virtual HRESULT SetUp();
	virtual HRESULT Reset();
	virtual void Update();
	virtual void Render();
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) ;

	//iButtonDelegate override
	virtual void OnClick(cUIButton* pSender) override;

	bool InCollider(cUIObject * pUI);

	void SetMap();
	void PlayerMoveTest();


};

