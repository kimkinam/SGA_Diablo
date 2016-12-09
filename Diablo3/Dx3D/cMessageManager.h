#pragma once
#include "MessageTypes.h"

class cGameObject;

class cMessageManager
{
private:
	std::set<Telegram> PriorityQ;

	void Discharge(cGameObject* pReceiver, const Telegram& msg);

public:
	cMessageManager();
	~cMessageManager();

	static cMessageManager* Instance()
	{
		static cMessageManager instance;
		return &instance;
	}

	void MessageSend(float delay, int sender, int receiver, MESSAGE_TYPE msg, void* ExtraInfo = NULL);
	void MessageDelayedSend();
};

#define g_pMessageManager cMessageManager::Instance()
