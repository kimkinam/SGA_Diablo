#pragma once

class cGrid;
class cSkinnedMesh;
class cObj;
class cMap;
class cPlayer;
class cMonster;
class cBoundBox;
class cBoss;

class cTestScene : public cSceneObject, public iButtonDelegate
{
private:
	cGrid*						m_pGrid;

	cPlayer*					m_pPlayer;

	std::vector<cMonster*>		m_vecMonster;

	//현재 오브젝트
	cMap*						m_pCurObj;
	int							m_nCurIndex;

	//오브젝트 벡터
	std::vector<cMap*>			m_vecObj;
	std::vector<cMap*>			m_vecMap;

	//바운드 박스
	std::vector<cOBB*>			m_vecBoundBox;

	//맵 위치 정할 타일
	std::vector<ST_PC_VERTEX>	m_vecTiles;
	bool						m_bIsSetMap;
	bool						m_bIsBound;
	bool						m_bIsDone;
	D3DXVECTOR3					m_vpickPos;

	//UI
	LPD3DXSPRITE				m_pSprite;

	std::vector<cUIObject*>		m_vecObjUI;
	std::vector<cUIObject*>		m_vecMonsterUI;
	cUIObject*					m_pUIRoot;
	cUIObject*					m_pMonsterUI;
	cUIObject*					m_pObjUI;


	D3DXVECTOR3					m_vMin;
	D3DXVECTOR3					m_vMax;

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
	void SetBoundBox();

	bool CollisionTest();
	void Save(string fileName);
	void Load(string fileName);


};

