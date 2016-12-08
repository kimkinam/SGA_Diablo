#pragma once

class cGrid;
class cSkinnedMesh;
class cObj;
class cMap;
class cPlayer;
class cMonster;
class cBoundBox;
class cBoss;

enum TOOL_STATE
{
	SET_MAP,
	SET_MONSTER,
	SET_BOX,
	SET_DONE,
};

class cTestScene : public cSceneObject, public iButtonDelegate
{
private:
	cGrid*						m_pGrid;

	cPlayer*					m_pPlayer;
	D3DXMATRIXA16				m_matR;
	
	

	//현재 오브젝트
	cMap*						m_pCurObj;
	cMonster*					m_pCurMonster;
	int							m_nCurIndex;

	//오브젝트 벡터
	std::vector<cMap*>			m_vecObj;
	std::vector<cMap*>			m_vecOutMap;
	std::vector<cMonster*>		m_vecMonster;
	std::vector<cMonster*>		m_vecOutMonster;

	//바운드 박스
	std::vector<cOBB*>			m_vecBoundBox;

	//맵 위치 정할 타일
	std::vector<ST_PC_VERTEX>	m_vecTiles;

	TOOL_STATE					m_emState;
	bool						m_bisClone;
	bool						m_bIsBound;
	D3DXVECTOR3					m_vpickPos;

	//UI
	LPD3DXSPRITE				m_pSprite;

	std::vector<cUIObject*>		m_vecObjUI;
	std::vector<cUIObject*>		m_vecMonsterUI;
	cUIObject*					m_pUIRoot;


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
	void DebugRender();
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) ;

	//iButtonDelegate override
	virtual void OnClick(cUIButton* pSender) override;

	bool InCollider(cUIObject * pUI);

	void SetMap();
	void StateChange();
	void PlayerMoveTest();
	void SetBoundBox();

	bool CollisionTest();
	void Save(string fileName);
	void Load(string fileName);


};

