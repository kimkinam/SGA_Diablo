#pragma once
#include "cObject.h"

class cFireShaderClass :
	public cObject
{
public:
	cFireShaderClass();
	~cFireShaderClass();

	void Setup(LPDIRECT3DDEVICE9 device, HWND hwnd);
	void Release();
	void Render(LPDIRECT3DDEVICE9 device, int nIndex,
		D3DXMATRIX matWorld, D3DXMATRIX matView, D3DXMATRIX matProj,
		LPDIRECT3DTEXTURE9 fireTexture, LPDIRECT3DTEXTURE9 noiseTexture,
		LPDIRECT3DTEXTURE9 alphaTexture, float frameTime, D3DXVECTOR3 scrollSpeeds,
		D3DXVECTOR3 vScales, D3DXVECTOR2 distortion1, D3DXVECTOR2 distortion2,
		D3DXVECTOR2 distortion3, float distortionScale, float distortionBias);

private:
	bool SetupShader(LPDIRECT3DDEVICE9 device, HWND hwnd, char* fileName);
	void ReleaseShader();
	void SetShaderPatameters(D3DXMATRIX matWorld, D3DXMATRIX matView, D3DXMATRIX matProj,
		LPDIRECT3DTEXTURE9 fireTexture, LPDIRECT3DTEXTURE9 noiseTexture,
		LPDIRECT3DTEXTURE9 alphaTexture, float frameTime, D3DXVECTOR3 scrollSpeeds,
		D3DXVECTOR3 scales, D3DXVECTOR2 distortion1, D3DXVECTOR2 distortion2, D3DXVECTOR2 distortion3,
		float distortionScale, float distortionBias);
	void RenderShader(LPDIRECT3DDEVICE9* device, int nIndex);

private:
	LPD3DXEFFECT	m_pEffect;
	LPHANDLE		m_pTechnique;
	D3DXMATRIX*		m_matWorld;
	D3DXMATRIX*		m_matView;
	D3DXMATRIX*		m_matProj;

	LPDIRECT3DTEXTURE9	m_pFireTexture;
	LPDIRECT3DTEXTURE9	m_pNoiseTexture;
	LPDIRECT3DTEXTURE9	m_pAlphaTexture;

	float*				m_fFrameTime;
	D3DXVECTOR3*		m_vScrollSpeed;
	D3DXVECTOR3*		m_vScale;

	D3DXVECTOR2*		m_vDistortion1;
	D3DXVECTOR2*		m_vDistortion2;
	D3DXVECTOR2*		m_vDistortion3;

	float*				m_fDistortionScale;
	float*				m_fDistortionBias;





};

