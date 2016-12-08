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
	for each(auto c in m_AImap)
	{
		SAFE_DELETE(c.second);
	}
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

void cAIManager::RegisterAIBase(iAI_Base* NewAIBase)
{
	m_AImap[NewAIBase->GetID()] = NewAIBase;
}

iAI_Base* cAIManager::GetAIBaseFromID(int id)
{
	if (m_AImap.find(id) == m_AImap.end()) 
	{
		return NULL;
	}
	return m_AImap[id];
}

void cAIManager::RemoveAIBase(iAI_Base* pAIBase)
{

}