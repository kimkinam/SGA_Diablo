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
	pReceiver->HandleMessage(msg);
}


void cMessageManager::MessageSend(float delay, int sender, int receiver, MESSAGE_TYPE msg, void* ExtraInfo)
{
	//매세지를 처리해야할 대상을 고유식별 아이디를 통해 찾아온다.
	cGameObject* pReceiver = g_pAIManager->GetAIBaseFromID(receiver);


	if (!pReceiver) return;

	//전달할 메시지를 담을 변수를 만든다.
	Telegram telegram(sender, receiver, msg, delay, ExtraInfo);
	
	//딜레이가 없는 매세지면 바로 실행한다.
	if (delay <= 0.0f)
	{
		Discharge(pReceiver, telegram);
	}
	//딜레이가 있는 매세지이면 현재 시간을 더해 매세지 큐에 넣어준다.
	else
	{
		float currentTime = g_pTimeManager->GetDeltaTime();
	
		telegram.fDelayTime = currentTime + delay;
	
		PriorityQ.insert(telegram);
	}
}

void cMessageManager::MessageDelayedSend()
{
	float currentTime = g_pTimeManager->GetTotalSec();
	
	// 전송될 필요가 있는 정보가 있는지 알아보기 위해, 큐를 들여다본다.
	// 시간이 지난 정보들의 큐를 앞부분부터 제거해 나간다.
	while (!PriorityQ.empty()
		&& (PriorityQ.begin()->fDelayTime < currentTime) 
		&& (PriorityQ.begin()->fDelayTime > 0))
	{
		// 큐의 앞부분부터 정보를 읽는다.
		Telegram telegram = *PriorityQ.begin();
	
		// 수신자를 찾아낸다.
		cGameObject* pReceiver = g_pAIManager->GetAIBaseFromID(telegram.nReceiver);
	
		// 수신자에게 정보를 보낸다.
		Discharge(pReceiver, telegram);
	
		// 그리고 큐로부터 제거한다.
		PriorityQ.erase(PriorityQ.begin());
	}
}