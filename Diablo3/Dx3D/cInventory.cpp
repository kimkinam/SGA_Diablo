#include "stdafx.h"
#include "cInventory.h"
#include "cUIImage.h"
#include "cUIItem.h"
#include "cUIButton.h"
#include "cUIText.h"


cInventory::cInventory()
	: m_pInvenInfo(NULL)
	, m_pInvenItem(NULL)
	, m_pInvenStat(NULL)
	, m_pSprite(NULL)
{
}


cInventory::~cInventory()
{
	if (m_pInvenItem)
		m_pInvenItem->Destroy();

	if (m_pInvenInfo)
		m_pInvenInfo->Destroy();

	if (m_pInvenStat)
		m_pInvenStat->Destroy();
}

void cInventory::Setup()
{
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

	SetupInven_Item();
	SetupInven_Info();
	SetupInven_Stat();
}

void cInventory::Update()
{
}

void cInventory::Render()
{
	if (m_pInvenInfo)
		m_pInvenInfo->Render(m_pSprite);
}

void cInventory::SetupInven_Item()
{
}

void cInventory::SetupInven_Info()
{
	D3DXMATRIX matS;

	cUIImage* head = new cUIImage;
	head->SetPosition(194, 85, 0);
	D3DXMatrixIdentity(&matS);
	D3DXMatrixScaling(&matS, 0.54f, 0.44f, 1);
	head->SetmatS(matS);
	head->SetTexture("./Resources/UI/인벤머리.png");
	head->SetTag(cUIObject::Inven_head);
	m_pInvenInfo->AddChild(head);
}

void cInventory::SetupInven_Stat()
{
}
