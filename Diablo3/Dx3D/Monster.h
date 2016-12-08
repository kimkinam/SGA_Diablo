enum MONSTER_STATE
{
	MONSTER_IDLE_START,					//0
	MONSTER_IDLE,						//1
	MONSTER_TRACE_START,				//2
	MONSTER_TRACE,						//3
	MONSTER_ATTACK_START,				//4
	MONSTER_ATTACK,						//5
	MONSTER_HITTED_START,				//6
	MONSTER_HITTED,						//7
	MONSTER_KNOCKBACK_START,			//8
	MONSTER_KNOCKBACK,					//9
	MONSTER_STUNNED_START,				//10
	MONSTER_STUNNED,					//11
	MONSTER_DEAD_START,					//12
	MONSTER_DEAD,						//13
};

struct ST_MONSTER_STAT
{
	float fHp;
	float fMp;
	float fAtk;
	float fDef;

	float fSpeed;
	float fAttackRange;
	float fTraceRange;

	ST_MONSTER_STAT() { fHp = fMp = fAtk = fDef
		= fSpeed = fAttackRange = fTraceRange = 0.0f; }

	ST_MONSTER_STAT(float _fHp, float _fMp, float _fAtk, float _fDef,
		float _fSpeed, float _fAttackRange, float _fTraceRange) 
	{
		fHp				= _fHp;
		fMp				= _fMp;
		fAtk			= _fAtk;
		fDef			= _fDef;
		fSpeed			= _fSpeed;
		fAttackRange	= _fAttackRange;
		fTraceRange		= _fTraceRange;
	}
};