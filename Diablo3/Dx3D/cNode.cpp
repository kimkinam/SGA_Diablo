#include "stdafx.h"
#include "cNode.h"


cNode::cNode()
	: m_pMtlTex(NULL)
	, nKeyFrame(0.0f)
	, m_vecRaycastPos(0, 0, 0)
	, m_vecDirection(0, -1, 0)
	, m_fHeight(1000.0f)
{

	D3DXMatrixIdentity(&m_matLocalTM);
	D3DXMatrixIdentity(&m_matWorldTM);
	m_stFrameInfo.nFistFrame = m_stFrameInfo.nFrameSpeed = m_stFrameInfo.nLastFrame = m_stFrameInfo.nTickPerFrame = 0;
	
	memset(&m_stBounding, 0, sizeof(ST_BOUNDING_SPHERE));
}


cNode::~cNode()
{
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pMtlTex);
}

void cNode::Update(D3DXMATRIXA16 * pmatParent)
{
	nKeyFrame += g_pTimeManager->GetDeltaTime() * m_stFrameInfo.nFrameSpeed * m_stFrameInfo.nTickPerFrame;

	if (nKeyFrame > m_stFrameInfo.nLastFrame * m_stFrameInfo.nTickPerFrame)
		nKeyFrame = m_stFrameInfo.nFistFrame * m_stFrameInfo.nTickPerFrame;

	D3DXMATRIXA16 matPosTM, matRotTM;

	CalcAniPosTM(nKeyFrame, &matPosTM);
	CalcAniRotTM(nKeyFrame, &matRotTM);

	m_matLocalTM = matRotTM * matPosTM;

	m_matWorldTM = m_matLocalTM;

	if (pmatParent)
		m_matWorldTM = m_matLocalTM * (*pmatParent);

	for each(auto pChild in m_vecChild)
	{
		pChild->Update(&m_matWorldTM);
	}

}

void cNode::Render(bool isChecked)
{
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorldTM);
	if (m_pMtlTex)
	{
		if(isChecked)
			g_pD3DDevice->SetTexture(0, m_pMtlTex->GetTexture());
		else
			g_pD3DDevice->SetTexture(0, NULL);
		g_pD3DDevice->SetMaterial(&m_pMtlTex->GetMtl());
		g_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(ST_PNT_VERTEX));
		g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
		g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_vecVertex.size() / 3);

		/*g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
			m_vecVertex.size() / 3,
			&m_vecVertex[0],
			sizeof(ST_PNT_VERTEX));*/
	}

	//g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorldTM);
	

	for each(auto p in m_vecChild)
	{
		p->Render(isChecked);
	}

}

void cNode::AddChild(cNode * pChild)
{
	m_vecChild.push_back(pChild);
}

void cNode::Destroy()
{
	for each(auto c in m_vecChild)
	{
		c->Destroy();
	}
	this->Release();
}

void cNode::CalcLocalTM(D3DXMATRIXA16 * pmatParent)
{
	m_matLocalTM = m_matWorldTM;

	if (pmatParent)
	{
		D3DXMATRIXA16 matInvParent;
		D3DXMatrixInverse(&matInvParent, 0, pmatParent);
		m_matLocalTM = m_matWorldTM * matInvParent;
	}

	

	for each(auto c in m_vecChild)
	{
		c->CalcLocalTM(&m_matWorldTM);
	}

}

void cNode::SetFrameInfo(ST_FRAME_INFO frameInfo)
{
	m_stFrameInfo = frameInfo;

	nKeyFrame = frameInfo.nFistFrame * frameInfo.nTickPerFrame;


	g_pD3DDevice->CreateVertexBuffer(m_vecVertex.size() * sizeof(ST_PNT_VERTEX), 0, ST_PNT_VERTEX::FVF, D3DPOOL_MANAGED, &m_pVB, NULL);

	void* pVertices;
	m_pVB->Lock(0, m_vecVertex.size() * sizeof(ST_PNT_VERTEX), (void**)&pVertices, 0);

	memcpy(pVertices, &m_vecVertex[0], m_vecVertex.size() * sizeof(ST_PNT_VERTEX));

	m_pVB->Unlock();

	for each(auto c in m_vecChild)
	{
		c->SetFrameInfo(m_stFrameInfo);
	}

}

void cNode::CalcAniPosTM(IN float nKeyFrame, OUT D3DXMATRIXA16 * posTM)
{
	D3DXVECTOR3 vPos;

	int nStart = 0;
	int nEnd = -1;

	D3DXMatrixIdentity(posTM);

	//비어있다면.
	if (m_vecPos.empty())
	{
		posTM->_41 = m_matLocalTM._41;
		posTM->_42 = m_matLocalTM._42;
		posTM->_43 = m_matLocalTM._43;
		return;
	}
	//제일 앞 프레임보다 작을 경우.
	else if (nKeyFrame <= m_vecPos.front().keyFrame)
	{
		posTM->_41 = m_vecPos.front().v.x;
		posTM->_42 = m_vecPos.front().v.y;
		posTM->_43 = m_vecPos.front().v.z;

		return;
	}
	//제일 뒤 프레임보다 큰 경우.
	else if (nKeyFrame >= m_vecPos.back().keyFrame)
	{
		posTM->_41 = m_vecPos.back().v.x;
		posTM->_42 = m_vecPos.back().v.y;
		posTM->_43 = m_vecPos.back().v.z;

		return;
	}
	for (size_t i = 0; i < m_vecPos.size(); ++i)
	{
		if (nKeyFrame > m_vecPos[i].keyFrame)
		{
			nEnd = i + 1;
		}
	}
	nStart = nEnd - 1;
	

	float t =(nKeyFrame - m_vecPos[nStart].keyFrame) /  (float)(m_vecPos[nEnd].keyFrame - m_vecPos[nStart].keyFrame);

	D3DXVec3Lerp(&vPos, &m_vecPos[nStart].v, &m_vecPos[nEnd].v, t);
	posTM->_41 = vPos.x;
	posTM->_42 = vPos.y;
	posTM->_43 = vPos.z;

}

void cNode::CalcAniRotTM(IN float nKeyFrame, OUT D3DXMATRIXA16 * rotTM)
{
	D3DXQUATERNION q;

	int nStart = 0;
	int nEnd = -1;

	D3DXMatrixIdentity(rotTM);

	//비어있다면.
	if (m_vecRot.empty())
	{
		rotTM->_11 = m_matLocalTM._11;
		rotTM->_12 = m_matLocalTM._12;
		rotTM->_13 = m_matLocalTM._13;
		rotTM->_21 = m_matLocalTM._21;
		rotTM->_22 = m_matLocalTM._22;
		rotTM->_23 = m_matLocalTM._23;
		rotTM->_31 = m_matLocalTM._31;
		rotTM->_32 = m_matLocalTM._32;
		rotTM->_33 = m_matLocalTM._33;

		return;
	}
	//제일 앞 프레임보다 작을 경우.
	else if (nKeyFrame <= m_vecRot.front().keyFrame)
	{
		D3DXMatrixRotationQuaternion(rotTM, &m_vecRot.front().q);
		return;
	}
	//제일 뒤 프레임보다 큰 경우.
	else if (nKeyFrame >= m_vecRot.back().keyFrame)
	{
		D3DXMatrixRotationQuaternion(rotTM, &m_vecRot.back().q);
		return;
	}
	for (size_t i = 0; i < m_vecRot.size(); ++i)
	{
		if (nKeyFrame > m_vecRot[i].keyFrame)
		{
			nEnd = i + 1;
		}
	}
	nStart = nEnd - 1;

	float t = (nKeyFrame - m_vecRot[nStart].keyFrame) / (float)(m_vecRot[nEnd].keyFrame - m_vecRot[nStart].keyFrame);

	D3DXQuaternionSlerp(&q, &m_vecRot[nStart].q, &m_vecRot[nEnd].q, t);
	
	D3DXMatrixRotationQuaternion(rotTM, &q);
}

