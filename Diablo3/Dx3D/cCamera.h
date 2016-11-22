#pragma once

class cRay;

class cCamera
{
private:
	
	D3DXVECTOR3		m_vLookAt;
	D3DXVECTOR3		m_vUp;
	D3DXVECTOR3		m_vDir;
	POINT			m_ptPrevMouse;
	float			m_fAngleX;
	float			m_fAngleY;
	float			m_fDistance;

	SYNTHESIZE(bool, m_isLButtonDown, IsLButtonDown);
	SYNTHESIZE(D3DXVECTOR3, m_vEye, Eye);

public:
	cCamera(void);
	~cCamera(void);

	void SetView(D3DXVECTOR3* pEye, D3DXVECTOR3* pLookAt, D3DXVECTOR3* pUp);

	void Setup();
	void Render();
	void Update(D3DXVECTOR3* pTarget = NULL);
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


};

