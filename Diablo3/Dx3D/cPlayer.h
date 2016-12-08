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

	virtual void Setup(D3DXVECTOR3* vLook = NULL);
	virtual void Update();
	virtual void Render();
	virtual void AniControl();
	virtual void Picking();
	virtual void PlayerPosition();

	virtual void OnActionFinish(cAction* pSender) override;
};

