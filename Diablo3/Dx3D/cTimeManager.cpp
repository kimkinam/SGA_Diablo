#include "StdAfx.h"
#include "cTimeManager.h"


cTimeManager::cTimeManager(void)
	: m_dwLastTimeStamp(0)
	, m_dwDeltaTime(0)
	, m_dwFPS(0)
	, m_dwFrameCount(0)
	, m_dwPassedFPSTime(0)
	, m_TotalSec(0.0f)
{
	m_dwLastTimeStamp = GetTickCount();
}


cTimeManager::~cTimeManager(void)
{
}

void cTimeManager::Update()
{
	DWORD dwCurrTime = GetTickCount();
	m_dwDeltaTime = dwCurrTime - m_dwLastTimeStamp;
	m_dwLastTimeStamp = dwCurrTime;

	m_dwPassedFPSTime += m_dwDeltaTime;
	if (m_dwPassedFPSTime >= 1000)
	{
		m_dwPassedFPSTime -= 1000;
		m_dwFPS = m_dwFrameCount;
		m_dwFrameCount = 0;
	}

	while (m_dwPassedFPSTime >= 1000)
	{
		m_dwPassedFPSTime -= 1000;
	}

	m_TotalSec += m_dwDeltaTime / 1000.f;
	m_dwFrameCount++;
}

void cTimeManager::Render()
{
	LPD3DXFONT font;
	font = g_pFontManger->GetFont(cFontManager::E_NORMAL);

	char temp[128];
	sprintf_s(temp, "FPS : %d", m_dwFPS, 128);
	RECT rc = { 0, 0, 250, 15 };
	font->DrawText(NULL,
		temp,
		128,
		&rc,
		DT_LEFT,
		D3DCOLOR_XRGB(255, 255, 255));

	sprintf_s(temp, "TotalSec : %.2f", m_TotalSec, 128);
	rc = { 0, 20, 250, 35 };
	font->DrawText(NULL,
		temp,
		128,
		&rc,
		DT_LEFT,
		D3DCOLOR_XRGB(255, 255, 255));
}

float cTimeManager::GetDeltaTime()
{
	return m_dwDeltaTime / 1000.f;
}

