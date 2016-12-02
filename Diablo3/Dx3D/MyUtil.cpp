#include "stdafx.h"
#include "MyUtil.h"

namespace MyUtil
{
	POINT GetMousePos()
	{
		POINT mousePos;
		GetCursorPos(&mousePos);	//마우스 Screen 상의 위치를 얻어준다.

									//Screen 상의 위치를 Client 영영역 범위로 바꾼다.
		ScreenToClient(g_hWnd, &mousePos);

		return mousePos;
	}

	//마우스위치 셋팅
	void SetMousePos(int x, int y)
	{
		POINT setMousePos;
		setMousePos.x = x;
		setMousePos.y = y;

		//Client 상의 위치를 Screen 영영역 범위로 바꾼다.
		ClientToScreen(g_hWnd, &setMousePos);

		//SetCursorPos 는   Screen 상의 위치로 셋팅해야한다.
		SetCursorPos(setMousePos.x, setMousePos.y);
	}

	//value 값을 min max 사이로 맞추줌
	float Clamp(float value, float min, float max)
	{
		if (value < min)
			return min;
		else if (value > max)
			return max;

		return value;

	}
	//value 값 0 ~ 1 사일로 맞춤
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
		// 전
		vecIndex.push_back(0); // 좌하 (0, 1)
		vecIndex.push_back(1); // 좌상 (0, 0)
		vecIndex.push_back(2); // 우상 (1, 0)

		vecIndex.push_back(0); // 좌하 (0, 1)
		vecIndex.push_back(2); // 우상 (1, 0)
		vecIndex.push_back(3); // 우하 (1, 1)

							   // 후
		vecIndex.push_back(7); // 좌하
		vecIndex.push_back(6); // 좌상
		vecIndex.push_back(5); // 우상

		vecIndex.push_back(7); // 좌하
		vecIndex.push_back(5); // 우상
		vecIndex.push_back(4); // 우하

							   // 좌
		vecIndex.push_back(4);
		vecIndex.push_back(5);
		vecIndex.push_back(1);

		vecIndex.push_back(4);
		vecIndex.push_back(1);
		vecIndex.push_back(0);

		// 우
		vecIndex.push_back(3);
		vecIndex.push_back(2);
		vecIndex.push_back(6);

		vecIndex.push_back(3);
		vecIndex.push_back(6);
		vecIndex.push_back(7);

		// 상
		vecIndex.push_back(1);
		vecIndex.push_back(5);
		vecIndex.push_back(6);

		vecIndex.push_back(1);
		vecIndex.push_back(6);
		vecIndex.push_back(2);

		// 하
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

	LPD3DXEFFECT LoadEffect(const char * szFileName)
	{
		LPD3DXEFFECT pEffect = NULL;

		// 셰이더 로딩
		LPD3DXBUFFER		pError = NULL;			//에러 버퍼 ( 셰이더를 컴파일할때 잘못 된 문법이나 오류정보를 리턴해주는 버퍼 )
		DWORD				dwShaderFlag = 0;		//셰이더 플레그 0 

#ifdef _DEBUG
		dwShaderFlag = dwShaderFlag | D3DXSHADER_DEBUG;		//셰이더를 디버그모드로 컴파일하겠다 ( 디버그모드로 해야 잘못된 컴파일 오류가 날때 Error 버퍼에 오류정보가 들어간다 ) 
#endif

															//fx 파일로 부터 셰이더 객체 생성
		D3DXCreateEffectFromFile(
			g_pD3DDevice,				// 디바이스
			szFileName,					// 불러올 셰이더 코드 파일이름
			NULL,						// 셰이더를 컴파일할때 추가로 사용할 #define 정의 ( 일단 NULL )
			NULL,						// 셰이더를 컴파일할때 #include 지시문을 처리할때 사용할 인터페이스 플레그 ( 일단 NULL )
			dwShaderFlag,				// 셰이더 컴파일 플레그
			NULL,						// 셰이더 매개변수를 공유할 메모리풀 ( 일단 NULL )
			&pEffect,					// 로딩될 셰이더 Effect 포인터
			&pError						// 셰이더를 로딩하고 컴파일할때 문제가 생기면 해당 버퍼에 에러메시지가 들어간다 ( 성공적으로 로딩되면 NULL 이 참조된다 )
		);

		//셰이더 파일로딩에문재가 있다면..
		if (pError != NULL || pEffect == NULL) {

			//문제의 내용이 뭔지 문자열로 확인
			int size = pError->GetBufferSize();
			char* str = new char[size];

			//str에 버퍼에있는 내용을 저장한다.
			sprintf_s(str, size, (const char*)pError->GetBufferPointer());

			OutputDebugString(str);
			//오류내용을 출력했으니 오류버퍼 해제
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