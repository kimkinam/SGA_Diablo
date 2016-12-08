#pragma once

class iAI_Base;

class cAIManager
{
private:
	// 기존 포인터들 저장
	typedef std::map<int, iAI_Base*> AIMap;
	
	AIMap m_AImap;

public:
	cAIManager();
	cAIManager(const cAIManager&);
	~cAIManager();

	/*cAIBaseManager& operator=(const cAIBaseManager&);*/

	static cAIManager* Instance()
	{
		static cAIManager instance;
		return &instance;
	}

	void RegisterAIBase(iAI_Base* NewAIBase);

	iAI_Base* GetAIBaseFromID(int id);

	void RemoveAIBase(iAI_Base* pAIBase);
};

#define g_pAIManager cAIManager::Instance()