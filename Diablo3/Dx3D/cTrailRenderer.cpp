#include "stdafx.h"
#include "cTrailRenderer.h"


cTrailRenderer::cTrailRenderer()
{
	D3DXMatrixIdentity(&m_matLeftLocal);
	D3DXMatrixIdentity(&m_matRightLocal);
	D3DXMatrixIdentity(&m_matWorld);
	m_pMatParent = NULL;
}


cTrailRenderer::~cTrailRenderer()
{
}

void cTrailRenderer::SetUp(float trailLiveTime, float width, LPDIRECT3DTEXTURE9 pTrailTex, D3DXCOLOR color, LPDIRECT3DTEXTURE9 pDistortTex)
{
	//�ʴ� 60 �������̶� �������� ���� �����ð��� ����� Trail Line �ִ�� 
	this->m_nTrailMaxNum = (int)(trailLiveTime * 60) * 2;		//+10 �� Ȥ�ó�.....

																//LOG_MGR->AddLog( "%d Trail ����", m_nTrailMaxNum );

																//Trail �����ð� ����
	this->m_fLineTime = trailLiveTime;

	//Trail ����ü �迭 ����
	this->m_pTrails = new Trail[this->m_nTrailMaxNum];

	//Vertex ���� Trail ���� 2 ��
	this->m_pTrailVertices = new TrailVertex[this->m_nTrailMaxNum * 2];

	//�ﰢ�� ���� Trail ���� 2 �� ( TrailIndex �̰� �ϳ��� Triangle �ϳ� �̴� )
	this->m_pTrailIndices = new TrailIndex[this->m_nTrailMaxNum * 2];

	//�ڽ� Transfom Widht ���� ���߾� ����
	this->m_vLeftTrailPos = D3DXVECTOR3(0, 0, 0);  //Transform.SetWorldPosition(width * -0.5f, 0.0f, 0.0f);
	this->m_vRightTrailPos = D3DXVECTOR3(0, 0, width);  //.SetWorldPosition(width * 0.5f, 0.0, 0.0f);

														//�ڽ� ���δ�.
														//this->Transform.AddChild(&m_LeftTransform);
														//this->Transform.AddChild(&m_RightTransform);

	D3DXMATRIXA16 matT;
	D3DXMatrixTranslation(&matT, m_vLeftTrailPos.x, m_vLeftTrailPos.y, m_vLeftTrailPos.z);
	m_matLeftLocal = matT;

	D3DXMatrixTranslation(&matT, m_vRightTrailPos.x, m_vRightTrailPos.y, m_vRightTrailPos.z);
	m_matRightLocal = matT;

	//�ε��� �ʱ�ȭ ( �� ���� ���� ���̸� Ȱ��ȭ �� ������ �������� �ʴ´� )
	this->m_nHeadIndex = 0;
	this->m_nTailIndex = 0;

	//Texture ����
	m_pTrailTexture = pTrailTex;
	m_pDistortTexture = pDistortTex;

	//������
	this->TrailGenStart();

	//�ܰ� Effect
	m_pDistortEffect = LoadEffect("./Resources/Shaders/Distort_1.fx"); //RESOURCE_FX->GetResource("../Resources/Shaders/Distort_1.fx");

																	   //GrabTexture
	g_pD3DDevice->CreateTexture(
		WINSIZE_X,
		WINSIZE_Y,
		1,							//�Ӹ�ü�� ����
		D3DUSAGE_RENDERTARGET,		//RenderTarget �� Texture�̴�
		D3DFMT_A8R8G8B8,			//TEXTURE ���� RenderTexture ����D3DFMT_A8R8G8B8 ������
		D3DPOOL_DEFAULT,			//RenderTarget �� Texture �� Pool �� Default
		&m_pGrabTexture,
		NULL);


	this->m_TrailColor = color;
}

void cTrailRenderer::Release()
{
	SAFE_RELEASE(m_pDistortEffect);
	SAFE_RELEASE(m_pGrabTexture);

	SAFE_DELETE_ARRAY(m_pTrails);
	SAFE_DELETE_ARRAY(m_pTrailVertices);
	SAFE_DELETE_ARRAY(m_pTrailIndices);
}

void cTrailRenderer::Update(float timeDelta)
{
	//i ���� Ȱ��ȭ �� ���� ��ġ�̴�.
	int i = m_nHeadIndex - 1; //m_nHeadIndex �� �߰��� ��ġ�̱� ������ ���� �ε����� �̰��� 1 �� ���ڸ� ���� �ȴ�.

							  //m_nHeadIndex �� -1 �̵Ǵ� ��� �ε����� Trail �迭�� �Ǹ����� �ε����� �ȴ�.
	if (i < 0)
		i = m_nTrailMaxNum - 1;

	//TailIndex ������ ���� �� ����
	m_nTailIndex = i;

	//����.
	while (true)
	{
		//��Ȱ��ȭ �� Trail �� �����ٸ� ���Ŀ��� ���� �� �̹Ƿ� Ż��
		if (m_pTrails[i].bLive == false)
			break;

		//Ȱ��ȭ�� Trail Line ���� �ð� ���.
		m_pTrails[i].leftTime -= timeDelta;

		//�̰� �ð��� �� �����ٸ� ���̻��� ���Ŀ� Ȱ��ȭ �� ������ ���� ���� �ǹǷ� ��Ȱ��ȭ ��Ű�� Ż��
		if (m_pTrails[i].leftTime < 0.0f) {
			m_pTrails[i].bLive = false;
			//LOG_MGR->AddLog( "%d ���λ���", i );
			break;
		}

		//i ���� ��� �ε��� ���� �Դٸ� �ѹ� ��ȸ �Ѱ��� �ǹǷ� ����
		if (i == m_nHeadIndex)
			break;

		//�ε����� ���ٷ� ���� ���� �ε����� �ϳ� ����.
		i--;

		//������ �Ǵ� ��� �迭�� �� ������ �ε����� ����.
		if (i < 0)
			i = m_nTrailMaxNum - 1;

		//TailIndex ������ ���� �� ����
		m_nTailIndex = i;
	}

	//������ �� ���̶��..
	if (this->b_LineGen)
	{
		D3DXMATRIXA16 finalMatrixLeft;
		D3DXMATRIXA16 finalMatrixRight;
		//if (m_pMatParent)
		//{
		//	finalMatrixLeft = m_matLeftLocal*m_matWorld*(*m_pMatParent);
		//	finalMatrixRight = m_matRightLocal*m_matWorld*(*m_pMatParent);
		//}
		//else
		//{
		//	finalMatrixLeft = m_matLeftLocal*m_matWorld;
		//	finalMatrixRight = m_matRightLocal*m_matWorld;
		//}
		m_matWorld = (*m_pMatParent);
		finalMatrixLeft = m_matLeftLocal*m_matWorld;
		finalMatrixRight = m_matRightLocal*m_matWorld;
		//���� ��ġ ����
		D3DXVECTOR3 nowLeftPos = D3DXVECTOR3(finalMatrixLeft._41, finalMatrixLeft._42, finalMatrixLeft._43);
		D3DXVECTOR3 nowRightPos = D3DXVECTOR3(finalMatrixRight._41, finalMatrixRight._42, finalMatrixRight._43);

		//������ ������ġ
		int nextLineIndex = m_nHeadIndex + 1;
		if (nextLineIndex >= m_nTrailMaxNum)	//�Ѿ�� �ٽ� ó������...
			nextLineIndex = 0;

		//��ġ�� ��ȭ�� �ְ� ������ ������ġ�� m_nTailIndex �� �ƴ϶��...
		if ((nowLeftPos != m_vPrevLeftTrailPos || nowRightPos != m_vPrevRightTrailPos) &&
			nextLineIndex != m_nTailIndex)
		{
			this->m_pTrails[this->m_nHeadIndex].p0 = nowLeftPos;
			this->m_pTrails[this->m_nHeadIndex].p1 = nowRightPos;
			this->m_pTrails[this->m_nHeadIndex].bLive = true;
			this->m_pTrails[this->m_nHeadIndex].leftTime = m_fLineTime;

			//��� �ε����� ���� �ε�����...
			m_nHeadIndex = nextLineIndex;
		}

		//���� ��ġ ���� ��ġ�� ����
		m_vPrevLeftTrailPos = nowLeftPos;
		m_vPrevRightTrailPos = nowRightPos;
	}

}

void cTrailRenderer::Render()
{
	DWORD triNum = 0;
	DWORD verNum = 0;

	int i = m_nHeadIndex - 1;
	if (i < 0)
		i = m_nTrailMaxNum - 1;

	//���� ���� �߰� ( ������ ������ �־�� �ﰢ���� �����ȴ�.
	m_pTrailVertices[verNum].pos = m_pTrails[i].p0;
	m_pTrailVertices[verNum].uv.y = 1;
	m_pTrailVertices[verNum].uv.x = 0.0f;		//���� ������ UV.x �����̴�.
	m_pTrailVertices[verNum].color = this->m_TrailColor;
	verNum++;

	m_pTrailVertices[verNum].pos = m_pTrails[i].p1;
	m_pTrailVertices[verNum].uv.y = 0;
	m_pTrailVertices[verNum].uv.x = 0.0f;		//���� ������ UV.x �����̴�.
	m_pTrailVertices[verNum].color = this->m_TrailColor;
	verNum++;

	//���� ���� ���� ���� 
	i--;
	if (i < 0)
		i = m_nTrailMaxNum - 1;

	while (true)
	{
		//�ϳ��� ��Ȱ��ȭ �� ������ �����ٸ�...
		if (m_pTrails[i].bLive == false)
			break;

		float uvX = 1.0f - (m_pTrails[i].leftTime / m_fLineTime);		//UV�� x �� ���� �ð���� ���ȴ�.

		m_pTrailVertices[verNum].pos = m_pTrails[i].p0;
		m_pTrailVertices[verNum].uv.y = 1;
		m_pTrailVertices[verNum].uv.x = uvX;
		m_pTrailVertices[verNum].color = this->m_TrailColor;

		verNum++;

		m_pTrailVertices[verNum].pos = m_pTrails[i].p1;
		m_pTrailVertices[verNum].uv.y = 0;
		m_pTrailVertices[verNum].uv.x = uvX;		//���� ������ UV.x �����̴�.
		m_pTrailVertices[verNum].color = this->m_TrailColor;

		verNum++;

		m_pTrailIndices[triNum + 0] = TrailIndex(triNum + 0, triNum + 1, triNum + 2);
		m_pTrailIndices[triNum + 1] = TrailIndex(triNum + 2, triNum + 1, triNum + 3);
		triNum += 2;

		if (i == m_nHeadIndex)
			break;

		i--;
		if (i < 0)
			i = m_nTrailMaxNum - 1;
	}

	//������ ��ġ�� ���� �������� ��������� ������ ���� ����� �ʱ�ȭ �Ǿ�� �Ѵ�.
	D3DXMATRIXA16	matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

	//��� �ٱ׸���.
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//Texture ����
	g_pD3DDevice->SetTexture(0, m_pTrailTexture);
	//g_pD3DDevice->SetTextureStageState(0 , D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	//g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	//
	//g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	//g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	//
	//
	//g_pD3DDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
	//g_pD3DDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	//
	//g_pD3DDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	//g_pD3DDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	//
	//g_pD3DDevice->SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_DISABLE);
	//g_pD3DDevice->SetTextureStageState(2, D3DTSS_ALPHAOP, D3DTOP_DISABLE);


	g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

	g_pD3DDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(0, 0, 0, 127));
	g_pD3DDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
	g_pD3DDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_BLENDFACTORALPHA);


	//���� ����.
	DWORD prevLight;
	g_pD3DDevice->GetRenderState(D3DRS_LIGHTING, &prevLight);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//���� ���� Ȱ��ȭ
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


	//���� �׸���.
	g_pD3DDevice->SetFVF(TrailVertex::FVF);
	g_pD3DDevice->DrawIndexedPrimitiveUP(
		D3DPT_TRIANGLELIST,
		0,
		verNum,
		triNum,
		m_pTrailIndices,
		D3DFMT_INDEX32,
		m_pTrailVertices,
		sizeof(TrailVertex));


	//�ٽ� ���� ���´�.
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, prevLight);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void cTrailRenderer::RenderDistort(cCamera * pCam)
{

}

void cTrailRenderer::TrailGenStart()
{
	//�� Ȱ��ȭ ���� true
	this->b_LineGen = true;
	D3DXMATRIXA16 finalMatrixLeft;
	D3DXMATRIXA16 finalMatrixRight;
	//if (m_pMatParent)
	//{

	//	finalMatrixLeft = m_matLeftLocal*m_matWorld*(*m_pMatParent);
	//	finalMatrixRight = m_matRightLocal*m_matWorld*(*m_pMatParent);
	//}
	//else
	//{
	m_matWorld = (*m_pMatParent);
	finalMatrixLeft = m_matLeftLocal*m_matWorld;
	finalMatrixRight = m_matRightLocal*m_matWorld;
	//}
	//���� ��ġ ���
	this->m_vPrevLeftTrailPos = D3DXVECTOR3(finalMatrixLeft._41, finalMatrixLeft._42, finalMatrixLeft._43);
	this->m_vPrevRightTrailPos = D3DXVECTOR3(finalMatrixRight._41, finalMatrixRight._42, finalMatrixRight._43);
}

void cTrailRenderer::TrailGenStop()
{
	//�� Ȱ��ȭ ���� false
	this->b_LineGen = false;
}
