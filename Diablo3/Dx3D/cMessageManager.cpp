#include "stdafx.h"
#include "cMessageManager.h"


cMessageManager::cMessageManager()
{
}


cMessageManager::~cMessageManager()
{
}

void cMessageManager::MessageSend(float delay, int sender, int receiver, int msg, void* ExtraInfo)
{
	//Telegram telegram(sender, receiver, msg, delay, ExtraInfo);
}

void cMessageManager::MessageDelayedSend()
{

}