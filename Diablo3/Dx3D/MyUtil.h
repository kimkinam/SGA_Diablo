#pragma once

#include <string>

#define EPSILON		0.0001f
#define FLOAT_EQUAL	(a,b)	( abs((a) - (b) ) < FEPSILON )

namespace MyUtil
{
	POINT GetMousePos();						//마우스위치
	void SetMousePos(int x, int y);			//마우스위치 셋팅

	float Clamp(float value, float min, float max);	//value 값을 min max 사이로 맞추줌
	float Clamp01(float value);						//value 값 0 ~ 1 사일로 맞춤
	float ClampMinusOnePlusOne(float value);


}