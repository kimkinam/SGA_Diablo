#pragma once

#include "singletonBase.h"

#include <map>
#include <string>

//ä�� ���� ���� (����)

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

	//���� �߰�
	void addSound(string keyName, string soundName, bool background = false, bool loop = false);

	//���
	void play(string keyName, float volume);
	void playOnce(string keyName, float vloume);

	//����
	void stop(string keyName);

	//����
	void pause(string keyName);

	//�ٽ� ���
	void resume(string keyName);

	//�����ִ�?
	bool isPauseSound(string keyName);

	//������̴�?
	bool isPlaySound(string keyName);

	//fmod �ý����� ����
	void update();
};

