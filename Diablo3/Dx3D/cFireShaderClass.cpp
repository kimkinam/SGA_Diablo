#include "stdafx.h"
#include "cFireShaderClass.h"


cFireShaderClass::cFireShaderClass()
	: m_pEffect(NULL)
	, m_pTechnique(NULL)
	, m_matWorld(NULL)
	, m_matView(NULL)
	, m_matProj(NULL)
	, m_pFireTexture(NULL)
	, m_pNoiseTexture(NULL)
	, m_pAlphaTexture(NULL)
	, m_fFrameTime(NULL)
	, m_vScrollSpeed(NULL)
	, m_vScale(NULL)
	, m_vDistortion1(NULL)
	, m_vDistortion2(NULL)
	, m_vDistortion3(NULL)
	, m_fDistortionScale(NULL)
	, m_fDistortionBias(NULL)

{
}


cFireShaderClass::~cFireShaderClass()
{
}

void cFireShaderClass::Setup(LPDIRECT3DDEVICE9 device, HWND hwnd)
{
}

void cFireShaderClass::Release()
{
}

void cFireShaderClass::Render(LPDIRECT3DDEVICE9 device, int nIndex, D3DXMATRIX matWorld, D3DXMATRIX matView, D3DXMATRIX matProj, LPDIRECT3DTEXTURE9 fireTexture, LPDIRECT3DTEXTURE9 noiseTexture, LPDIRECT3DTEXTURE9 alphaTexture, float frameTime, D3DXVECTOR3 scrollSpeeds, D3DXVECTOR3 vScales, D3DXVECTOR2 distortion1, D3DXVECTOR2 distortion2, D3DXVECTOR2 distortion3, float distortionScale, float distortionBias)
{
}

bool cFireShaderClass::SetupShader(LPDIRECT3DDEVICE9 device, HWND hwnd, char * fileName)
{
	return false;
}

void cFireShaderClass::ReleaseShader()
{
}

void cFireShaderClass::SetShaderPatameters(D3DXMATRIX matWorld, D3DXMATRIX matView, D3DXMATRIX matProj, LPDIRECT3DTEXTURE9 fireTexture, LPDIRECT3DTEXTURE9 noiseTexture, LPDIRECT3DTEXTURE9 alphaTexture, float frameTime, D3DXVECTOR3 scrollSpeeds, D3DXVECTOR3 scales, D3DXVECTOR2 distortion1, D3DXVECTOR2 distortion2, D3DXVECTOR2 distortion3, float distortionScale, float distortionBias)
{
}

void cFireShaderClass::RenderShader(LPDIRECT3DDEVICE9 * device, int nIndex)
{
}
