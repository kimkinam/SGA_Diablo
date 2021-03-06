#include "StdAfx.h"
#include "cObject.h"

cObject::cObject(void)
	: m_nRefCount(1)
{
}

cObject::~cObject(void)
{
	assert(m_nRefCount == 0 && "우리 이러지 말자..");
}

void cObject::AddRef()
{
	++m_nRefCount;
}

void cObject::Release()
{
	--m_nRefCount;
	if(m_nRefCount == 0)
	{
		delete this;
	}
}

