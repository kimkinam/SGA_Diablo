#pragma once

// 몬스터 매니저 & 플레이어매니저
//		서로 때리는 이벤트 처리
// 몬스터 매니저. 플레이어매니저 & 유아이매니저
//		이벤트에 따른 값 변화 처리


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

