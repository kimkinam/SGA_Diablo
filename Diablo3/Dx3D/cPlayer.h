#pragma once

class cAtion;
class cSkinnedMesh;
class cObj;
class cTrailRenderer;
class cUIImage;


class cPlayer : public cGameObject
{
public:
	cStateMachine<cPlayer>* m_pSateMachnie;
private:
	double						m_dAttackStartTime;
	double						m_dAttackTermTime;
	cObj*						m_pSword;
	
	
	
	//SYNTHESIZE(PLAYER_STATE, m_emState, State);
	SYNTHESIZE(int, m_nCurMap, CurMap);

	cTrailRenderer* m_pTrailRenderer;

	vector <LPDIRECT3DTEXTURE9> m_vecTrailTex;
	int m_nTrailIndex;
	
	
	cUIImage*					m_pSphere;


public:
	cPlayer();
	virtual ~cPlayer();

	virtual void Setup(D3DXVECTOR3* vLook = NULL);
	virtual void Update();
	virtual void Render();
	virtual void PlayerPosition();


public:
	void TrailRender();
	void TrailUpdate();
	void TrailTexSetUp(const char* texFileName);


	// �Ļ� Ŭ�������� �޽����� ����Ͽ� ����� �� �ִ�
	virtual bool HandleMessage(const Telegram& msg);
};

