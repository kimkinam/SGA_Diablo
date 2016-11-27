#include "stdafx.h"
#include "MyUtil.h"

namespace MyUtil
{
	POINT GetMousePos()
	{
		POINT mousePos;
		GetCursorPos(&mousePos);	//���콺 Screen ���� ��ġ�� ����ش�.

									//Screen ���� ��ġ�� Client ������ ������ �ٲ۴�.
		ScreenToClient(g_hWnd, &mousePos);

		return mousePos;
	}

	//���콺��ġ ����
	void SetMousePos(int x, int y)
	{
		POINT setMousePos;
		setMousePos.x = x;
		setMousePos.y = y;

		//Client ���� ��ġ�� Screen ������ ������ �ٲ۴�.
		ClientToScreen(g_hWnd, &setMousePos);

		//SetCursorPos ��   Screen ���� ��ġ�� �����ؾ��Ѵ�.
		SetCursorPos(setMousePos.x, setMousePos.y);
	}

	//value ���� min max ���̷� ������
	float Clamp(float value, float min, float max)
	{
		if (value < min)
			return min;
		else if (value > max)
			return max;

		return value;

	}
	//value �� 0 ~ 1 ���Ϸ� ����
	float Clamp01(float value)
	{
		if (value < 0.0f)
			return 0.0f;
		else if (value > 1.0f)
			return 1.0f;

		return value;
	}


	float ClampMinusOnePlusOne(float value)
	{
		if (value < -1.0f)
			return -1.0f;
		else if (value > 1.0f)
			return 1.0f;

		return value;
	}

	char * StringToChar(string str)
	{
		
		char* c = new char[str.size() + 1];
		std::copy(str.begin(), str.end(), c);
		c[str.size()] = '\0';

		return c;
	}

}