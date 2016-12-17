#include "stdafx.h"
#include "cLighting.h"
#include "cActionMove.h"

cLighting::cLighting()
	: m_pEffect(NULL)
	, m_pTexture(NULL)
	, m_pAction(NULL)
	, m_vPosition(0, 0, 0)
	, m_vDirection(0, 0, 1)
	, m_vUp(0, 1, 0)
	, m_vRight(1, 0, 0)
	, m_vScale(1, 1, 1)
	, m_fSpeed(0.0f)
	, m_nFrameCount(0)
	, m_fCurAmbient(0.0f)
	, m_fCurFrameRate(0.0f)
	, m_ffLifeTimeDecrease(0.0f)
	, m_fTrailInterval(0.5f)
{
	m_stInfo = ST_LIGHT_STAT();

	D3DXMatrixIdentity(&m_matLocal);
	D3DXMatrixIdentity(&m_matWorld);

}


cLighting::~cLighting()
{

}

void cLighting::Setup(char* szTexture, char* szShader, int nStartFrame)
{
	assert(m_stInfo.fLifeTime != 0 && "LightStat을 먼저 설정해 주세요");

	m_vecVertex.push_back(ST_PT_VERTEX(D3DXVECTOR3(-0.5f, 0, 0.5f),  D3DXVECTOR2(0, 0)));
	m_vecVertex.push_back(ST_PT_VERTEX(D3DXVECTOR3(0.5f, 0, 0.5f),  D3DXVECTOR2(0, 1)));
	m_vecVertex.push_back(ST_PT_VERTEX(D3DXVECTOR3(0.5f, 0, -0.5f), D3DXVECTOR2(1, 1)));
	m_vecVertex.push_back(ST_PT_VERTEX(D3DXVECTOR3(0.5f, 0, -0.5f), D3DXVECTOR2(1, 1)));
	m_vecVertex.push_back(ST_PT_VERTEX(D3DXVECTOR3(-0.5f, 0, -0.5f), D3DXVECTOR2(1, 0)));
	m_vecVertex.push_back(ST_PT_VERTEX(D3DXVECTOR3(-0.5f, 0, 0.5f),  D3DXVECTOR2(0, 0)));

	//05-----1
	//|     |
	//4-----23
	for (size_t i = 0; i < m_vecVertex.size(); ++i)
	{
		float u = 1 / (float)m_nFrameCount;
		if (m_vecVertex[i].t.x == 0)
			m_vecVertex[i].t.x += (u * (nStartFrame - 1));
		else
			m_vecVertex[i].t.x *= (u * nStartFrame);
	}

	m_pTexture = g_pTextureManager->GetTexture(szTexture);
	assert(m_pTexture != NULL && "텍스쳐 로드 실패");

	m_pEffect = LoadEffect(szShader);
	assert(m_pEffect != NULL && "쉐이더 로드 실패");

	
	m_pEffect->SetFloat("fAmbient", 1.0f);
	m_pEffect->SetFloat("fLifeTime", m_stInfo.fLifeTime);
	m_pEffect->SetFloat("fAlpha", m_stInfo.fAlpha);
	m_pEffect->SetFloat("fColorChangeTime", 1.5f);
}

void cLighting::SetLightingStat(float fLifeTime, float fLifeTimeDecrease,int nFrameCount, float fFrameRate, float fAmbientMax)
{
	m_stInfo.fAlpha = 1.0f;
	m_stInfo.fLifeTime = fLifeTime;
	m_stInfo.fFrameRate = fFrameRate;
	m_stInfo.fAmbientMax = fAmbientMax;

	m_nFrameCount = nFrameCount;
	m_fCurFrameRate = m_stInfo.fFrameRate;
	m_fCurAmbient = 3.0f;

	m_fSpeed = 0.1f;
	m_ffLifeTimeDecrease = fLifeTimeDecrease;
}

void cLighting::SetLocalMatrix(D3DXVECTOR3 vDirection, D3DXVECTOR3 vSacling, DWORD dwLightNum)
{
	D3DXVECTOR3 vDir = vDirection;
	D3DXVec3Normalize(&vDir, &vDir);
	m_vScale = vSacling;

	D3DXVec3Cross(&m_vRight, &D3DXVECTOR3(0, 1, 0), &vDir);
	D3DXVec3Normalize(&m_vRight, &m_vRight);
	
	D3DXVec3Cross(&m_vUp, &vDir, &m_vRight);
	D3DXVec3Normalize(&m_vUp, &m_vUp);

	D3DXVECTOR3 scaledRight		= m_vScale.x * m_vRight;
	D3DXVECTOR3 scaledUp		= m_vScale.y * m_vUp;
	D3DXVECTOR3 scaledForward	= m_vScale.z * vDir;
	
	m_matLocal._11 = scaledRight.x;		m_matLocal._12 = scaledRight.y;		m_matLocal._13 = scaledRight.z;
	m_matLocal._21 = scaledUp.x;		m_matLocal._22 = scaledUp.y;		m_matLocal._23 = scaledUp.z;
	m_matLocal._31 = scaledForward.x;	m_matLocal._32 = scaledForward.y;	m_matLocal._33 = scaledForward.z;
	m_matLocal._41 = 0;					m_matLocal._42 = 0;					m_matLocal._43 = 0;
	

	if (dwLightNum == 1)
	{
		D3DXMATRIXA16 matR;
		D3DXMatrixRotationY(&matR, D3DXToRadian(-90.0f));

		m_matLocal = matR * m_matLocal;
	}
	
}

void cLighting::Update()
{
	m_fCurFrameRate -= g_pTimeManager->GetDeltaTime();
	m_stInfo.fLifeTime -= g_pTimeManager->GetDeltaTime();

	if (m_fCurFrameRate <= 0)
	{
		for (size_t i = 0; i < m_vecVertex.size(); ++i)
		{
			float u = 1 / (float)m_nFrameCount;
			m_vecVertex[i].t.x += u;
			m_fCurFrameRate = m_stInfo.fFrameRate;
		}
	}

	m_fCurAmbient += 0.1f;

	if (m_fCurAmbient >= m_stInfo.fAmbientMax)
		m_fCurAmbient = 1.0f;

	

	m_vPosition += m_vDirection * m_fSpeed;

	
	D3DXMATRIXA16 matT;

	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	m_matWorld = m_matLocal * matT;

	if (m_stInfo.fLifeTime <= 1.5f)
	{
		m_stInfo.fAlpha -= g_pTimeManager->GetDeltaTime() / 2;
		m_pEffect->SetFloat("fAlpha", m_stInfo.fAlpha);
	}

	m_pEffect->SetFloat("fAmbient", m_fCurAmbient);
	m_pEffect->SetFloat("fLifeTime", m_stInfo.fLifeTime);

	for (size_t i = 0; i < m_vecTrail.size(); ++i)
	{
		m_vecTrail[i]->Update();
	}
}

void cLighting::Render()
{

	DWORD dwPreLight = 0;
	g_pD3DDevice->GetRenderState(D3DRS_LIGHTING, &dwPreLight);

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	
	UINT numPasses = 0;
	m_pEffect->Begin(&numPasses, 0);

	for (size_t i = 0; i < numPasses; ++i)
	{
		m_pEffect->BeginPass(i);

		D3DXMATRIXA16 matView, matProj;

		g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
		g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);

		m_pEffect->SetMatrix("matWorld", &m_matWorld);
		m_pEffect->SetMatrix("matViewProjection", &(matView * matProj));
		m_pEffect->SetTexture("baseMap", m_pTexture);

		g_pD3DDevice->SetTexture(0, m_pTexture);
		g_pD3DDevice->SetFVF(ST_PT_VERTEX::FVF);
		g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
			m_vecVertex.size() / 3,
			&m_vecVertex[0],
			sizeof(ST_PT_VERTEX));

		m_pEffect->EndPass();
	}

	m_pEffect->End();


}

void cLighting::Release()
{
	SAFE_RELEASE(m_pEffect);
	SAFE_RELEASE(m_pAction);

	for each(auto c in m_vecTrail)
	{
		SAFE_RELEASE(c);
	}
}
