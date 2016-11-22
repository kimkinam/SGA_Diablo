#pragma once

class cMonsterManager;

class cPlayerManager
{
private:
	cMonsterManager*	m_pMonsterManager;
public:
	cPlayerManager();
	~cPlayerManager();

	void Setup();
	void Update();
	void Render();

	void SetAddressLink(cMonsterManager* cm)
	{
		assert(cm != NULL && "���͸Ŵ��� ����.");
		m_pMonsterManager = cm;
	}
};

