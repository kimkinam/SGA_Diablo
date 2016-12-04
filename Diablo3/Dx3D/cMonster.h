#pragma once

class cAtion;
class cSkinnedMesh;
class cPlayer;

class cMonster : public cGameObject
{
protected:
	D3DXMATRIX		m_matWorld;

	SYNTHESIZE(cSkinnedMesh*, m_pMesh, Mesh);

	SYNTHESIZE(MONSTER_STATE, m_emState, State); // ���� ����

	SYNTHESIZE(ST_MONSTER_STAT, m_stStat, Stat); // ���� ���� ?

	SYNTHESIZE(cGameObject*, m_pTarget, Target);

	SYNTHESIZE(float, m_fAttackRange, AttackRange);
	SYNTHESIZE(float, m_fTraceRange, TraceRange);
	SYNTHESIZE(float, m_fSpeed, Speed);

	LPD3DXMESH m_pAttackSphere;
	LPD3DXMESH m_pTraceSphere;


public:
	cMonster();
	virtual ~cMonster();


	virtual void Setup(char* szMonsterName);
	virtual void Update();
	virtual void Render();
	virtual void Trace();
	virtual void Attack();

};

