// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include <vector>
#include <string>
#include <map>
#include <set>
#include <list>
#include <assert.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

extern HWND	g_hWnd;
extern POINT g_ptMouse;

#define SAFE_DELETE_ARRAY(p) {if (p) { delete [] (p); (p) = NULL; }}
#define SAFE_DELETE(p) if (p) { delete (p); } (p) = NULL;
#define SAFE_RELEASE(p) if (p) { (p)->Release(); } (p) = NULL;
#define SAFE_ADDREF(p) if (p) { (p)->AddRef(); }

#define EPSILON 0.0001f

#define SINGLETONE(class_name) private:\
	class_name(void);\
	~class_name(void);\
public:\
	static class_name* GetInstance()\
	{\
		static class_name instance;\
		return &instance;\
	}

#define SYNTHESIZE(varType, varName, funName)\
protected: varType varName;\
public: inline varType Get##funName(void) const { return varName; }\
public: inline void Set##funName(varType var){ varName = var; }

#define SYNTHESIZE_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: inline varType& Get##funName(void) { return varName; }\
public: inline void Set##funName(varType& var){ varName = var; }

#define SYNTHESIZE_ADD_REF(varType, varName, funName)    \
protected: varType varName; \
public: virtual varType Get##funName(void) const { return varName; } \
public: virtual void Set##funName(varType var){\
	if (varName != var) {\
	SAFE_ADDREF(var);\
	SAFE_RELEASE(varName);\
	varName = var;\
	}\
}

// 1. Position ������ ��ǥ x,y,z(float)			: D3DFVF_XYZ
// 2. RHW (float)                               : D3DFVF_XYZRHW (D3DFVF_XYZ �Ǵ� D3DFVF_NORMAL�� ���� ���Ұ�)
// 3. Blending Weight Data ���� ����ġ (float)	: D3DFVF_XYZB1 ~ D3DFVF_XYZB5
// 4. Vertex Normal ������ ���� ���� x,y,z(float)	: D3DFVF_NORMAL
// 5. Vertex Point Size ������ �� ũ�� (float)	: D3DFVF_PSIZE
// 6. Diffuse Color (DWORD)						: D3DFVF_DIFFUSE
// 7. Specular Color (DWORD)                    : D3DFVF_SPECULAR
// 8. Texture Coordinate Set 1 (float)          : D3DFVF_TEX0 - D3DFVF_TEX8

struct ST_PC_VERTEX
{
	D3DXVECTOR3 p;
	D3DCOLOR	c;

	ST_PC_VERTEX() : p(0, 0, 0), c(0) {}
	ST_PC_VERTEX(D3DXVECTOR3 _p, D3DCOLOR _c) : p(_p), c(_c) {}

	enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE, };
};

struct ST_PT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR2 t;

	ST_PT_VERTEX() : p(0, 0, 0), t(0, 0) {}
	ST_PT_VERTEX(D3DXVECTOR3 _p, D3DXVECTOR2 _t) : p(_p), t(_t) {}

	enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1, };
};

struct ST_PN_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;

	ST_PN_VERTEX() : p(0, 0, 0), n(0, 0, 0) {}
	ST_PN_VERTEX(D3DXVECTOR3 _p, D3DXVECTOR3 _n) : p(_p), n(_n) {}

	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL, };
};

struct ST_PNT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	D3DXVECTOR2 t;

	ST_PNT_VERTEX() : p(0, 0, 0), n(0, 0, 0), t(0, 0) {}
	ST_PNT_VERTEX(D3DXVECTOR3 _p, D3DXVECTOR3 _n, D3DXVECTOR2 _t) : p(_p), n(_n), t(_t) {}

	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1, };
};

struct ST_RHWC_VERTEX
{
	D3DXVECTOR4 p;
	D3DCOLOR	c;
	enum { FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE };
};


struct ST_CONTROL_POS
{
	D3DXVECTOR3 v;
	int keyFrame;
};
struct ST_CONTROL_ROT
{
	D3DXQUATERNION q;
	int keyFrame;
};


struct ST_FRAME_INFO
{
	int nFistFrame;
	int nLastFrame;
	int nFrameSpeed;
	int nTickPerFrame;
};

struct ST_BOUNDING_SPHERE
{
	D3DXVECTOR3 center;
	float		radius;
	ST_BOUNDING_SPHERE() {};
	ST_BOUNDING_SPHERE(D3DXVECTOR3 c, float r) { center = c; radius = r; }
};

struct ST_COLLIDER
{
	int nWidth;
	int nHeight;
	int nStartX;
	int nStartY;

	ST_COLLIDER() : nWidth(0), nHeight(0), nStartX(0), nStartY(0) { }

	ST_COLLIDER(int startX, int startY, int width, int height) {
		
		nStartX = startX;
		nStartY = startY;
		
		nWidth = width;
		nHeight = height;

	}
};

struct ST_CLICKED
{
	bool bIsClicked;
	POINT ptClickedPos;

	ST_CLICKED() : bIsClicked(false)
	{
		ptClickedPos.x = 0;
		ptClickedPos.y = 0;
	};
};

struct ST_SPHERE
{
	float		fRadius;
	D3DXVECTOR3 vCenter;
	bool		isPicked;

	ST_SPHERE() : fRadius(0.0f), vCenter(0, 0, 0) {}
};

enum PLAYER_STATE
{
	PLAYER_IDLE,
	PLAYER_MOVE,
	PLAYER_ATTACK1,
	PLAYER_ATTACT2,
	PLAYER_STATE_COUNT
};


#define DEBUG_INTERVAL 5
#define	DEBUG_STARTX 0
#define DEBUG_STARTY 100


#include "cDeviceManager.h"
#include "cTimeManager.h"
#include "cTextureManager.h"
#include "cFontManager.h"
#include "cKeyManager.h"

//Interface
#include "cObject.h"
#include "cUIObject.h"
#include "iMap.h"
#include "cGameObject.h"
#include "iButtonDelegate.h"
#include "iScene.h"

//���� Ŭ����
#include "cRay.h"