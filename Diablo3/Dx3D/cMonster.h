#pragma once

class cAtion;
class cSkinnedMesh;
class cPlayer;

class cMonsterGlobalState;
class cMonsterTrace;

class cMonster : public cGameObject
{

public :
	cStateMachine<cMonster>* m_pSateMachnie;
protected:
	

	
	LPD3DXMESH m_pAttackSphere;
	LPD3DXMESH m_pTraceSphere;

private:
	SYNTHESIZE(string, m_sSumNailName, SumNailName);
	SYNTHESIZE(string, m_sObjName, ObjName);
	SYNTHESIZE(string, m_sFolderName, FolderName);

public:
	cMonster();
	virtual ~cMonster();

	virtual void Setup(char* szMonsterName, D3DXVECTOR3* vLookAt = NULL);
	virtual void Setup(ST_SAVEOBJECT wObj);
	virtual void Update();
	virtual void Render();
	//virtual void CloneMonster(cMonster* monster);

	virtual void OnActionFinish(cAction* pSender) override;

	// 파생 클래스들은 메시지를 사용하여 통신할 수 있다
	virtual bool HandleMessage(const Telegram& msg);
};

