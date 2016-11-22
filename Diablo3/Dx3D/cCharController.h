#pragma once

class iMap;
class cAction;


class cCharController : public cGameObject
{
private:
	bool						m_fFalling;
	float						m_fAngle;
	float						m_fSpeed;
	iMap*						m_pMap;
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIXA16, m_matWorld, WorldTM);
	
	SYNTHESIZE(cAction*, m_pAction, Action);

public:
	SYNTHESIZE(PLAYER_STATE, m_emState, State);
public:
	cCharController(void);
	~cCharController(void);

	void Update(iMap* map = NULL);

	virtual void OnActionFinish(cAction* pSender) override;


};

