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
	SYNTHESIZE(int, m_nCurMap, CurMap);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3*, m_pPosition, m_pPos);




public:
	cPlayer();
	virtual ~cPlayer();

	void SetUp();
	void Update();
	void Render();
	void AniControl();
	void Picking();
	void PlayerPosition();

	virtual void OnActionFinish(cAction* pSender) override;
};

