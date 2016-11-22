#pragma once

// ���� �Ŵ��� & �÷��̾�Ŵ���
//		���� ������ �̺�Ʈ ó��
// ���� �Ŵ���. �÷��̾�Ŵ��� & �����̸Ŵ���
//		�̺�Ʈ�� ���� �� ��ȭ ó��


class cMonsterManager;
class cPlayerManager;
class cMap;

class cGameManager
{
private:
	cMonsterManager*	m_pMonsterManager;
	cPlayerManager*		m_pPlayerManager;

public:
	cGameManager();
	~cGameManager();

	void SetAddressLink(cMonsterManager* cm) { m_pMonsterManager = cm; }
	void SetAddressLink(cPlayerManager* pm) { m_pPlayerManager = pm; }

};

