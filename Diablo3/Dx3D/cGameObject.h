#pragma once
#include "cObject.h"
#include "cAction.h"
#include "iActionDelegate.h"
#include "cSkinnedMesh.h"
#include "cAnimation.h"


class cOBB;

<<<<<<< HEAD
class cGameObject : public cObject//, public iActionDelegate
=======
template<typename T> class cStateMachine;

struct ST_MONSTER_STAT
{
	float fHp;
	float fMp;
	float fAtk;
	float fDef;

	float fSpeed;
	float fAttackRange;
	float fTraceRange;

	ST_MONSTER_STAT() {
		fHp = fMp = fAtk = fDef
			= fSpeed = fAttackRange = fTraceRange = 0.0f;
	}

	ST_MONSTER_STAT(float _fHp, float _fMp, float _fAtk, float _fDef,
		float _fSpeed, float _fAttackRange, float _fTraceRange)
	{
		fHp = _fHp;
		fMp = _fMp;
		fAtk = _fAtk;
		fDef = _fDef;
		fSpeed = _fSpeed;
		fAttackRange = _fAttackRange;
		fTraceRange = _fTraceRange;
	}
};

class cGameObject : public cObject, public iActionDelegate
>>>>>>> fd0862dac1e580af3d9fcbc3a09f36f02618a17e
{
public:
	enum STATE
	{
		IDLE_START,					//0
		IDLE,						//1
		TRACE_START,				//2
		TRACE,						//3
		MOVE_START,					//4	
		MOVE,						//5
		ATTACK_START,				//6
		ATTACK,						//7
		HITTED_START,				//8
		HITTED,						//9
		KNOCKBACK_START,			//10
		KNOCKBACK,					//11
		STUNNED_START,				//12
		STUNNED,					//13
		DEAD_START,					//14
		DEAD,						//15
	};
	SYNTHESIZE(ST_MONSTER_STAT, m_stStat, Stat);
private:
	//���� �ĺ� ��ȣ
	SYNTHESIZE(UINT, m_ID, ID);

	//���� ��ȿ�� �ĺ���ȣ(�� ����)
	static UINT m_INextID;

protected:
	//cStateMachine<cGameObject>* m_pSateMachnie;
	SYNTHESIZE_ADD_REF(cGameObject*, m_pTarget, Target);

	D3DXVECTOR3 m_vPrevPosition;
	SYNTHESIZE(D3DXVECTOR3, m_vPosition, Position);
	SYNTHESIZE(D3DXVECTOR3, m_vDirection, Direction);
	SYNTHESIZE(D3DXVECTOR3, m_vUp, UpVector);
	SYNTHESIZE(D3DXVECTOR3, m_vRight, Right);
	SYNTHESIZE(D3DXVECTOR3, m_vScale, Scale);
	SYNTHESIZE(cOBB*, m_pOBB, OBB);
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIX, m_matLocal, LocalTM);
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIX, m_matWorld, WorldTM);

	SYNTHESIZE_ADD_REF(cAction*, m_pAction, Action);
	SYNTHESIZE(float, m_fAngle, Angle);
	SYNTHESIZE(bool, m_bIsMove, IsMove);
	SYNTHESIZE(bool, m_bIsAtk, IsAtk);
	SYNTHESIZE(STATE, m_emState, State);
	SYNTHESIZE(cSkinnedMesh*, m_pMesh, Mesh);

	cStateMachine<cGameObject>* m_pSateMachnie;
public:
	cGameObject();
	virtual ~cGameObject();

	//static UINT GetNextID() { return m_INextID; }
	
	virtual void Setup(D3DXVECTOR3* vLook = NULL);
	virtual void Update();
	virtual void Render();
	virtual void OnActionFinish(cAction* pSender) override;

	// �Ļ� Ŭ�������� �޽����� ����Ͽ� ����� �� �ִ�
	virtual bool HandleMessage(const Telegram& msg);

	void SetNewDirection(D3DXVECTOR3 vDirection);
	D3DXVECTOR3* GetPtPosition(){ return &m_vPosition; }
	
};



