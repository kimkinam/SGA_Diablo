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
	//// ���۵� �ʿ䰡 �ִ� ������ �ִ��� �˾ƺ��� ����, ť�� �鿩�ٺ���.
	//// �ð��� ���� �������� ť�� �պκк��� ������ ������.
	//while ((PriorityQ.begin()->fDelayTime < currentTime) &&
	//	(PriorityQ.begin()->fDelayTime > 0))
	//{
	//	// ť�� �պκк��� ������ �д´�.
	//	Telegram telegram = *PriorityQ.begin();
	//
	//	// �����ڸ� ã�Ƴ���.
	//	cGameObject* pReceiver = g_pAIManager->GetAIBaseFromID(telegram.nReceiver);
	//
	//	// �����ڿ��� ������ ������.
	//	Discharge(pReceiver, telegram);
	//
	//	// �׸��� ť�κ��� �����Ѵ�.
	//	PriorityQ.erase(PriorityQ.begin())
	//}
}