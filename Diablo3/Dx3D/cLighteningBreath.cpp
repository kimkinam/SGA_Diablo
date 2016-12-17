#include "stdafx.h"
#include "cLighteningBreath.h"
#include "cTrailRenderer.h"

cLighteningBreath::cLighteningBreath()
	: m_vScail(1.0f, 1.0f, 1.0f)
	, m_nType(BREATH_YELLOW)
	, m_pTrailRenderer(NULL)
	, m_fFrameRate(0.0f)
	, m_fLifeTime(5.0f)
	, m_vDirection(0, 0, 1)
	, m_vUp(0, 1, 0)
	, m_vRight(1, 0, 0)
	, m_pLighteningTex(NULL)
	, m_bIsDrawLine(false)
	, m_pEffect(NULL)
	, m_fAlphaFactor(1.0f)
{
	//0---1
	//|	  |
	//2---3
	m_stPTV[0].p = D3DXVECTOR3(-0.5f, 0,   0.5f);
	m_stPTV[1].p = D3DXVECTOR3(0.5f,  0,   0.5f);
	m_stPTV[2].p = D3DXVECTOR3(-0.5f, 0,  -0.5f);
	m_stPTV[3].p = D3DXVECTOR3(0.5f,  0,  -0.5f);
	D3DXMatrixIdentity(&m_matLocal);
	D3DXMatrixIdentity(&m_matWorld);
	m_nIb[0] = 0;
	m_nIb[1] = 1;
	m_nIb[2] = 2;

	m_nIb[3] = 2;
	m_nIb[4] = 1;
	m_nIb[5] = 3;

	//m_fTimer = 0.0f;
	
}


cLighteningBreath::~cLighteningBreath()
{
	//SAFE_RELEASE(m_pLighteningTex);

	SAFE_RELEASE(m_pEffect);
}

void cLighteningBreath::SetUp(int type,D3DXVECTOR3 vPos, D3DXVECTOR3 vMoveDir, int animIndx)
{
	int a = 0;
	switch (type)
	{
	case BREATH_YELLOW:
	{
		m_pEffect = LoadEffect("./Resources/Shaders/Lighting.fx");
		//m_pEffect->SetFloat("fAmbientMax", 2.0f);
		m_vScail.z *= 1.0f;
		m_vScail.x *= 1.0f;
		m_vScail.y = 1.0f;
		D3DXMATRIXA16 matR_Y,matR_X;
		D3DXMatrixRotationY(&matR_Y, D3DXToRadian(-90.0f));
		//D3DXMatrixRotationX(&matR_X, D3DXToRadian(90.0f));
		m_matLocal = matR_Y;
		
		D3DXVECTOR3 vDirection = D3DXVECTOR3(0, 0, 1); //vMoveDir;// D3DXVECTOR3(0, -1, 0);
		D3DXVECTOR3 vUp(0,1,0);
		D3DXVECTOR3 vRight;
		D3DXVec3Cross(&vRight, &vUp, &vDirection);
		D3DXVec3Normalize(&vRight, &vRight);
		memcpy(&m_matLocal._11, &(vRight*m_vScail.x), sizeof(D3DXVECTOR3));
		memcpy(&m_matLocal._21, &vUp, sizeof(D3DXVECTOR3));
		memcpy(&m_matLocal._31, &(vDirection*m_vScail.z), sizeof(D3DXVECTOR3));
		m_pLighteningTex = g_pTextureManager->GetTexture("./Resources/Images/Particles/diablo_lightningForks.png");
		
		m_nAnimationIndex = animIndx;
		
		float fAnimationWidthFactor = 1.0f / 4.0f;
		m_stPTV[0].t = D3DXVECTOR2(m_nAnimationIndex*fAnimationWidthFactor, 1.0f);
		m_stPTV[1].t = D3DXVECTOR2((m_nAnimationIndex + 1)*fAnimationWidthFactor, 1.0f);
		m_stPTV[2].t = D3DXVECTOR2(m_nAnimationIndex*fAnimationWidthFactor, 0.0f);
		m_stPTV[3].t = D3DXVECTOR2((m_nAnimationIndex + 1)*fAnimationWidthFactor, 0.0f);

		//D3DXLoadMeshFromX("./Resources/LightingBreath.x",);

	}
		break;
	case BREATH_BLUE:
		D3DXMatrixRotationZ(&m_matLocal, D3DXToRadian(90.0f));
		m_pLighteningTex = g_pTextureManager->GetTexture("./Resources/Images/Particles/diablo_singleStream_00.png");
		break;
	case BREATH_TRAIL:
		m_pTrailRenderer = new cTrailRenderer;
		m_pTrailRenderer->SetParent(&m_matWorld);
		m_pTrailRenderer->SetColor(0xffffffff);
		m_pTrailRenderer->SetTrailTexture(g_pTextureManager->GetTexture("./Resources/Images/Particles/Trail_fire.png"));

		break;
	}
	m_vMoveDir = vMoveDir;
	m_vPos = vPos;
	
	//D3DXMATRIXA16 viewMat;
	//g_pD3DDevice->GetTransform(D3DTS_VIEW, &viewMat);
	//D3DXVECTOR3 forward =D3DXVECTOR3(-viewMat._31, viewMat._32, viewMat._33);
	////D3DXVec3Normalize(&m_vDirection, &forward);
	//D3DXVec3Normalize(&m_vDirection, &vDirection);
	//
	//D3DXVec3Cross(&m_vRight, &D3DXVECTOR3(0, 1, 0), &m_vDirection);
	//D3DXVec3Normalize(&m_vRight, &m_vRight);
	//
	//D3DXVec3Cross(&m_vUp, &m_vDirection, &m_vRight);
	//D3DXVec3Normalize(&m_vUp, &m_vUp);

	WorldUpdate();

}

void cLighteningBreath::TrailSetUp()
{
	
}

void cLighteningBreath::Release()
{
	//SAFE_RELEASE(m_pLighteningTex);

	if (m_pTrailRenderer)
		m_pTrailRenderer->Release();

	SAFE_DELETE(m_pTrailRenderer);
}

void cLighteningBreath::Update()
{
	
	switch (m_nType)
	{
	case BREATH_YELLOW:
		BreathUpdate();
		
		break;
	case BREATH_BLUE:
		BreathUpdate();
		break;
	case BREATH_FORK:
		ForkUpdate();
		break;
	case BREATH_TRAIL:
		m_pTrailRenderer->Update(g_pTimeManager->GetDeltaTime());
		break;
	}
	//LocalUpdate();
	WorldUpdate();
}

void cLighteningBreath::LocalUpdate()
{
}


void cLighteningBreath::WorldUpdate()
{

	D3DXMatrixIdentity(&m_matWorld);
	D3DXVECTOR3 vScailedRight = m_vRight;
	D3DXVECTOR3 vScailedUp = m_vUp;
	D3DXVECTOR3 vScailedDirection = m_vDirection;

	memcpy(&m_matWorld._11,&vScailedRight , sizeof(D3DXVECTOR3));
	memcpy(&m_matWorld._21, &vScailedUp, sizeof(D3DXVECTOR3));
	memcpy(&m_matWorld._31, &vScailedDirection, sizeof(D3DXVECTOR3));
	memcpy(&m_matWorld._41, &m_vPos, sizeof(D3DXVECTOR3));

}

void cLighteningBreath::BreathUpdate()
{
	m_fLifeTime -= g_pTimeManager->GetDeltaTime();

	m_fFrameRate += g_pTimeManager->GetDeltaTime();

	if (m_fFrameRate >= 0.2f)
	{
		m_nAnimationIndex++;
		if (m_nAnimationIndex > 4)
		{
			m_nAnimationIndex = 0;
		}

		float fAnimationWidthFactor = 1.0f / 4.0f;
		m_stPTV[0].t = D3DXVECTOR2(m_nAnimationIndex*fAnimationWidthFactor, 1.0f);
		m_stPTV[1].t = D3DXVECTOR2((m_nAnimationIndex + 1)*fAnimationWidthFactor, 1.0f);
		m_stPTV[2].t = D3DXVECTOR2(m_nAnimationIndex*fAnimationWidthFactor, 0.0f);
		m_stPTV[3].t = D3DXVECTOR2((m_nAnimationIndex + 1)*fAnimationWidthFactor, 0.0f);

		m_fFrameRate = 0.0f;
	}

	float fSpeed = 0.5f;
	//m_vMoveDir = D3DXVECTOR3(m_matLocal._21, m_matLocal._22, m_matLocal._23);
	m_vPos += m_vMoveDir * fSpeed * g_pTimeManager->GetDeltaTime();

	if (m_fLifeTime < 2.0f)
	{
		//m_fAlphaFactor -= g_pTimeManager->GetDeltaTime();
		//m_pEffect->SetFloat("fAlpha", m_fAlphaFactor);
	}	

	m_pEffect->SetFloat("fAmbient", 1.0f);
	m_pEffect->SetFloat("fLifeTime", m_fLifeTime);
}

void cLighteningBreath::ForkUpdate()
{
}

void cLighteningBreath::TrailUpdate()
{
}

void cLighteningBreath::Render()
{
	switch (m_nType)
	{
	case BREATH_YELLOW:
		BreathRender();
		break;
	case BREATH_BLUE:
		BreathRender();
		break;
	case BREATH_FORK:
		ForkRender();
		break;
	case BREATH_TRAIL:
		m_pTrailRenderer->Render();
		break;
	}
	
}

void cLighteningBreath::BreathRender()
{
	if (!m_pEffect) return;
	DWORD prevFVF, prevLighting,prevCullMode;

	D3DXMATRIXA16 prevWorld, matLightingWorld;
	D3DXMATRIXA16 matFinal;

	g_pD3DDevice->GetFVF(&prevFVF);
	g_pD3DDevice->GetTransform(D3DTS_WORLD, &prevWorld);
	g_pD3DDevice->GetRenderState(D3DRS_LIGHTING, &prevLighting);
	g_pD3DDevice->GetRenderState(D3DRS_CULLMODE, &prevCullMode);

	UINT numPasses = 0;
	m_pEffect->Begin(&numPasses, 0);

	for (size_t i = 0; i < numPasses; ++i)
	{
		m_pEffect->BeginPass(i);
		g_pD3DDevice->SetFVF(ST_PT_VERTEX::FVF);
		g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

		D3DXMATRIXA16 matView, matProj;

		g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
		g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);

		matFinal = m_matLocal * m_matWorld;

		m_pEffect->SetMatrix("matWorld", &matFinal);
		m_pEffect->SetMatrix("matViewProjection", &(matView * matProj));
		m_pEffect->SetTexture("baseMap", m_pLighteningTex);

		//g_pD3DDevice->SetTexture(0, m_pLighteningTex);
		//g_pD3DDevice->SetTransform(D3DTS_WORLD, &matFinal);

		g_pD3DDevice->DrawIndexedPrimitiveUP(
			D3DPT_TRIANGLELIST,
			0, 4,
			2,
			m_nIb, D3DFMT_INDEX32,
			&m_stPTV[0], sizeof(ST_PT_VERTEX));

		m_pEffect->EndPass();
	}
	



	if (m_bIsDrawLine)
	{
		g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		D3DXMATRIXA16 matIdent;
		D3DXMatrixIdentity(&matIdent);
		
		g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matIdent);
		g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
		D3DCOLOR LineColor = 0xff00ffff;
		ST_PC_VERTEX LineQuad[4];
		LineQuad[0].p = D3DXVECTOR3(-0.5f, 0.5f, 0.0f);
		LineQuad[1].p = D3DXVECTOR3(0.5f, 0.5f, 0.0f);
		LineQuad[2].p = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);
		LineQuad[3].p = D3DXVECTOR3(0.5f, -0.5f, 0.0f);
		DWORD LineIDX[8];
		LineIDX[0] = 0; LineIDX[1] = 1;
		LineIDX[2] = 1; LineIDX[3] = 3;
		LineIDX[4] = 3; LineIDX[5] = 2;
		LineIDX[6] = 2; LineIDX[7] = 0;
		for (int i = 0; i < 4; i++)
		{
			D3DXVec3TransformCoord(&LineQuad[i].p, &LineQuad[i].p, &(matFinal));
		}
		g_pD3DDevice->SetTexture(0, NULL);
		g_pD3DDevice->DrawIndexedPrimitiveUP(
			D3DPT_LINELIST,
			0, 4,
			4,
			LineIDX, D3DFMT_INDEX32,
			&LineQuad[0], sizeof(ST_PC_VERTEX));
		ST_PC_VERTEX LineForward[2];
		DWORD		forwardColor = 0xffffff00;
		LineForward[0].p = m_vPos;
		D3DXVECTOR3 vForward = D3DXVECTOR3(matFinal._31, matFinal._32, matFinal._33);
			//D3DXVECTOR3 vForward = D3DXVECTOR3(m_matLocal._31, m_matLocal._32, m_matLocal._33);
		
		D3DXVec3Normalize(&vForward, &vForward);
		LineForward[1].p = m_vPos+vForward*10.0f;
		
		
		LineForward[0].c = forwardColor;
		LineForward[1].c = forwardColor;

		g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, LineForward, sizeof(ST_PC_VERTEX));

	}
	

	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	g_pD3DDevice->SetFVF(prevFVF);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, prevLighting);
	g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, prevCullMode);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &prevWorld);


}

void cLighteningBreath::ForkRender()
{
}

void cLighteningBreath::TrailRender()
{
}

bool cLighteningBreath::IsDestroyTime()
{
	if(m_fLifeTime <= 0)
		return true;
	
	return false;
}
