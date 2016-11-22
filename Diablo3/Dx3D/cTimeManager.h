#pragma once

#define g_pTimeManager cTimeManager::GetInstance()

class cTimeManager
{
private:
	SINGLETONE(cTimeManager);

private:
	DWORD	m_dwLastTimeStamp;
	DWORD	m_dwDeltaTime;
	DWORD	m_dwFPS;
	DWORD	m_dwFrameCount;
	DWORD	m_dwPassedFPSTime;

	double	m_TotalSec;

public:
	void Update();
	void Render();
	float GetDeltaTime();
};

