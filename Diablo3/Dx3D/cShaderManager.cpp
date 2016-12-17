#include "stdafx.h"
#include "cShaderManager.h"


cShaderManager::cShaderManager() :
m_pEffect(NULL)
, m_pMesh(NULL)
, m_Texture1(NULL)
, m_Texture2(NULL)
, m_Texture3(NULL)
, Translation(0, 0, 0)
, Scaling(0, 0, 0)
, Alpha(0)
{
}


cShaderManager::~cShaderManager()
{
	SAFE_RELEASE(m_pEffect);
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_Texture1);
	SAFE_RELEASE(m_Texture2);
	SAFE_RELEASE(m_Texture3);
}





void cShaderManager::Setup(char* Shaderfilename, char* Xfilename, char* Texfilename1, char* Texfilename2, char* Texfilename3)
{
	string fileName(Shaderfilename);
	string filePath = "./Resources/Shaders/";
	string fullPath = filePath + fileName;
	m_pEffect = LoadShader(StringToChar(fullPath));

	string XfileName(Xfilename);
	string Xfile_fullPath = filePath + XfileName;
	LoadModel(StringToChar(Xfile_fullPath));




	if (Texfilename1)
	{
		string Tex1_fileName(Texfilename1);
		string Tex1file_fullPath = filePath + Tex1_fileName;
		D3DXCreateTextureFromFile(g_pD3DDevice, StringToChar(Tex1file_fullPath), &m_Texture1);
	}

	if (Texfilename2)
	{
		string Tex2_fileName(Texfilename2);
		string Tex2file_fullPath = filePath + Tex2_fileName;
		D3DXCreateTextureFromFile(g_pD3DDevice, StringToChar(Tex2file_fullPath), &m_Texture2);
	}

	if (Texfilename3)
	{
		string Tex3_fileName(Texfilename3);
		string Tex3file_fullPath = filePath + Tex3_fileName;
		D3DXCreateTextureFromFile(g_pD3DDevice, StringToChar(Tex3file_fullPath), &m_Texture3);
	}
}


void cShaderManager::Render()
{

	UINT numPasses = 0;


	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	//g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

	//g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pEffect->Begin(&numPasses, NULL);

	for (int j = 0; j < 3; j++)
	{
		for (UINT i = 0; i < numPasses; i++)
		{

			m_pEffect->BeginPass(i);

			m_pMesh->DrawSubset(j);
			m_pEffect->EndPass();

		}
	}
	//g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	m_pEffect->End();
}
void cShaderManager::Update()
{
	D3DXMatrixScaling(&matS, Scaling.x, Scaling.y, Scaling.z);
	D3DXMatrixTranslation(&matT, Translation.x, Translation.y, Translation.z);
}


void cShaderManager::Shader_info_Set(float fSpeed, float fWaveH, float fWave, float vspeed)
{
	float fTime;
	ULONGLONG tick = GetTickCount64();


	D3DXMATRIXA16 matWorld, matR, matView, matProj;
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXMatrixIdentity(&matWorld);

	matWorld = matWorld*matS*matT;

	m_pEffect->SetMatrix("matWorld", &matWorld);
	m_pEffect->SetMatrix("matWorldViewProjection", &(matWorld*matView*matProj));
	m_pEffect->SetFloat("fSpeed", fSpeed);
	m_pEffect->SetFloat("fWaveH", fWaveH);
	m_pEffect->SetFloat("fWaveF", fWave);
	m_pEffect->SetFloat("uvspeed", vspeed);
	m_pEffect->SetFloat("alpha", Alpha);
	m_pEffect->SetFloat("fTime", tick / 1000.0f);
	m_pEffect->SetTexture("DiffuseMap_Tex", m_Texture1);
	m_pEffect->SetTexture("Fieldstone_SM_Tex", m_Texture2); // 스펙이 돌아간다
	m_pEffect->SetTexture("Diablo_glow_Tex", m_Texture3);

}
void cShaderManager::Shader_info_Set(float Red, float Yellow, float Alpha)
{
	float fTime;
	ULONGLONG tick = GetTickCount64();


	D3DXMATRIXA16 matWorld, matR, matView, matProj;
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXMatrixIdentity(&matWorld);

	matWorld = matWorld*matS*matT;

	m_pEffect->SetMatrix("matWorld", &matWorld);
	m_pEffect->SetMatrix("matWorldViewProjection", &(matWorld*matView*matProj));
	m_pEffect->SetFloat("RedTime", Red); //R
	m_pEffect->SetFloat("YellowTime", Yellow);//G
	m_pEffect->SetFloat("alpha", Alpha);
	m_pEffect->SetFloat("fTime", tick / 1000.0f);
	m_pEffect->SetTexture("DiffuseMap_Tex", m_Texture1);
}



LPD3DXEFFECT cShaderManager::LoadShader(const char* filename)
{
	LPD3DXEFFECT effect = NULL;
	LPD3DXBUFFER pError = NULL;
	DWORD dwShaderFlags = 0;

#if _DEBUG
	dwShaderFlags = dwShaderFlags | D3DXSHADER_DEBUG;
#endif

	D3DXCreateEffectFromFile(g_pD3DDevice, filename, NULL, NULL, dwShaderFlags, NULL, &effect, &pError);

	if (!effect && pError)
	{
		int size = pError->GetBufferSize();
		void* ack = pError->GetBufferPointer();

		if (ack)
		{
			char* str = new char[size];
			sprintf(str, (const char*)ack, size);
			OutputDebugString(str);
			delete[] str;
		}

		return NULL;
	}
	return effect;
}

void cShaderManager::LoadModel(const char* filename)
{
	LPD3DXBUFFER	adjBuffer = NULL;
	LPD3DXBUFFER	mtrlBuffer = NULL;
	DWORD			numMtrls = NULL;


	D3DXLoadMeshFromX(filename, D3DXMESH_MANAGED, g_pD3DDevice
		, &adjBuffer, &mtrlBuffer, 0, &numMtrls, &m_pMesh);
}