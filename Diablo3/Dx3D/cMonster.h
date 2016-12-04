#pragma once

class cAtion;
class cSkinnedMesh;
class cPlayer;

class cMonster : public cGameObject
{
protected:
	D3DXMATRIX		m_matWorld;

	SYNTHESIZE(cSkinnedMesh*, m_pMesh, Mesh);

	SYNTHESIZE(MONSTER_STATE, m_emState, State); // 몬스터 상태

	SYNTHESIZE(ST_MONSTER_STAT, m_stStat, Stat); // 몬스터 스텟 ?

	SYNTHESIZE(cGameObject*, m_pTarget, Target);

	SYNTHESIZE(float, m_fAttackRange, AttackRange);
	SYNTHESIZE(float, m_fTraceRange, TraceRange);
	SYNTHESIZE(float, m_fSpeed, Speed);

	LPD3DXMESH m_pAttackSphere;
	LPD3DXMESH m_pTraceSphere;

<<<<<<< HEAD

=======
	double p;
	LPD3DXANIMATIONSET pCurAS;
>>>>>>> 491dfae5fe567714700bbb46b0294cd6a68cb1c9
public:
	cMonster();
	virtual ~cMonster();


	virtual void Setup(char* szMonsterName);
	virtual void Update();
	virtual void Render();
	virtual void Trace();
	virtual void Attack();

};

