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

	void GetBoundMinMax(LPD3DXMESH pMesh, D3DXVECTOR3 & vMin, D3DXVECTOR3 & vMax)
	{
		LPVOID pV = NULL;

		pMesh->LockVertexBuffer(0, &pV);

		D3DXComputeBoundingBox((D3DXVECTOR3*)pV,
			pMesh->GetNumVertices(),
			D3DXGetFVFVertexSize(pMesh->GetFVF()),
			&vMin, &vMax);

		//D3DXVec3Minimize(&m_vMin, &m_vMin, &vMin);
		//D3DXVec3Maximize(&m_vMax, &m_vMax, &vMax);

		pMesh->UnlockVertexBuffer();
	}

	void GetBoundSphere(LPD3DXMESH pMesh, ST_SPHERE & pSphere)
	{

		LPVOID pV = NULL;

		pMesh->LockVertexBuffer(0, &pV);

		D3DXComputeBoundingSphere((D3DXVECTOR3*)pV,
			pMesh->GetNumVertices(),
			D3DXGetFVFVertexSize(pMesh->GetFVF()),
			&(pSphere.vCenter),
			&(pSphere.fRadius));

		pMesh->UnlockVertexBuffer();
	}

	void MakeBoundBox(D3DXVECTOR3 vMin, D3DXVECTOR3 vMax, std::vector<ST_PC_VERTEX>& vecVertex)
	{
		std::vector<ST_PC_VERTEX> vertex;

		vertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(vMin.x, vMin.y, vMin.z), D3DCOLOR_XRGB(255, 255, 255)));
		vertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(vMin.x, vMax.y, vMin.z), D3DCOLOR_XRGB(255, 255, 255)));
		vertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(vMax.x, vMax.y, vMin.z), D3DCOLOR_XRGB(255, 255, 255)));
		vertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(vMax.x, vMin.y, vMin.z), D3DCOLOR_XRGB(255, 255, 255)));
		vertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(vMin.x, vMin.y, vMax.z), D3DCOLOR_XRGB(255, 255, 255)));
		vertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(vMin.x, vMax.y, vMax.z), D3DCOLOR_XRGB(255, 255, 255)));
		vertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(vMax.x, vMax.y, vMax.z), D3DCOLOR_XRGB(255, 255, 255)));
		vertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(vMax.x, vMin.y, vMax.z), D3DCOLOR_XRGB(255, 255, 255)));
		
		
		std::vector<int>		vecIndex;
		vecIndex.reserve(36);
		// ��
		vecIndex.push_back(0); // ���� (0, 1)
		vecIndex.push_back(1); // �»� (0, 0)
		vecIndex.push_back(2); // ��� (1, 0)

		vecIndex.push_back(0); // ���� (0, 1)
		vecIndex.push_back(2); // ��� (1, 0)
		vecIndex.push_back(3); // ���� (1, 1)

							   // ��
		vecIndex.push_back(7); // ����
		vecIndex.push_back(6); // �»�
		vecIndex.push_back(5); // ���

		vecIndex.push_back(7); // ����
		vecIndex.push_back(5); // ���
		vecIndex.push_back(4); // ����

							   // ��
		vecIndex.push_back(4);
		vecIndex.push_back(5);
		vecIndex.push_back(1);

		vecIndex.push_back(4);
		vecIndex.push_back(1);
		vecIndex.push_back(0);

		// ��
		vecIndex.push_back(3);
		vecIndex.push_back(2);
		vecIndex.push_back(6);

		vecIndex.push_back(3);
		vecIndex.push_back(6);
		vecIndex.push_back(7);

		// ��
		vecIndex.push_back(1);
		vecIndex.push_back(5);
		vecIndex.push_back(6);

		vecIndex.push_back(1);
		vecIndex.push_back(6);
		vecIndex.push_back(2);

		// ��
		vecIndex.push_back(4);
		vecIndex.push_back(0);
		vecIndex.push_back(3);

		vecIndex.push_back(4);
		vecIndex.push_back(3);
		vecIndex.push_back(7);

		for (size_t i = 0; i < vecIndex.size(); ++i)
		{
			ST_PC_VERTEX v;
			v.p = vertex[vecIndex[i]].p;
			v.c = vertex[vecIndex[i]].c;
			vecVertex.push_back(v);
		}

		int a = 0;
	}

}