enum MONSTER_STATE
{
	MONSTER_IDLE,
	MONSTER_MOVE,
	MONSTER_SKILL1,
	MONSTER_SKILL2,
	MONSTER_SKILL3,
	MONSTER_SKILL4,
	MONSTER_SKILL5,
	MONSTER_KNOCKBACK,
	MONSTER_GETHIT,
	MONSTER_STUNNED,
	MONSTER_DEAD,
};

struct ST_MONSTER_STAT
{
	float fHp;
	float fMp;
	float fAtk;
	float fDef;
	float fStr;
	float fDex;
	float fInt;

	ST_MONSTER_STAT() { fHp = fMp = fAtk = fDef = fStr = fDex = fInt = 0.0f; }
};