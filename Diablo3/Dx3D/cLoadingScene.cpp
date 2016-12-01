#include "stdafx.h"
#include "cLoadingScene.h"


cLoadingScene::cLoadingScene()
{
}


cLoadingScene::~cLoadingScene()
{

}

HRESULT cLoadingScene::SetUp()
{
	if (m_bIsLoad)
	{
		Reset();

		return S_OK;
	}
	cSceneObject::SetUp();

	return S_OK;
}


void cLoadingScene::Update()
{
}

void cLoadingScene::Render()
{
}

void cLoadingScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
}
