#pragma once

class cGameObject;

class cActionTrace : public cAction
{
public:
	float angle;
private:
	SYNTHESIZE_PASS_BY_REF(std::vector<cOBB*>, m_vecOBB, OBB);
	SYNTHESIZE_PASS_BY_REF(std::vector<cGameObject*>, m_vecMonster, Monster);
	SYNTHESIZE(float, m_fSpeed, Speed);
	//������Ÿ�
	SYNTHESIZE(float, m_fTraceRange, TraceRange);
	//���ݻ�Ÿ�
	SYNTHESIZE(float, m_fAttackRange, AttackRange);
	SYNTHESIZE(bool, m_bIsAtk, IsAtk);

	SYNTHESIZE(cGameObject*, m_pTraceTarget, TraceTarget);

	//Ÿ�ٰ��� �Ÿ�
	SYNTHESIZE(float, m_fDistance, Distance);
	SYNTHESIZE(D3DXVECTOR3, m_vDirToTarget, DirToTarget);

	SYNTHESIZE(D3DXVECTOR3, m_vDirToTargetX, DirToTargetX);
	SYNTHESIZE(D3DXVECTOR3, m_vDirToTargetZ, DirToTargetZ);
public:
	cActionTrace();
	virtual~cActionTrace();

	static cAction* Create();

	virtual void Start() override;
	virtual void Update() override;

};

