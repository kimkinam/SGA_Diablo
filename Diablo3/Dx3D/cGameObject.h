#pragma once
#include "cObject.h"
#include "cAction.h"
#include "iActionDelegate.h"
#include "cSkinnedMesh.h"
#include "cAnimation.h"
#include "iAI_Base.h"

class cOBB;

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
	
private:
	//고유 식별 번호
	SYNTHESIZE(UINT, m_ID, ID);

	//다음 유효한 식별번호(총 갯수)
	static UINT m_INextID;

protected:
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

	
public:
	cGameObject();
	virtual ~cGameObject();

	//static UINT GetNextID() { return m_INextID; }
	
	virtual void Setup(D3DXVECTOR3* vLook = NULL);
	virtual void Update();
	virtual void Render();
	virtual void OnActionFinish(cAction* pSender) override;

	// 파생 클래스들은 메시지를 사용하여 통신할 수 있다
	virtual bool HandleMessage(const Telegram& msg);

	void SetNewDirection(D3DXVECTOR3 vDirection);
	D3DXVECTOR3* GetPtPosition(){ return &m_vPosition; }
	
};



