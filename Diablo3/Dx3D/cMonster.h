#pragma once

class cAtion;
class cSkinnedMesh;

class cMonster : public cGameObject
{
protected:
	D3DXMATRIX		m_matWorld;
	
	SYNTHESIZE(cSkinnedMesh*, m_pMesh, Mesh);
	SYNTHESIZE(MONSTER_STATE, m_emState, State); // 몬스터 상태
	SYNTHESIZE(ST_MONSTER_STAT, m_stStat, Stat); // 몬스터 스텟 ?
	SYNTHESIZE(cGameObject*, m_pTarget, Target);
	SYNTHESIZE(float, m_fTraceTime, TraceTime);

public:
	cMonster();
	virtual ~cMonster();

	virtual void Setup(char* szMonsterName);
	virtual void Update();
	virtual void Render();
	virtual void Trace();

};

