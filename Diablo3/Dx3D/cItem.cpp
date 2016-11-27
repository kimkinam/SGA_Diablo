#include "stdafx.h"
#include "cItem.h"
#include "cUIImage.h"

cItem::cItem()
	: m_eItemType(E_ITEM::ITEM_COUNT)
	, m_pSumNail(NULL)
	, m_pImage(NULL)
{
	fPlusHp = fPlusMp = fPlusAtk = fPlusSpeed = 0.0f;
	m_ImageName = m_SumNailName = { '\0' };
	m_ptArea.x = m_ptArea.y = 0;
}


cItem::~cItem()
{
}

void cItem::Setup(char * szFolder, char * szImageName, char * sumNailName)
{
	m_pImage = new cUIImage;
	string path = string(szFolder);
	string name = string(szImageName);
	string fullPath = path + name;
	m_pImage->SetTexture(StringToChar(fullPath));

}
