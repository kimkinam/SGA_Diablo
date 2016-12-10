#pragma once

class cOBB;

enum MESSAGE_TYPE
{
	MSG_RUN,
	MSG_IDLE,
	MSG_ATTACK,
	MSG_HITTED,
	MSG_NONE,	//1
};

struct ST_ATK_EXTRAINFO
{
	UINT	nTarget;
};

struct ST_RUN_EXTRAINFO
{
	UINT					nTarget;
	UINT					nBoxCount;
	std::vector<cOBB*>		vecBox;
	//std::vector<ST_SPHERE*> vecSphere;
	D3DXVECTOR3				vDest;
	float					fSpeed;

	ST_RUN_EXTRAINFO() : nTarget(0), nBoxCount(0), vDest(0, 0, 0), fSpeed(0.0f) {}
	ST_RUN_EXTRAINFO(UINT _nTarget, UINT _nBoxCount, D3DXVECTOR3 _vDest, float _fSpeed)
	{
		nTarget			= _nTarget;
		nBoxCount		= _nBoxCount;
		vDest			= _vDest;
		fSpeed			= _fSpeed;
	}
};

struct Telegram
{
	int				nSender;		//보내는 식별자
	int				nReceiver;		//받는 식별자
	MESSAGE_TYPE	emMessageType;	//메시지 타입
	float			fDelayTime;		//딜레이시간

	void*			ExtraInfo;

	Telegram() : nSender(0), nReceiver(0), emMessageType(MSG_NONE), fDelayTime(0), ExtraInfo(NULL){}
	Telegram(int _sender, int _receiver, MESSAGE_TYPE _msg, float _fDelayTime, void* _ExtraInfo)
	{
		nSender			= _sender;
		nReceiver		= _receiver;
		emMessageType	= _msg;
		fDelayTime		= _fDelayTime;
		ExtraInfo		= _ExtraInfo;
	}

	bool operator<(const Telegram& ins) const
	{
		return fDelayTime < ins.fDelayTime;
	}
};