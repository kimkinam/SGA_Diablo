#pragma once

class cAtion;
class cSkinnedMesh;

class cMonster : public cGameObject
{
private:
	D3DXMATRIX		m_matWorld;
	
	SYNTHESIZE(cSkinnedMesh*, m_pMesh, Mesh);
	SYNTHESIZE(MONSTER_STATE, m_emState, State);
	SYNTHESIZE(ST_MONSTER_STAT, m_stStat, Stat);

public:
	cMonster();
	~cMonster();

	void Setup(char* szMonsterName);
	void Update();
	void Render();

};

