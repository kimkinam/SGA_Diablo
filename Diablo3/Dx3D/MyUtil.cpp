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

	bool CollisionRayToOBB(D3DXVECTOR3 vSrc, D3DXVECTOR3 vDest,
		D3DXVECTOR3 vObbPos, D3DXVECTOR3 vMin, D3DXVECTOR3 vMax
		, D3DXMATRIXA16* pMatWorld)
	{
		D3DXVECTOR3 vTemp1, vTemp2, vOrigin;
		D3DXMATRIXA16 matInv;

		D3DXMatrixInverse(&matInv, 0, pMatWorld);

		vTemp1	= vSrc;
		vTemp2	= vDest;
		vOrigin = vObbPos;

		D3DXVec3TransformCoord(&vTemp1, &vTemp1, &matInv);
		D3DXVec3TransformCoord(&vTemp2, &vTemp2, &matInv);
		D3DXVec3TransformCoord(&vOrigin, &vOrigin, &matInv);

		return CollisionRayToABB(vTemp1, vTemp2, vOrigin, vMin, vMax);
	}

	bool CollisionRayToABB(D3DXVECTOR3 vSrc, D3DXVECTOR3 vDest, D3DXVECTOR3 vAbbPos,
		D3DXVECTOR3 vMin, D3DXVECTOR3 vMax)
	{
		D3DXVECTOR3 BoxMin = vAbbPos + vMin;
		D3DXVECTOR3 BoxMax = vAbbPos + vMax;

		D3DXVECTOR3 d = vSrc - vDest;

		float t1, t2, tmin, tmax, temp;

		tmin = INT_MIN;
		tmax = INT_MAX;

		bool bInBox1 = vSrc.x > BoxMin.x && vSrc.x < BoxMax.x &&
			vSrc.y > BoxMin.y && vSrc.y < BoxMax.y &&
			vSrc.z > BoxMin.z && vSrc.z < BoxMax.z;

		bool bInBox2 = vDest.x > BoxMin.x && vDest.x < BoxMax.x &&
			vDest.y > BoxMin.y && vDest.y < BoxMax.y &&
			vDest.z > BoxMin.z && vDest.z < BoxMax.z;

		if (bInBox1 && bInBox2)
			return false;
		else if (bInBox1 != bInBox2)
			return true;

		if (abs(d.x) < EPSILON)
		{
			if (vSrc.x < BoxMin.x || vSrc.x > BoxMax.x)
				return false;
		}
		else
		{
			t1 = (BoxMin.x - vSrc.x) / d.x;
			t2 = (BoxMax.x - vSrc.x) / d.x;

			if (t1 > t2)
			{
				temp = t1;
				t1 = t2;
				t2 = temp;
			}

			if (t1 > tmin)
				tmin = t1;
			if (t2 > tmax)
				tmax = t2;
			if (tmin > tmax)
				return false;

			if (tmax < 0.0f)
				return false;
			if (tmin > 1.0f)
				return false;
		}

		return true;
	}

	LPD3DXEFFECT LoadEffect(const char * szFileName)
	{
		LPD3DXEFFECT pEffect = NULL;

		// ���̴� �ε�
		LPD3DXBUFFER		pError = NULL;			//���� ���� ( ���̴��� �������Ҷ� �߸� �� �����̳� ���������� �������ִ� ���� )
		DWORD				dwShaderFlag = 0;		//���̴� �÷��� 0 

#ifdef _DEBUG
		dwShaderFlag = dwShaderFlag | D3DXSHADER_DEBUG;		//���̴��� ����׸��� �������ϰڴ� ( ����׸��� �ؾ� �߸��� ������ ������ ���� Error ���ۿ� ���������� ���� ) 
#endif

															//fx ���Ϸ� ���� ���̴� ��ü ����
		D3DXCreateEffectFromFile(
			g_pD3DDevice,				// ����̽�
			szFileName,					// �ҷ��� ���̴� �ڵ� �����̸�
			NULL,						// ���̴��� �������Ҷ� �߰��� ����� #define ���� ( �ϴ� NULL )
			NULL,						// ���̴��� �������Ҷ� #include ���ù��� ó���Ҷ� ����� �������̽� �÷��� ( �ϴ� NULL )
			dwShaderFlag,				// ���̴� ������ �÷���
			NULL,						// ���̴� �Ű������� ������ �޸�Ǯ ( �ϴ� NULL )
			&pEffect,					// �ε��� ���̴� Effect ������
			&pError						// ���̴��� �ε��ϰ� �������Ҷ� ������ ����� �ش� ���ۿ� �����޽����� ���� ( ���������� �ε��Ǹ� NULL �� �����ȴ� )
		);

		//���̴� ���Ϸε������簡 �ִٸ�..
		if (pError != NULL || pEffect == NULL) {

			//������ ������ ���� ���ڿ��� Ȯ��
			int size = pError->GetBufferSize();
			char* str = new char[size];

			//str�� ���ۿ��ִ� ������ �����Ѵ�.
			sprintf_s(str, size, (const char*)pError->GetBufferPointer());

			OutputDebugString(str);
			//���������� ��������� �������� ����
			SAFE_RELEASE(pError);
			if (str)
			{
				delete[] str;
			}

			return NULL;
		}

		return pEffect;
	}

}