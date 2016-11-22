#pragma once

class cPlayerManager;

class cMonsterManager
{
private:
	cPlayerManager*		m_pPlayerManager;
public:
	cMonsterManager();
	~cMonsterManager();

	void Setup();
	void Update();
	void Render();

	void SetAddressLink(cPlayerManager* pm)
	{
		assert(pm != NULL && "�÷��̾�Ŵ��� ����.");
		m_pPlayerManager = pm;
	}
};

