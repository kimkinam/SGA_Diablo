#pragma once

class cAtion;
class cSkinnedMesh;

class cMonster : public cGameObject
{
private:
	D3DXMATRIX		m_matWorld;
	
	SYNTHESIZE(cSkinnedMesh*, m_pMesh, Mesh);
	SYNTHESIZE(MONSTER_STATE, m_emState, State); // 몬스터 상태
	SYNTHESIZE(ST_MONSTER_STAT, m_stStat, Stat); // 몬스터 스텟 ? 
	
	LPD3DXEFFECT				m_pEffect;

public:
	cMonster();
	virtual ~cMonster();

	void Setup(char* szMonsterName);
	void Update();
	void Render();

};

