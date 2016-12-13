#pragma once
#include "cObject.h"
#include "cAction.h"
#include "iActionDelegate.h"
#include "cSkinnedMesh.h"
#include "cAnimation.h"

class cOBB;
template<typename T> class cStateMachine;
enum CHARACTER_TYPE
{
	CHARACTER_BARBARIAN,
	CHARACTER_DIABLO,
	CHARACTER_FETISH,
	CHARACTER_SKELETON,
	CHARACTER_SKELETONARCHER,
	CHARACTER_STITCH,
	CHARACTER_GARHANTUAN,
	CHARACTER_ZOMBIEDOG,
	CHARACTER_NONE
};

struct ST_MONSTER_STAT
{
	CHARACTER_TYPE chType;

	float fMaxHp;
	float fMaxMp;
	float fHp;
	float fMp;
	float fAtk;
	float fDef;

	float fSpeed;
	float fAttackRange;
	float fTraceRange;

	bool bIsDead;

	ST_MONSTER_STAT() {
		fHp = fMp = fAtk = fDef
			= fSpeed = fAttackRange = fTraceRange = 0.0f;
		chType = CHARACTER_NONE;
		bIsDead = false;
	}

	ST_MONSTER_STAT(float _fHp, float _fMp, float _fAtk, float _fDef,
		float _fSpeed, float _fAttackRange, float _fTraceRange, CHARACTER_TYPE _chType)
	{
		fHp = _fHp;
		fMp = _fMp;
		fAtk = _fAtk;
		fDef = _fDef;
		fSpeed = _fSpeed;
		fAttackRange = _fAttackRange;
		fTraceRange = _fTraceRange;
		chType = _chType;
		bIsDead = false;
	}
};

class cGameObject : public cObject, public iActionDelegate
{
public:
	SYNTHESIZE_PASS_BY_REF(ST_MONSTER_STAT, m_stStat, Stat);
private:
	//���� �ĺ� ��ȣ
	SYNTHESIZE(UINT, m_ID, ID);

	//���� ��ȿ�� �ĺ���ȣ(�� ����)
	static UINT m_INextID;

protected:
	//cStateMachine<cGameObject>* m_pSateMachnie;
	

	SYNTHESIZE(D3DXVECTOR3, m_vPosition, Position);
	SYNTHESIZE(D3DXVECTOR3, m_vDirection, Direction);
	SYNTHESIZE(D3DXVECTOR3, m_vUp, UpVector);
	SYNTHESIZE(D3DXVECTOR3, m_vRight, Right);
	SYNTHESIZE(D3DXVECTOR3, m_vScale, Scale);
	SYNTHESIZE(cOBB*, m_pOBB, OBB);
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIX, m_matLocal, LocalTM);
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIX, m_matWorld, WorldTM);

	SYNTHESIZE_ADD_REF(cAction*, m_pAction, Action);
	SYNTHESIZE(cGameObject*, m_pTarget, Target);
	SYNTHESIZE(cSkinnedMesh*, m_pMesh, Mesh);

	SYNTHESIZE_PASS_BY_REF(std::vector<cOBB*>, m_vecBoundBox, BoundBox);

	
	LPD3DXMESH	m_pSphere;
	
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
	
	

	//�ִϸ��̼� ����
	bool IsDoneCurAni();
	double GetCurAniTime();
	double GetAniTimeWithName(string szAniName);
	LPD3DXANIMATIONSET GetCurAnimation();
	void SetAnimation(int nIndex);
	void SetAnimation(string szAniName);
};



