#pragma once

class cCamera
{
private:
	D3DXVECTOR3		m_vEye;
	D3DXVECTOR3		m_vForward;
	D3DXVECTOR3		m_vRight;
	D3DXVECTOR3		m_vLookAt;
	D3DXVECTOR3		m_vUp;
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
	void Render();
	void Update(D3DXVECTOR3* pTarget = NULL);

	void MoveForward(float fSpeed, float fY);
	void MoveSideward(float fSpeed);
	void MouseRotate();
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

