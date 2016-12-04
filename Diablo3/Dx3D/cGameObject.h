#pragma once
#include "cObject.h"
#include "cAction.h"
#include "iActionDelegate.h"
#include "cSkinnedMesh.h"
#include "cAnimation.h"


class cGameObject : public cObject, public iActionDelegate
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
protected:
	D3DXVECTOR3 m_vPrevPosition;
	SYNTHESIZE(D3DXVECTOR3, m_vPosition, Position);
	SYNTHESIZE(D3DXVECTOR3, m_vDirection, Direction);
	SYNTHESIZE(D3DXVECTOR3, m_vUp, UpVector);
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIXA16, m_matWorld, WorldTM);

	SYNTHESIZE_ADD_REF(cAction*, m_pAction, Action);
	SYNTHESIZE(float, m_fAngle, Angle);
	SYNTHESIZE(bool, m_bIsMove, IsMove);
	SYNTHESIZE(bool, m_bIsAtk, IsAtk);
	SYNTHESIZE(STATE, m_emState, State);
	SYNTHESIZE(cSkinnedMesh*, m_pMesh, Mesh);
	SYNTHESIZE_PASS_BY_REF(cAnimation*, m_pAni, Ani);

	float time;

public:
	cGameObject();
	virtual ~cGameObject();
	
	virtual void Setup(D3DXVECTOR3* vLook = NULL);
	virtual void Update();
	virtual void Render();
	virtual void OnActionFinish(cAction* pSender) override;

	D3DXVECTOR3* GetPtPosition(){ return &m_vPosition; }
	
};

