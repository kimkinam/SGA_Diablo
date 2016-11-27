#include "stdafx.h"
#include "cUIInventory.h"
#include "cUIImage.h"
#include "cUIItem.h"
#include "cUIButton.h"
#include "cUIText.h"

cUIInventory::cUIInventory()
	: m_pInvenItem(NULL)
	, m_pInvenInfo(NULL)
	, m_pInvenStat(NULL)
{
}


cUIInventory::~cUIInventory()
{
	if (m_pInvenItem)
		m_pInvenItem->Destroy();

	if (m_pInvenInfo)
		m_pInvenInfo->Destroy();

	if (m_pInvenStat)
		m_pInvenStat->Destroy();

}

void cUIInventory::Setup()
{

	SetupInven_Item();
	SetupInven_Info();
	SetupInven_Stat();

}
void cUIInventory::Update()
{

}
void cUIInventory::Render()
{

}

//하단 아이템 격자 공간
void cUIInventory::SetupInven_Item()
{

}

//상단 아이템 장착 공간
void cUIInventory::SetupInven_Info()
{

}

//캐릭터 스텟 표시 공간
void cUIInventory::SetupInven_Stat()
{
}
