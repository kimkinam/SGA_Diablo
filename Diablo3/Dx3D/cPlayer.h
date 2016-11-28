#pragma once

class cAtion;
class cSkinnedMesh;
class cObj;

class cPlayer : public cGameObject
{
private:
	D3DXMATRIX		m_matWorld;
	double			m_dAttackStartTime;
	double			m_dAttackTermTime;
	cObj*			m_pSword;

	SYNTHESIZE(bool, m_bIsAtk, IsAtk);
	SYNTHESIZE(cSkinnedMesh*, m_pMesh, Mesh);
	SYNTHESIZE(PLAYER_STATE, m_emState, State);


public:
	cPlayer();
	~cPlayer();

	void SetUp();
	void Update();
	void Render();
	void AniControl();
	void Picking();

	virtual void OnActionFinish(cAction* pSender) override;
};

