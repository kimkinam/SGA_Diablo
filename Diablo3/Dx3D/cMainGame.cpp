#include "StdAfx.h"
#include "cMainGame.h"
#include "cLoadingScene.h"
#include "cGamingScene.h"
#include "cTestScene.h"
#include "cBossScene.h"


cMainGame::cMainGame(void)
{
}

cMainGame::~cMainGame(void)
{

	g_pSceneManager->Destroy();
	g_pTextureManager->Destroy();
	g_pFontManger->Destroy();
	g_pSkinnedMeshManager->Destroy();
	g_pDeviceManager->Destroy();
	
	SOUNDMANAGER->release();

}

void cMainGame::Setup()
{
	SOUNDMANAGER->init();

	g_pSceneManager->addScene("GamingScene", new cGamingScene);
	g_pSceneManager->addScene("TestScene", new cTestScene);
	g_pSceneManager->addScene("LoadingScene", new cLoadingScene);
	g_pSceneManager->addScene("BossScene", new cBossScene);

	SOUNDMANAGER->addSound("GamingSceneBGM", "Sound/GamingScene_BGM.mp3", true, true);

	SOUNDMANAGER->addSound("SwordSwing", "Sound/Sword_Swing.mp3", false, true);
	SOUNDMANAGER->addSound("FootStep", "Sound/Foot_Step.mp3", false, true);
	SOUNDMANAGER->addSound("WarCay", "Sound/War_Cay.mp3", false, true);
	SOUNDMANAGER->addSound("WhirlWind", "Sound/Whirl_Wind.mp3", false, true);
	

	SOUNDMANAGER->addSound("SkeletonAttack", "Sound/Skeleton_Attack.mp3", false, false);
	SOUNDMANAGER->addSound("SkeletonGetHit", "Sound/Skeleton_GetHit.mp3", false, false);
	SOUNDMANAGER->addSound("SkeletonDead", "Sound/Skeleton_Dead.mp3", false, false);

	SOUNDMANAGER->addSound("FetishAttack", "Sound/Fetish_Attack.mp3", false, false);
	SOUNDMANAGER->addSound("FetishGetHit", "Sound/Fetish_GetHit.mp3", false, false);
	SOUNDMANAGER->addSound("FetishDead", "Sound/Fetish_Dead.mp3", false, false);

	SOUNDMANAGER->addSound("GargantuanAttack", "Sound/Gargantuan_Attack.mp3", false, false);
	SOUNDMANAGER->addSound("GargantuanGetHit", "Sound/Gargantuan_GetHit.mp3", false, false);
	SOUNDMANAGER->addSound("GargantuanDead", "Sound/Gargantuan_Dead.mp3", false, false);

	SOUNDMANAGER->addSound("StitchAttack", "Sound/Stitch_Attack.mp3", false, false);
	SOUNDMANAGER->addSound("StitchGetHit", "Sound/Stitch_GetHit.mp3", false, false);
	SOUNDMANAGER->addSound("StitchDead", "Sound/Stitch_Dead.mp3", false, false);

	SOUNDMANAGER->addSound("ZombieDogAttack", "Sound/ZombieDog_Attack.mp3", false, false);
	SOUNDMANAGER->addSound("ZombieDogGetHit", "Sound/ZombieDog_GetHit.mp3", false, false);
	SOUNDMANAGER->addSound("ZombieDogDead", "Sound/ZombieDog_Dead.mp3", false, false);
<<<<<<< HEAD
	g_pSceneManager->changeScene("TestScene");
=======
	
	g_pSceneManager->changeScene("GamingScene");
>>>>>>> 67aa77e3eecb03a61445d9c5915c3740ec5413e2
	
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
	if (g_pKeyManager->isOnceKeyDown(VK_F4))
		g_pSceneManager->changeScene("BossScene");
	
	g_pSceneManager->Update();
	SOUNDMANAGER->update();
	
}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		//D3DCOLOR_XRGB(20, 20, 20),
		D3DCOLOR_XRGB(0, 0, 0),
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
	sprintf_s(temp, "F1 : LoadingScene\nF2 : TestScene\nF3 : GamingScene\nF4 : BossScene",512);
	
	RECT rc;
	SetRect(&rc, DEBUG_STARTX, DEBUG_STARTY + 40, DEBUG_STARTX + 250, DEBUG_STARTY + 130);
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
	switch (message)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostMessage(hWnd, WM_DESTROY, NULL, NULL);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
	}

	if (g_pSceneManager->GetCurScene())
		g_pSceneManager->GetCurScene()->WndProc(hWnd, message, wParam, lParam);
	//if(m_pCurScene)
	//	m_pCurScene->WndProc(hWnd, message, wParam, lParam);
}

void cMainGame::SetLight()
{
	D3DLIGHT9 stLight;
	stLight.Ambient = stLight.Diffuse = stLight.Specular = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	stLight.Type = D3DLIGHT_DIRECTIONAL;
	D3DXVECTOR3 vDir(0, -1, 0);
	D3DXVec3Normalize(&vDir, &vDir);
	stLight.Direction = vDir;
	g_pD3DDevice->SetLight(0, &stLight);
	g_pD3DDevice->LightEnable(0, true);
	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
}

