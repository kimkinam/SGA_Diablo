enum PLAYER_STATE
{
	PLAYER_IDLE_START,				//0
	PLAYER_IDLE,					//1
	PLAYER_MOVE_START,				//2
	PLAYER_MOVE,					//3
	PLAYER_ATTAACK1_START,			//4
	PLAYER_ATTACK1,					//5
	PLAYER_ATTACK2_START,			//6
	PLAYER_ATTACT2,					//7
	PLAYER_STATE_COUNT				//8
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