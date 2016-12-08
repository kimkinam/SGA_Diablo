#pragma once

enum MESSAGE_TYPE
{
	MSG_RUN,

	MSG_NONE,	//1
};

struct Telegram
{
	int				nSender;		//������ �ĺ���
	int				nReceiver;		//�޴� �ĺ���
	MESSAGE_TYPE	Msg;			//�޽��� Ÿ��
	float			fDelayTime;		//�����̽ð�

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