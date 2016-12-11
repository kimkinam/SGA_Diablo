#pragma once

#define g_pAutoReleasePool cAutoReleasePool::GetInstance()

class cObject;

class cAutoReleasePool
{
private:
	std::set<cObject*> m_setObjectPool;

	SINGLETONE(cAutoReleasePool);
public:

	void AddObject(cObject* pObject);
	void Drain();
};

