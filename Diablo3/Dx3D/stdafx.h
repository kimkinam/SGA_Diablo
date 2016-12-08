// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <vector>
#include <string>
#include <map>
#include <set>
#include <list>
#include <assert.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#define WINSIZE_X	1280//g_recWinsize.right		//초기 윈도우 가로 크기
#define WINSIZE_Y	800//g_recWinsize.bottom		//초기 윈도우 세로 크기
#define WINPOS_X	0			//초기 윈도우 시작 위치 X
#define WINPOS_Y	0			//초기 윈도우 시작 위치 Y
#define WIN_CLASS	"Unknown - BoundTool"		//윈도우 클레스 이름
#define WIN_TITLE	"Unknown - BoundTool"		//윈도우 타이틀 이름
#define WIN_STYLE	WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN	//윈도우 형태
#define WIN_STYLEEX NULL

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


// 1. Position 정점의 좌표 x,y,z(float)			: D3DFVF_XYZ
// 2. RHW (float)                               : D3DFVF_XYZRHW (D3DFVF_XYZ 또는 D3DFVF_NORMAL과 같이 사용불가)
// 3. Blending Weight Data 결합 가중치 (float)	: D3DFVF_XYZB1 ~ D3DFVF_XYZB5
// 4. Vertex Normal 정점의 법선 벡터 x,y,z(float)	: D3DFVF_NORMAL
// 5. Vertex Point Size 정점의 점 크기 (float)	: D3DFVF_PSIZE
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


struct ST_SPHERE
{
	float		fRadius;
	D3DXVECTOR3 vCenter;
	bool		isPicked;

	ST_SPHERE() : fRadius(0.0f), vCenter(0, 0, 0) {}
};


typedef struct ST_SAVEOBJECT
{
	char szfileName[128];
	char szFolderName[128];
	D3DXVECTOR3 vPosition;
	D3DXVECTOR3 vForward;
	D3DXVECTOR3 vUp;
	D3DXVECTOR3	vRight;
	D3DXVECTOR3 vScale;

}ST_SAVEOBJECT, *LPST_SAVEOBJECT;

<<<<<<< HEAD
=======
struct Telegram
{
	int Sender;
	int Receiver;
	int Msg;
	float SendTime;

	//void* ExtraInfo;

	Telegram() : Sender(0), Receiver(0), Msg(0), SendTime(0) {}
};
>>>>>>> e0112a28f4a653c275e510a043686c65ae0a0200

#define DEBUG_INTERVAL 5
#define	DEBUG_STARTX 0
#define DEBUG_STARTY 50


#include "cDeviceManager.h"
#include "cTimeManager.h"
#include "cTextureManager.h"
#include "cFontManager.h"
#include "cKeyManager.h"
#include "cSceneManager.h"
#include "cSkinnedMeshManager.h"

//Interface
#include "cObject.h"
#include "cUIObject.h"
#include "iMap.h"
#include "cGameObject.h"
#include "iButtonDelegate.h"
#include "cSceneObject.h"

//전역 클래스
#include "cRay.h"
#include "cBoundBox.h"
#include "cOBB.h"
#include "cGameObject.h"
#include "iAI_Base.h"
#include "iState.h"
#include "cStateMachine.h"

//전역 해더
#include "Item.h"
#include "MyUtil.h"
#include "player.h"
#include "Monster.h"

using namespace MyUtil;