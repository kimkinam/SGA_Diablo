#include "StdAfx.h"
#include "cMainGame.h"
#include "cLoadingScene.h"
#include "cGamingScene.h"
#include "cTestScene.h"


cMainGame::cMainGame(void)
	: m_pCurScene(NULL)
	, m_pGamingScene(NULL)
	, m_pLoadingScene(NULL)
{
}

cMainGame::~cMainGame(void)
{
	SAFE_DELETE(m_pGamingScene);
	SAFE_DELETE(m_pLoadingScene);
	SAFE_DELETE(m_pTestScene);
	SAFE_DELETE(m_pCurScene);

	g_pTextureManager->Destroy();
	g_pFontManger->Destroy();
	g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{
	m_pLoadingScene = new cLoadingScene;
	m_pGamingScene = new cGamingScene;
	m_pTestScene = new cTestScene;

	m_pCurScene = m_pGamingScene;
	m_pCurScene->SetUp();

	SetLight();
}

void cMainGame::Update()
{
	g_pTimeManager->Update();

	if (g_pKeyManager->isOnceKeyDown('T'))
	{
		m_pCurScene = m_pTestScene;
		m_pCurScene->SetUp();
	}
	if (g_pKeyManager->isOnceKeyDown('G'))
	{
		m_pCurScene = m_pGamingScene;
		m_pCurScene->SetUp();
	}
		

	if (m_pCurScene)
		m_pCurScene->Update();
	
}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		//D3DCOLOR_XRGB(20, 20, 20),
		D3DCOLOR_XRGB(50, 50, 50),
		1.0f, 0);

	g_pD3DDevice->BeginScene();

	// 그림을 그린다.

	if (m_pCurScene)
		m_pCurScene->Render();

	g_pTimeManager->Render();

	g_pD3DDevice->EndScene();

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void cMainGame::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{

	if(m_pCurScene)
		m_pCurScene->WndProc(hWnd, message, wParam, lParam);

}

void cMainGame::SetLight()
{
	D3DLIGHT9 stLight;
	stLight.Ambient = stLight.Diffuse = stLight.Specular = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	stLight.Type = D3DLIGHT_DIRECTIONAL;
	D3DXVECTOR3 vDir(1, -1, 1);
	D3DXVec3Normalize(&vDir, &vDir);
	stLight.Direction = vDir;
	g_pD3DDevice->SetLight(0, &stLight);
	g_pD3DDevice->LightEnable(0, true);

	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
}

