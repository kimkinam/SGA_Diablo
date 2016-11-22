#include "stdafx.h"
#include "cPlayerManager.h"
#include "cMonsterManager.h"


cPlayerManager::cPlayerManager()
	: m_pMonsterManager(NULL)
{
}


cPlayerManager::~cPlayerManager()
{
	SAFE_DELETE(m_pMonsterManager);
}

void cPlayerManager::Setup()
{
}

void cPlayerManager::Update()
{
}

void cPlayerManager::Render()
{
}
