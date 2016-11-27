#include "StdAfx.h"
#include "cMainGame.h"
#include "cLoadingScene.h"
#include "cGamingScene.h"
#include "cTestScene.h"


cMainGame::cMainGame(void)
{
}

cMainGame::~cMainGame(void)
{

	g_pTextureManager->Destroy();
	g_pFontManger->Destroy();
	g_pSceneManager->Destroy();
	g_pSkinnedMeshManager->Destroy();
	g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{
	g_pSceneManager->addScene("GamingScene", new cGamingScene);
	g_pSceneManager->addScene("TestScene", new cTestScene);
	g_pSceneManager->addScene("LoadingScene", new cLoadingScene);

	g_pSceneManager->changeScene("TestScene");
	
	SetLight();
}

void cMainGame::Update()
{
	g_pTimeManager->Update();

	if (g_pKeyManager->isOnceKeyDown(VK_F1))
		g_pSceneManager->changeScene("LoadingScene");
	if (g_pKeyManager->isOnceKeyDown(VK_F2))
		g_pSceneManager->changeScene("TestScene");
	if (g_pKeyManager->isOnceKeyDown(VK_F3))
		g_pSceneManager->changeScene("GamingScene");
	
	g_pSceneManager->Update();

	
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
	g_pSceneManager->Render();
	//if (m_pCurScene)
	//	m_pCurScene->Render();

	g_pTimeManager->Render();

	LPD3DXFONT font;
	font = g_pFontManger->GetFont(cFontManager::E_NORMAL);

	char temp[512];
	sprintf_s(temp, "F1 : LoadingScene\nF2 : TestScene\nF3 : GamingScene",512);
	
	RECT rc = { DEBUG_STARTX, DEBUG_STARTY + 40, DEBUG_STARTX + 250, DEBUG_STARTY + 130 };
	font->DrawText(NULL,
		temp,
		128,
		&rc,
		DT_LEFT,
		D3DCOLOR_XRGB(255, 255, 255));


	g_pD3DDevice->EndScene();

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void cMainGame::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	g_pSceneManager->GetCurScene()->WndProc(hWnd, message, wParam, lParam);
	//if(m_pCurScene)
	//	m_pCurScene->WndProc(hWnd, message, wParam, lParam);
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

