#include "stdafx.h"
#include "cMonsterManager.h"
#include "cPlayerManager.h"


cMonsterManager::cMonsterManager()
	: m_pPlayerManager(NULL)
{
}


cMonsterManager::~cMonsterManager()
{
	SAFE_DELETE(m_pPlayerManager);
}

void cMonsterManager::Render()
{
}

void cMonsterManager::Update()
{
}

void cMonsterManager::Setup()
{
}
