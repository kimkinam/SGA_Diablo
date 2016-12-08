#pragma once

enum MESSAGE_TYPE
{
	MSG_RUN,

	MSG_NONE,	//1
};

struct Telegram
{
	int				nSender;		//보내는 식별자
	int				nReceiver;		//받는 식별자
	MESSAGE_TYPE	Msg;			//메시지 타입
	float			fDelayTime;		//딜레이시간

	//void* ExtraInfo;

	Telegram() : nSender(0), nReceiver(0), Msg(MSG_NONE), fDelayTime(0) {}
	Telegram(int _sender, int _receiver, MESSAGE_TYPE _msg, float _fDelayTime)
	{
		nSender		= _sender;
		nReceiver	= _receiver;
		Msg			= _msg;
		fDelayTime	= _fDelayTime;
	}
};