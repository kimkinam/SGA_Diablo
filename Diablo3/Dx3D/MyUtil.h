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

	char* StringToChar(string str);

	void GetBoundMinMax(LPD3DXMESH pMesh, D3DXVECTOR3& vMin, D3DXVECTOR3& vMax);

	void GetBoundSphere(LPD3DXMESH pMesh, ST_SPHERE& pSphere);

	void MakeBoundBox(D3DXVECTOR3 vMin, D3DXVECTOR3 vMax, std::vector<ST_PC_VERTEX>& vecVertex);

	bool CollisionRayToOBB(D3DXVECTOR3 vSrc, D3DXVECTOR3 vDest, 
		D3DXVECTOR3 vObbPos, D3DXVECTOR3 vMin, D3DXVECTOR3 vMax
		, D3DXMATRIXA16* pMatWorld = NULL);
	
	bool CollisionRayToABB(D3DXVECTOR3 vSrc, D3DXVECTOR3 vDest, D3DXVECTOR3 vAbbPos, 
		D3DXVECTOR3 vMin, D3DXVECTOR3 vMax);
	
	LPD3DXEFFECT LoadEffect(const char* szFileName);

	void SetupVertexFog(DWORD dwColor, DWORD dwMode, BOOL bUseRange, FLOAT fDensity);

	int RandomIntRange(int min, int max);

	float RandomFloatRange(float min, float max);
	DWORD FloatToDWORD(float f);
}