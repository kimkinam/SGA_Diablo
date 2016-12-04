#pragma once

class cAtion;
class cSkinnedMesh;

class cMonster : public cGameObject
{
protected:
	D3DXMATRIX		m_matWorld;

	SYNTHESIZE(cSkinnedMesh*, m_pMesh, Mesh);

	SYNTHESIZE(MONSTER_STATE, m_emState, State); // 몬스터 상태
<<<<<<< HEAD

	SYNTHESIZE(ST_MONSTER_STAT, m_stStat, Stat); // 몬스터 스텟 ?

	SYNTHESIZE(cGameObject*, m_pTarget, Target);

	SYNTHESIZE(float, m_fAttackRange, AttackRange);
	SYNTHESIZE(float, m_fTraceRange, TraceRange);
	SYNTHESIZE(float, m_fSpeed, Speed);

	LPD3DXMESH m_pAttackSphere;
	LPD3DXMESH m_pTraceSphere;
=======
	SYNTHESIZE(ST_MONSTER_STAT, m_stStat, Stat); // 몬스터 스텟 ?
	SYNTHESIZE(cGameObject*, m_pTarget, Target);
	SYNTHESIZE(float, m_fTraceTime, TraceTime);
>>>>>>> 65ab56b83d0f8d2a0d2f65015c3637a9a04789e0

public:
	cMonster();
	virtual ~cMonster();

	virtual void Setup(char* szMonsterName);
	virtual void Update();
	virtual void Render();
	virtual void Trace();
<<<<<<< HEAD
	virtual void Attack();
=======
>>>>>>> 65ab56b83d0f8d2a0d2f65015c3637a9a04789e0

};

