#pragma once

class cCamera
{
private:
	SYNTHESIZE(D3DXVECTOR3, m_vEye,		Eye);
	SYNTHESIZE(D3DXVECTOR3, m_vForward, Forward);
	SYNTHESIZE(D3DXVECTOR3, m_vRight,	Rright);
	SYNTHESIZE(D3DXVECTOR3, m_vLookAt,	LookAt);
	SYNTHESIZE(D3DXVECTOR3, m_vUp,		Up);
private:

	D3DXMATRIX		m_matView;
	D3DXMATRIX		m_matProj;


	bool			m_isLButtonDown;
	POINT			m_ptPrevMouse;
	float			m_fAngleX;
	float			m_fAngleY;
	float			m_fDistance;


public:
	cCamera(void);
	~cCamera(void);

	void Setup();
	void Reset();
	void Render();
	void Update(D3DXVECTOR3* pTarget = NULL);

	void MoveForward(float fSpeed, float fY);
	void MoveSideward(float fSpeed);
	void MouseRotate();
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

