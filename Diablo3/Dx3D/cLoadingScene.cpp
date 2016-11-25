#include "stdafx.h"
#include "cLoadingScene.h"


cLoadingScene::cLoadingScene()
{
}


cLoadingScene::~cLoadingScene()
{
	m_nRefCount--;
}

HRESULT cLoadingScene::SetUp()
{
	this->AddRef();

	return S_OK;
}

void cLoadingScene::Release()
{
	m_nRefCount--;
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
