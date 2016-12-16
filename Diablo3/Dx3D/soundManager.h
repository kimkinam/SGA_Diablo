#pragma once

#include "singletonBase.h"

#include <map>
#include <string>

//채널 갯수 설정 (버퍼)

#define TOTAL_SOUND_CHANNEL 100

using namespace FMOD;

class soundManager : public singletonBase<soundManager>
{
private:
	typedef map<string, Sound**> arrSounds;
	typedef map<string, Sound**>::iterator arrSoundsIter;
	typedef map<string, Channel**> arrChannels;
	typedef map<string, Channel**>::iterator arrChannelIter;

private:
	System* _system;
	Sound** _sound;

	Channel** _channel;

	arrSounds _mTotalSounds;

public:
	soundManager();
	~soundManager();

	HRESULT init();
	void release();

	//사운드 추가
	void addSound(string keyName, string soundName, bool background = false, bool loop = false);

	//재생
	void play(string keyName, float volume);
	void playOnce(string keyName, float vloume);

	//종료
	void stop(string keyName);

	//정지
	void pause(string keyName);

	//다시 재생
	void resume(string keyName);

	//멈춰있니?
	bool isPauseSound(string keyName);

	//재생중이니?
	bool isPlaySound(string keyName);

	//fmod 시스템을 갱신
	void update();
};

