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
	switch (msg.Msg)
	{
		case MSG_IDLE:
			pReceiver->HandleMessage(msg);
		break;
		case MSG_RUN:
			pReceiver->HandleMessage(msg);
		break;
		case MSG_NONE:
		break;
		default:
		break;
	}
}


void cMessageManager::MessageSend(float delay, int sender, int receiver, MESSAGE_TYPE msg, void* ExtraInfo)
{
	cGameObject* pReceiver = g_pAIManager->GetAIBaseFromID(receiver);
	


	//memccpy(&MSG.nBox, ExtraInfo, 1, sizeof(UINT));
	//memccpy(&MSG.bound, ExtraInfo, 11, sizeof(cOBB*));
	//memccpy(&MSG.vPickPos, ExtraInfo, 1, sizeof(D3DXVECTOR3));
	//msg.bound = ExtraInfo;
	//msg.vPickPos = pickPos;

	Telegram telegram;
	
	switch (msg)
	{
		case MSG_IDLE:
			telegram = Telegram(sender, receiver, msg, delay, NULL);
		break;
		case MSG_RUN:
		{
			
			telegram = Telegram(sender, receiver, msg, delay, ExtraInfo);
		}
		break;
		case MSG_NONE:
		break;
		default:
			telegram = Telegram(sender, receiver, msg, delay, NULL);
		break;
	}
	
	
	if (delay <= 0.0f)
	{
		Discharge(pReceiver, telegram);
	}
	else
	{
		float currentTime = g_pTimeManager->GetDeltaTime();
	
		telegram.fDelayTime = currentTime + delay;
	
		PriorityQ.insert(telegram);
	}
}

void cMessageManager::MessageDelayedSend()
{
	float currentTime = g_pTimeManager->GetDeltaTime();
	
	// ���۵� �ʿ䰡 �ִ� ������ �ִ��� �˾ƺ��� ����, ť�� �鿩�ٺ���.
	// �ð��� ���� �������� ť�� �պκк��� ������ ������.
	while ((PriorityQ.begin()->fDelayTime < currentTime) &&
		(PriorityQ.begin()->fDelayTime > 0))
	{
		// ť�� �պκк��� ������ �д´�.
		Telegram telegram = *PriorityQ.begin();
	
		// �����ڸ� ã�Ƴ���.
		cGameObject* pReceiver = g_pAIManager->GetAIBaseFromID(telegram.nReceiver);
	
		// �����ڿ��� ������ ������.
		Discharge(pReceiver, telegram);
	
		// �׸��� ť�κ��� �����Ѵ�.
		PriorityQ.erase(PriorityQ.begin());
	}
}