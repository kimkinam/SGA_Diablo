#pragma once

class cGameObject;

class cAIManager
{
private:
	// 기존 포인터들 저장
	typedef std::map<int, cGameObject*>				AIMap;
	typedef std::map<int, cGameObject*>::iterator	iter;

	SYNTHESIZE(AIMap, m_AImap, AImap);
	iter	m_iter;
	

public:
	cAIManager();
	cAIManager(const cAIManager&);
	~cAIManager();

	//cAIManager& operator=(const cAIManager&);
	void Destroy();
	/*cAIBaseManager& operator=(const cAIBaseManager&);*/

	static cAIManager* Instance()
	{
		static cAIManager instance;
		return &instance;
	}

	void RegisterAIBase(cGameObject* NewAIBase);

	cGameObject* GetAIBaseFromID(int id);

	void RemoveAIBase(cGameObject* pAIBase);
};

#define g_pAIManager cAIManager::Instance()