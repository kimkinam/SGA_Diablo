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
	//초당 60 프레임이란 기준으로 궤적 생존시간에 비례한 Trail Line 최대수 
	this->m_nTrailMaxNum = (int)(trailLiveTime * 60) * 2;		//+10 은 혹시나.....

																//LOG_MGR->AddLog( "%d Trail 생성", m_nTrailMaxNum );

																//Trail 생존시간 대입
	this->m_fLineTime = trailLiveTime;

	//Trail 구조체 배열 생성
	this->m_pTrails = new Trail[this->m_nTrailMaxNum];

	//Vertex 수는 Trail 수의 2 배
	this->m_pTrailVertices = new TrailVertex[this->m_nTrailMaxNum * 2];

	//삼각형 수는 Trail 수의 2 배 ( TrailIndex 이거 하나가 Triangle 하나 이다 )
	this->m_pTrailIndices = new TrailIndex[this->m_nTrailMaxNum * 2];

	//자식 Transfom Widht 폭에 맞추어 셋팅
	this->m_vLeftTrailPos = D3DXVECTOR3(0, 0, 0);  //Transform.SetWorldPosition(width * -0.5f, 0.0f, 0.0f);
	this->m_vRightTrailPos = D3DXVECTOR3(0, 0, width);  //.SetWorldPosition(width * 0.5f, 0.0, 0.0f);

														//자식 붙인다.
														//this->Transform.AddChild(&m_LeftTransform);
														//this->Transform.AddChild(&m_RightTransform);

	D3DXMATRIXA16 matT;
	D3DXMatrixTranslation(&matT, m_vLeftTrailPos.x, m_vLeftTrailPos.y, m_vLeftTrailPos.z);
	m_matLeftLocal = matT;

	D3DXMatrixTranslation(&matT, m_vRightTrailPos.x, m_vRightTrailPos.y, m_vRightTrailPos.z);
	m_matRightLocal = matT;

	//인덱스 초기화 ( 두 값이 같은 값이면 활성화 된 라인은 존재하지 않는다 )
	this->m_nHeadIndex = 0;
	this->m_nTailIndex = 0;

	//Texture 셋팅
	m_pTrailTexture = pTrailTex;
	m_pDistortTexture = pDistortTex;

	//젠시작
	this->TrailGenStart();

	//외곡 Effect
	m_pDistortEffect = LoadEffect("./Resources/Shaders/Distort_1.fx"); //RESOURCE_FX->GetResource("../Resources/Shaders/Distort_1.fx");

																	   //GrabTexture
	g_pD3DDevice->CreateTexture(
		WINSIZE_X,
		WINSIZE_Y,
		1,							//밉맵체인 레벨
		D3DUSAGE_RENDERTARGET,		//RenderTarget 용 Texture이다
		D3DFMT_A8R8G8B8,			//TEXTURE 포맷 RenderTexture 용은D3DFMT_A8R8G8B8 로하자
		D3DPOOL_DEFAULT,			//RenderTarget 용 Texture 는 Pool 을 Default
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
	//i 값을 활성화 된 시작 위치이다.
	int i = m_nHeadIndex - 1; //m_nHeadIndex 는 추가될 위치이기 때문에 시작 인덱스는 이거의 1 나 모자른 값이 된다.

							  //m_nHeadIndex 가 -1 이되는 경우 인덱스는 Trail 배열의 맨마지막 인덱스가 된다.
	if (i < 0)
		i = m_nTrailMaxNum - 1;

	//TailIndex 갱신을 위한 값 대입
	m_nTailIndex = i;

	//돈다.
	while (true)
	{
		//비활성화 된 Trail 을 만났다면 이후에는 없는 것 이므로 탈출
		if (m_pTrails[i].bLive == false)
			break;

		//활성화된 Trail Line 남은 시간 깐다.
		m_pTrails[i].leftTime -= timeDelta;

		//이게 시간이 다 까졌다면 더이상의 이후에 활성화 된 라인은 없는 것이 되므로 비활성화 시키고 탈출
		if (m_pTrails[i].leftTime < 0.0f) {
			m_pTrails[i].bLive = false;
			//LOG_MGR->AddLog( "%d 라인삭제", i );
			break;
		}

		//i 값이 헤드 인덱스 까지 왔다면 한번 순회 한것이 되므로 리턴
		if (i == m_nHeadIndex)
			break;

		//인덱스를 꺼꾸로 돌기 위해 인덱스를 하나 뺀다.
		i--;

		//음수가 되는 경우 배열의 맨 마지막 인덱스로 간다.
		if (i < 0)
			i = m_nTrailMaxNum - 1;

		//TailIndex 갱신을 위한 값 대입
		m_nTailIndex = i;
	}

	//라인이 젠 중이라면..
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
		//현제 위치 대입
		D3DXVECTOR3 nowLeftPos = D3DXVECTOR3(finalMatrixLeft._41, finalMatrixLeft._42, finalMatrixLeft._43);
		D3DXVECTOR3 nowRightPos = D3DXVECTOR3(finalMatrixRight._41, finalMatrixRight._42, finalMatrixRight._43);

		//라인의 다음위치
		int nextLineIndex = m_nHeadIndex + 1;
		if (nextLineIndex >= m_nTrailMaxNum)	//넘어가면 다시 처음으로...
			nextLineIndex = 0;

		//위치의 변화가 있고 라인의 다음위치가 m_nTailIndex 가 아니라면...
		if ((nowLeftPos != m_vPrevLeftTrailPos || nowRightPos != m_vPrevRightTrailPos) &&
			nextLineIndex != m_nTailIndex)
		{
			this->m_pTrails[this->m_nHeadIndex].p0 = nowLeftPos;
			this->m_pTrails[this->m_nHeadIndex].p1 = nowRightPos;
			this->m_pTrails[this->m_nHeadIndex].bLive = true;
			this->m_pTrails[this->m_nHeadIndex].leftTime = m_fLineTime;

			//헤드 인덱스는 다음 인덱스로...
			m_nHeadIndex = nextLineIndex;
		}

		//현제 위치 이전 위치로 갱신
		m_vPrevLeftTrailPos = nowLeftPos;
		m_vPrevRightTrailPos = nowRightPos;
	}

}

void cTrailRenderer::Render()
{
	//DWORD triNum = 0;
	//DWORD verNum = 0;

	//int i = m_nHeadIndex - 1;
	//if (i < 0)
	//	i = m_nTrailMaxNum - 1;

	////최초 정점 추가 ( 최초의 정점이 있어야 삼각형이 구성된다.
	//m_pTrailVertices[verNum].pos = m_pTrails[i].p0;
	//m_pTrailVertices[verNum].uv.y = 1;
	//m_pTrailVertices[verNum].uv.x = 0.0f;		//최초 정점은 UV.x 시작이다.
	//m_pTrailVertices[verNum].color = this->m_TrailColor;
	//verNum++;

	//m_pTrailVertices[verNum].pos = m_pTrails[i].p1;
	//m_pTrailVertices[verNum].uv.y = 0;
	//m_pTrailVertices[verNum].uv.x = 0.0f;		//최초 정점은 UV.x 시작이다.
	//m_pTrailVertices[verNum].color = this->m_TrailColor;
	//verNum++;

	////최초 정점 이전 부터 
	//i--;
	//if (i < 0)
	//	i = m_nTrailMaxNum - 1;

	//while (true)
	//{
	//	//하나라도 비활성화 된 정점을 만났다면...
	//	if (m_pTrails[i].bLive == false)
	//		break;

	//	float uvX = 1.0f - (m_pTrails[i].leftTime / m_fLineTime);		//UV의 x 는 남은 시간대비 계산된다.

	//	m_pTrailVertices[verNum].pos = m_pTrails[i].p0;
	//	m_pTrailVertices[verNum].uv.y = 1;
	//	m_pTrailVertices[verNum].uv.x = uvX;
	//	m_pTrailVertices[verNum].color = this->m_TrailColor;

	//	verNum++;

	//	m_pTrailVertices[verNum].pos = m_pTrails[i].p1;
	//	m_pTrailVertices[verNum].uv.y = 0;
	//	m_pTrailVertices[verNum].uv.x = uvX;		//최초 정점은 UV.x 시작이다.
	//	m_pTrailVertices[verNum].color = this->m_TrailColor;

	//	verNum++;

	//	m_pTrailIndices[triNum + 0] = TrailIndex(triNum + 0, triNum + 1, triNum + 2);
	//	m_pTrailIndices[triNum + 1] = TrailIndex(triNum + 2, triNum + 1, triNum + 3);
	//	triNum += 2;

	//	if (i == m_nHeadIndex)
	//		break;

	//	i--;
	//	if (i < 0)
	//		i = m_nTrailMaxNum - 1;
	//}

	////정점의 위치가 월드 기준으로 만들어졌기 때문에 월드 행렬을 초기화 되어야 한다.
	//D3DXMATRIXA16	matWorld;
	//D3DXMatrixIdentity(&matWorld);
	//g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

	////양면 다그린다.
	//g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	////Texture 셋팅
	//g_pD3DDevice->SetTexture(0, m_pTrailTexture);
	////g_pD3DDevice->SetTextureStageState(0 , D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	////g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	////
	////g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	////g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	////
	////
	////g_pD3DDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
	////g_pD3DDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	////
	////g_pD3DDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	////g_pD3DDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	////
	////g_pD3DDevice->SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_DISABLE);
	////g_pD3DDevice->SetTextureStageState(2, D3DTSS_ALPHAOP, D3DTOP_DISABLE);


	//g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	//g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

	//g_pD3DDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(0, 0, 0, 127));
	//g_pD3DDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
	//g_pD3DDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	//g_pD3DDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_BLENDFACTORALPHA);


	////광원 끈다.
	//DWORD prevLight;
	//g_pD3DDevice->GetRenderState(D3DRS_LIGHTING, &prevLight);
	//g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	////알파 블렌딩 활성화
	//g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	//g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


	////정점 그린다.
	//g_pD3DDevice->SetFVF(TrailVertex::FVF);
	//g_pD3DDevice->DrawIndexedPrimitiveUP(
	//	D3DPT_TRIANGLELIST,
	//	0,
	//	verNum,
	//	triNum,
	//	m_pTrailIndices,
	//	D3DFMT_INDEX32,
	//	m_pTrailVertices,
	//	sizeof(TrailVertex));


	////다시 돌려 놓는다.
	//g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//g_pD3DDevice->SetTexture(0, NULL);
	//g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, prevLight);
	//g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

DWORD prevLight, prevAlphaBlend, prevCullmode, prevFVF, prevTextureFactor, prevZBuffer;
   D3DXMATRIXA16 prevWorld;
   g_pD3DDevice->GetRenderState(D3DRS_LIGHTING, &prevLight);
   g_pD3DDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &prevAlphaBlend);
   g_pD3DDevice->GetRenderState(D3DRS_CULLMODE, &prevCullmode);
   g_pD3DDevice->GetFVF(&prevFVF);
   g_pD3DDevice->GetRenderState(D3DRS_TEXTUREFACTOR, &prevTextureFactor);
   g_pD3DDevice->GetRenderState(D3DRS_ZWRITEENABLE, &prevZBuffer);
   g_pD3DDevice->GetTransform(D3DTS_WORLD, &prevWorld);

   DWORD triNum = 0;
   DWORD verNum = 0;

   int i = m_nHeadIndex - 1;
   if (i < 0)
      i = m_nTrailMaxNum - 1;

   //최초 정점 추가 ( 최초의 정점이 있어야 삼각형이 구성된다.
   m_pTrailVertices[verNum].pos = m_pTrails[i].p0;
   m_pTrailVertices[verNum].uv.y = 1;
   m_pTrailVertices[verNum].uv.x = 0.0f;      //최초 정점은 UV.x 시작이다.
   m_pTrailVertices[verNum].color = this->m_TrailColor;
   verNum++;

   m_pTrailVertices[verNum].pos = m_pTrails[i].p1;
   m_pTrailVertices[verNum].uv.y = 0;
   m_pTrailVertices[verNum].uv.x = 0.0f;      //최초 정점은 UV.x 시작이다.
   m_pTrailVertices[verNum].color = this->m_TrailColor;
   verNum++;

   //최초 정점 이전 부터 
   i--;
   if (i < 0)
      i = m_nTrailMaxNum - 1;

   while (true)
   {
      //하나라도 비활성화 된 정점을 만났다면...
      if (m_pTrails[i].bLive == false)
         break;

      float uvX = 1.0f - (m_pTrails[i].leftTime / m_fLineTime);      //UV의 x 는 남은 시간대비 계산된다.

      m_pTrailVertices[verNum].pos = m_pTrails[i].p0;
      m_pTrailVertices[verNum].uv.y = 1;
      m_pTrailVertices[verNum].uv.x = uvX;
      m_pTrailVertices[verNum].color = this->m_TrailColor;

      verNum++;

      m_pTrailVertices[verNum].pos = m_pTrails[i].p1;
      m_pTrailVertices[verNum].uv.y = 0;
      m_pTrailVertices[verNum].uv.x = uvX;      //최초 정점은 UV.x 시작이다.
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

   //정점의 위치가 월드 기준으로 만들어졌기 때문에 월드 행렬을 초기화 되어야 한다.
   D3DXMATRIXA16   matWorld;
   D3DXMatrixIdentity(&matWorld);
   g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

   //양면 다그린다.
   g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

   //Texture 셋팅
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


   //g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
   //g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
   //
   //g_pD3DDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(0, 0, 0, 127));
   //g_pD3DDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
   //g_pD3DDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);
   //g_pD3DDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_BLENDFACTORALPHA);


   //광원 끈다.
   g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

   //알파 블렌딩 활성화
   g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
   g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
   g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
   g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


   //정점 그린다.
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


   //다시 돌려 놓는다.
   g_pD3DDevice->SetTexture(0, NULL);
   g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, prevLight);
   g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, prevAlphaBlend);
   g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, prevCullmode);
   g_pD3DDevice->SetFVF(prevFVF);
   g_pD3DDevice->SetRenderState(D3DRS_TEXTUREFACTOR, prevTextureFactor);
   g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, prevZBuffer);
   g_pD3DDevice->SetTransform(D3DTS_WORLD, &prevWorld);
}

void cTrailRenderer::RenderDistort(cCamera * pCam)
{

}

void cTrailRenderer::TrailGenStart()
{
	//젠 활성화 여부 true
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
	//이전 위치 기억
	this->m_vPrevLeftTrailPos = D3DXVECTOR3(finalMatrixLeft._41, finalMatrixLeft._42, finalMatrixLeft._43);
	this->m_vPrevRightTrailPos = D3DXVECTOR3(finalMatrixRight._41, finalMatrixRight._42, finalMatrixRight._43);
}

void cTrailRenderer::TrailGenStop()
{
	//젠 활성화 여부 false
	this->b_LineGen = false;
}
