#pragma once

#include <string>

#define EPSILON		0.0001f
#define FLOAT_EQUAL	(a,b)	( abs((a) - (b) ) < FEPSILON )

namespace MyUtil
{
	POINT GetMousePos();						//���콺��ġ
	void SetMousePos(int x, int y);			//���콺��ġ ����

	float Clamp(float value, float min, float max);	//value ���� min max ���̷� ������
	float Clamp01(float value);						//value �� 0 ~ 1 ���Ϸ� ����
	float ClampMinusOnePlusOne(float value);


}