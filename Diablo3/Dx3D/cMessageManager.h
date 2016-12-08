#pragma once
#include "MessageTypes.h"

class cMessageManager
{
private:
	std::set<Telegram> PriorityQ;

	void Discharge();

public:
	cMessageManager();
	~cMessageManager();

	static cMessageManager* Instance()
	{
		static cMessageManager instance;
		return &instance;
	}

	void MessageSend(float delay, int sender, int receiver, int msg, void* ExtraInfo);
	void MessageDelayedSend();
};

#define g_pMessageManager cMessageManager::Instance()
