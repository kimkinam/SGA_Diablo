#pragma once

class cAtion;
class cSkinnedMesh;
class cObj;
class cTrailRenderer;
class cUIImage;

#define HP_SPHERE_SCALE	 2
#define HP_SPHERE_SIZE_X 48 * HP_SPHERE_SCALE
#define HP_SPHERE_SIZE_Y 49 * HP_SPHERE_SCALE

struct ST_HPSPHERE
{
	int nCount;
	int nIndex;
	DWORD dwAniTime;
	DWORD dwOldAniTime;
	std::vector<RECT> pRect;
	std::vector<D3DXVECTOR3> pCenter;
};

class cPlayer : public cGameObject
{
public:
	cStateMachine<cPlayer>* m_pSateMachnie;
private:
	double			m_dAttackStartTime;
	double			m_dAttackTermTime;
	cObj*			m_pSword;
	ST_HPSPHERE				m_Hp;
	LPDIRECT3DTEXTURE9			m_pHpTex;
	ST_HPSPHERE					m_Mp;
	LPDIRECT3DTEXTURE9			m_pMpTex;
	
	
	//SYNTHESIZE(PLAYER_STATE, m_emState, State);
	SYNTHESIZE(int, m_nCurMap, CurMap);

	cTrailRenderer* m_pTrailRenderer;

	vector <LPDIRECT3DTEXTURE9> m_vecTrailTex;
	int m_nTrailIndex;
	LPD3DXSPRITE				m_pSprite;
	
	cUIImage*					m_pSphere;


public:
	cPlayer();
	virtual ~cPlayer();

	virtual void Setup(D3DXVECTOR3* vLook = NULL);
	virtual void Update();
	virtual void Render();
	virtual void PlayerPosition();

	virtual void OnActionFinish(cAction* pSender) override;

public:
	void TrailUpdate();
	void TrailTexSetUp(const char* texFileName);
	void SetSphere(ST_HPSPHERE& sphere);

	// 파생 클래스들은 메시지를 사용하여 통신할 수 있다
	virtual bool HandleMessage(const Telegram& msg);
};

