#pragma once

#define g_pFontManger cFontManager::GetInstance()

class cFontManager
{
private:
	SINGLETONE(cFontManager);
public:
	enum eFontType
	{
		E_NORMAL,
		E_CHAT,
		E_DIABLO,
	};
private:
	std::map<eFontType, LPD3DXFONT> m_mapFont;


public:

	LPD3DXFONT GetFont(eFontType e);


	void Destroy();
};
