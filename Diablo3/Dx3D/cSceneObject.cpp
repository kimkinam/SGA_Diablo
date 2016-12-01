#include "stdafx.h"
#include "cSceneObject.h"
#include "cCamera.h"


cSceneObject::cSceneObject()
	: m_bIsLoad(false)
	, m_pCamera(NULL)
{
}


cSceneObject::~cSceneObject()
{

}

HRESULT cSceneObject::SetUp()
{
	m_pCamera = new cCamera;
	m_pCamera->Setup();

	return S_OK;
}

HRESULT cSceneObject::Reset()
{
	m_pCamera->SetEye(D3DXVECTOR3(0, 0, -5));
	m_pCamera->SetLookAt(D3DXVECTOR3(0, 0, 0));
	m_pCamera->SetUp(D3DXVECTOR3(0, 1, 0));
	m_pCamera->SetForward(D3DXVECTOR3(0, 0, 1));
	m_pCamera->SetRright(D3DXVECTOR3(1, 0, 0));

	return S_OK;
}

void cSceneObject::Release()
{
}

void cSceneObject::Update()
{
}

void cSceneObject::Render()
{
}

