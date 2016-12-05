#pragma once

class cAtion;
class cSkinnedMesh;
class cObj;



class cPlayer : public cGameObject
{
private:
	double			m_dAttackStartTime;
	double			m_dAttackTermTime;
	cObj*			m_pSword;
	

	
	SYNTHESIZE(bool, m_bIsAtk, IsAtk);
	
	//SYNTHESIZE(PLAYER_STATE, m_emState, State);
	SYNTHESIZE(int, m_nCurMap, CurMap);
	SYNTHESIZE(D3DXVECTOR3, m_pPosition, m_pPos);




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

