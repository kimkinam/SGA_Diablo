enum PLAYER_STATE
{
	PLAYER_IDLE,
	PLAYER_MOVE,
	PLAYER_ATTACK1,
	PLAYER_ATTACT2,
	PLAYER_STATE_COUNT
};

struct ST_PLAYER_STAT
{
	float fHp;
	float fMp;
	float fAtk;
	float fDef;
	float fStr;
	float fDex;
	float fInt;

	ST_PLAYER_STAT() { fHp = fMp = fAtk = fDef = fStr = fDex = fInt = 0.0f; }
};