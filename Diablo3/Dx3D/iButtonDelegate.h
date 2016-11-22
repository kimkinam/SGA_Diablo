#pragma once

class cUIButton;

class iButtonDelegate
{
public:
	virtual void OnClick(cUIButton* pSender) = 0;
};

