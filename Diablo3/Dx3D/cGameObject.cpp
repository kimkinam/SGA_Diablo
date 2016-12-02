#include "stdafx.h"
#include "cGameObject.h"


cGameObject::cGameObject()
	: m_vPosition(0, 0, 0)
	, m_vDirection(0, 0, 0)
	, m_pAction(NULL)
	, m_fAngle(0.0f)
	, m_bIsMove(true)
{

}


cGameObject::~cGameObject()
{
	SAFE_RELEASE(m_pAction);

}

void cGameObject::OnActionFinish(cAction * pSender)
{
}
