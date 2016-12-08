#include "stdafx.h"
#include "cMessageManager.h"
#include "cGameObject.h"
cMessageManager::cMessageManager()
{
}


cMessageManager::~cMessageManager()
{
}

void cMessageManager::Discharge(cGameObject* pReceiver, const Telegram& msg)
{
	
}


void cMessageManager::MessageSend(float delay, int sender, int receiver, MESSAGE_TYPE msg)
{
	//cGameObject* pReceiver = g_pAIManager->GetAIBaseFromID(receiver);
	//
	//Telegram telegram(sender, receiver, msg, delay);
	//
	//if (delay <= 0.0f)
	//{
	//	Discharge(pReceiver, telegram);
	//}
	//else
	//{
	//	float currentTime = g_pTimeManager->GetDeltaTime();
	//
	//	telegram.fDelayTime = currentTime + delay;
	//
	//	PriorityQ.insert(telegram);
	//}
}

void cMessageManager::MessageDelayedSend()
{
	//float currentTime = g_pTimeManager->GetDeltaTime();
	//
	//// 전송될 필요가 있는 정보가 있는지 알아보기 위해, 큐를 들여다본다.
	//// 시간이 지난 정보들의 큐를 앞부분부터 제거해 나간다.
	//while ((PriorityQ.begin()->fDelayTime < currentTime) &&
	//	(PriorityQ.begin()->fDelayTime > 0))
	//{
	//	// 큐의 앞부분부터 정보를 읽는다.
	//	Telegram telegram = *PriorityQ.begin();
	//
	//	// 수신자를 찾아낸다.
	//	cGameObject* pReceiver = g_pAIManager->GetAIBaseFromID(telegram.nReceiver);
	//
	//	// 수신자에게 정보를 보낸다.
	//	Discharge(pReceiver, telegram);
	//
	//	// 그리고 큐로부터 제거한다.
	//	PriorityQ.erase(PriorityQ.begin())
	//}
}