#include "stdafx.h"
#include "cAIManager.h"

cAIManager::cAIManager()
{

}
cAIManager::cAIManager(const cAIManager&)
{

}
cAIManager::~cAIManager()
{
	//for each(auto c in m_AImap)
	//{
	//	SAFE_DELETE(c.second);
	//}
}

//cAIBaseManager& cAIBaseManager::operator=(const cAIBaseManager&)
//{
//	return 
//}

static cAIManager* Instance()
{
	static cAIManager instance;
	return &instance;
}

void cAIManager::RegisterAIBase(cGameObject* NewAIBase)
{
	SAFE_ADDREF(NewAIBase);
	m_AImap[NewAIBase->GetID()] = NewAIBase;
}

cGameObject* cAIManager::GetAIBaseFromID(int id)
{
	if (m_AImap.find(id) == m_AImap.end()) 
	{
		return NULL;
	}
	return m_AImap[id];
}

void cAIManager::RemoveAIBase(cGameObject* pAIBase)
{
	for (m_iter = m_AImap.begin(); m_iter != m_AImap.end();)
	{
		if (m_iter->second == pAIBase)
		{
			SAFE_RELEASE(pAIBase);
			m_AImap.erase(m_iter++);
		}
		else
			m_iter++;
	}
}