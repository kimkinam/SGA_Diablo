#pragma once

//�α� ���Ÿ��
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
	//�α� ���
	VOID			AddLog(char* strmsg, ...);
	//���Ϸ� �α� ����
	VOID			LogFile(char* strmsg, ...);

	//����
	VOID			Release(VOID);

private:
	//�α� ���������
	VOID			CreateLogWindow(HWND hWnd);
	//�α� ������ �޼��� ���ν���
	static LRESULT CALLBACK WndLogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

