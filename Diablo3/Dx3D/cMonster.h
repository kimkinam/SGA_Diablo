#pragma once

class cAtion;
class cSkinnedMesh;
class cPlayer;

class cMonster : public cGameObject
{
protected:
	//SYNTHESIZE(MONSTER_STATE, m_emState, State); // 몬스터 상태

	SYNTHESIZE(ST_MONSTER_STAT, m_stStat, Stat); // 몬스터 스텟 ?

	SYNTHESIZE_ADD_REF(cGameObject*, m_pTarget, Target);

	SYNTHESIZE(float, m_fAttackRange, AttackRange);
	SYNTHESIZE(float, m_fTraceRange, TraceRange);
	SYNTHESIZE(float, m_fSpeed, Speed);

	LPD3DXMESH m_pAttackSphere;
	LPD3DXMESH m_pTraceSphere;

public:
	cMonster();
	virtual ~cMonster();


	virtual void Setup(char* szMonsterName, D3DXVECTOR3* vLookAt = NULL);
	virtual void Update();
	virtual void Render();
	virtual void Trace();
	virtual void Attack();

virtual void OnActionFinish(cAction* pSender) override;

};

