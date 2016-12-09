#pragma once

class cGameObject;

class cAIManager
{
private:
	// 기존 포인터들 저장
	typedef std::map<int, cGameObject*>				AIMap;
	typedef std::map<int, cGameObject*>::iterator	iter;

	AIMap	m_AImap;
	iter	m_iter;

public:
	cAIManager();
	cAIManager(const cAIManager&);
	~cAIManager();

<<<<<<< HEAD
	//cAIManager& operator=(const cAIManager&);
=======
	void Destroy();
	/*cAIBaseManager& operator=(const cAIBaseManager&);*/
>>>>>>> fd0862dac1e580af3d9fcbc3a09f36f02618a17e

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