#pragma once

//로그 출력타입
#define LOG_NONE		0x00000000
#define LOG_CONSOLE		0x00000001
#define LOG_FILE		0x00000002
#define LOG_WINDOW		0x00000004
#define LOG_ALL			(LOG_CONSOLE | LOG_FILE | LOG_WINDOW)

#define LOG_WIN_TITLE		"logWindow"
#define LOG_WINSIZE_X		250				
#define LOG_WINSIZE_Y		800

#define g_pLogManager cLogManager::GetInstance()

class cLogManager
{
private:
	SINGLETONE(cLogManager);

private:
	unsigned int			m_nTarget;
	char					m_szFileName[MAX_PATH];
	HWND					m_hWndLog;
	HWND					m_hWndListBox;

public:
	HRESULT			Init(unsigned int nTarget, HWND hParentWnd, const char* pszFileName = NULL);
	//로그 찍기
	VOID			AddLog(char* strmsg, ...);
	//파일로 로그 저장
	VOID			LogFile(char* strmsg, ...);

	//해재
	VOID			Release(VOID);

private:
	//로그 윈도우생성
	VOID			CreateLogWindow(HWND hWnd);
	//로그 윈도우 메세지 프로시져
	static LRESULT CALLBACK WndLogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

