#include "stdafx.h"
#include "iScene.h"
#include "cCamera.h"


iScene::iScene()
{
	this->m_pCamera = new cCamera;
	m_pCamera->Setup();
}


iScene::~iScene()
{
	SAFE_DELETE(this->m_pCamera);
}
