#include "stdafx.h"
#include "cMap.h"
#include "cMtlTex.h"
#include "cObjLoader.h"

cMap::cMap()
	: m_pMesh(NULL)
	, m_pComMesh(NULL)
	, m_vPosition(0, 0, 0)
	, m_vScale(1.0f, 1.0f, 1.0f)
	, m_vDirection(0, 0, 1)
	, m_vRight(1, 0, 0)
	, m_vUp(0, 1, 0)
	//, m_bIsDrawBound(false)
	, m_pFogEffect(NULL)
{
	m_sSumNailName = m_sObjName = m_sFolderName = "";

	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matLocal);
}


cMap::~cMap()
{
	SAFE_RELEASE(m_pFogEffect);

	//통제 매쉬 해제
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pComMesh);

	//통짜 mtl 해제
	for each(auto c in m_vecMtl)
	{
		SAFE_RELEASE(c);
	}

	for each(auto c in m_vecComMtl)
	{
		SAFE_RELEASE(c);
	}

	//사라질 mtl 해제
	for each(auto c in m_vecHiddenMtl)
	{
		SAFE_RELEASE(c);
	}



	//사라질 매쉬 해제
	for each(auto c in m_vecHiddenObj)
	{
		SAFE_RELEASE(c);
	}

	for each(auto c in m_vecBoundBox)
	{
		SAFE_DELETE(c);
	}


}

void cMap::Setup(char * szFileName, char * szForlderName)
{
	m_sObjName = szFileName;
	m_sFolderName = szForlderName;

	cObjLoader loader;
	D3DXMATRIXA16 matS, matR;
	D3DXMatrixScaling(&matS, m_vScale.x, m_vScale.y, m_vScale.z);
	//D3DXMatrixRotationY(&matR, D3DXToRadian(90));
	m_matLocal = matS;// *matR;

	loader.Load(szFileName, szForlderName, &m_matLocal,
		m_vecMtl, m_pMesh,


		m_vecHiddenMtl, m_vecHiddenObj);

	cObjLoader loader2;
	m_pComMesh = loader2.Load(szFileName, szForlderName, m_vecComMtl, &m_matLocal);

	m_vecHiddenDraw.resize(m_vecHiddenObj.size());
	for (size_t i = 0; i < m_vecHiddenObj.size(); ++i)
		m_vecHiddenDraw[i] = (false);

	m_pFogEffect = LoadEffect("fog.txt");
	m_hFogTechHandle = m_pFogEffect->GetTechniqueByName("Fog");

}

void cMap::Setup(ST_SAVEOBJECT wObj)
{
	m_sObjName = wObj.szfileName;
	m_sFolderName = wObj.szFolderName;

	m_vPosition = wObj.vPosition;
	m_vDirection = wObj.vForward;
	m_vUp = wObj.vUp;
	m_vRight = wObj.vRight;
	m_vScale = wObj.vScale;

	D3DXVECTOR3 scaledRight		= m_vScale.x * m_vRight;
	D3DXVECTOR3 scaledUp		= m_vScale.y * m_vUp;
	D3DXVECTOR3 scaledForward	= m_vScale.z * m_vDirection;


	m_matLocal._11 = scaledRight.x;		m_matLocal._12 = scaledRight.y;		m_matLocal._13 = scaledRight.z;
	m_matLocal._21 = scaledUp.x;		m_matLocal._22 = scaledUp.y;		m_matLocal._23 = scaledUp.z;
	m_matLocal._31 = scaledForward.x;	m_matLocal._32 = scaledForward.y;	m_matLocal._33 = scaledForward.z;
	m_matLocal._41 = 0;					m_matLocal._42 = 0;					m_matLocal._43 = 0;

	cObjLoader loader;

	loader.Load(StringToChar(m_sObjName), StringToChar(m_sFolderName), &m_matLocal,
		m_vecMtl, m_pMesh,
		m_vecHiddenMtl, m_vecHiddenObj);

	cObjLoader loader2;
	m_pComMesh = loader2.Load(StringToChar(m_sObjName), StringToChar(m_sFolderName), m_vecComMtl, &m_matLocal);

	m_vecHiddenDraw.resize(m_vecHiddenObj.size());
	for (size_t i = 0; i < m_vecHiddenObj.size(); ++i)
		m_vecHiddenDraw[i] = (false);

	m_matWorld = m_matLocal;
	D3DXMatrixTranslation(&m_matLocal, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	SetLocalBoundBox(&m_matLocal);
	D3DXMatrixIdentity(&m_matLocal);

	m_pFogEffect = LoadEffect("fog.txt");
	m_hFogTechHandle = m_pFogEffect->GetTechniqueByName("Fog");

}

void cMap::Render()
{
	g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);

	D3DXMatrixTranslation(&m_matWorld, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	m_matWorld = m_matLocal * m_matWorld;
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);

	//m_pFogEffect->SetTechnique(m_hFogTechHandle);
	//
	//g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//g_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	//UINT numPasses = 0;
	//m_pFogEffect->Begin(&numPasses, 0);
	//D3DXMATRIXA16 I;
	//D3DXMatrixIdentity(&I);
	//for (size_t j = 0; j < numPasses; ++j)
	//{
	//	m_pFogEffect->BeginPass(j);
	for (size_t i = 0; i < m_vecMtl.size(); ++i)
	{
		g_pD3DDevice->SetMaterial(&m_vecMtl[i]->GetMtl());
		g_pD3DDevice->SetTexture(0, m_vecMtl[i]->GetTexture());

		m_pMesh->DrawSubset(i);
		//	}
		//	m_pFogEffect->EndPass();
		//}
		//m_pFogEffect->End();

		

		//D3DXMatrixIdentity(&m_matWorld);
		//g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
		//RenderBoundBox();
	}

	if (!m_vecHiddenObj.empty())
	{
		for (size_t j = 0; j < m_vecHiddenObj.size(); ++j)
		{
			if (m_vecHiddenDraw[j]) continue;
			for (size_t i = 0; i < m_vecHiddenMtl.size(); ++i)
			{
				g_pD3DDevice->SetMaterial(&m_vecHiddenMtl[i]->GetMtl());
				g_pD3DDevice->SetTexture(0, m_vecHiddenMtl[i]->GetTexture());

				m_vecHiddenObj[j]->DrawSubset(i);
			}
		}
	}
}

void cMap::RenerComplete()
{
	g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	D3DXMatrixTranslation(&m_matWorld, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	
	m_matWorld = m_matLocal * m_matWorld;

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);

	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	for (size_t i = 0; i < m_vecComMtl.size(); ++i)
	{
		g_pD3DDevice->SetMaterial(&m_vecComMtl[i]->GetMtl());
		g_pD3DDevice->SetTexture(0, m_vecComMtl[i]->GetTexture());
		m_pComMesh->DrawSubset(i);
	}
}

void cMap::RenderBoundBox()
{
	for (size_t i = 0; i < m_vecBoundBox.size(); ++i)
	{
		if (m_vecBoundBox[i]->GetIsDraw())
			m_vecBoundBox[i]->Render();
	}
}

void cMap::SetLocalBoundBox(D3DXMATRIX *mat)
{
	m_vecBoundBox.resize(m_vecHiddenObj.size());

	D3DXVECTOR3 vMin, vMax;

	for (size_t i = 0; i < m_vecBoundBox.size(); ++i)
	{
		GetBoundMinMax(m_vecHiddenObj[i], vMin, vMax);
		m_vecBoundBox[i] = new cBoundBox;
		if(mat)
			m_vecBoundBox[i]->Setup(vMin, vMax, mat);
		else
			m_vecBoundBox[i]->Setup(vMin, vMax, NULL);
	}
}

void cMap::CloneMap(cMap * map)
{
	this->SetDirection(map->GetDirection());
	this->SetRefMtl(map);
	this->SetScale(map->GetScale());
	this->SetFolderName(map->GetFolderName());
	this->SetRefObj(map);
	this->SetObjName(map->GetObjName());
	this->SetSumNailName(map->GetSumNailName());
	this->SetBoundBox(map->GetBoundBox());
	this->SetPosition(map->GetPosition());
	this->SetHiddenDraw(map->GetHiddenDraw());

	this->SetDirection(map->GetDirection());
	map->GetMesh()->CloneMeshFVF(
		map->GetMesh()->GetOptions(),
		map->GetMesh()->GetFVF(),
		g_pD3DDevice,
		&this->GetMesh());

	map->GetComMesh()->CloneMeshFVF(
		map->GetComMesh()->GetOptions(),
		map->GetComMesh()->GetFVF(),
		g_pD3DDevice,
		&this->GetComMesh());
}

void cMap::SetRefMtl(cMap* map)
{
	for (size_t i = 0; i < map->GetMtl().size(); ++i)
	{
		map->GetMtl()[i]->AddRef();
	}

	m_vecMtl = map->GetMtl();
	
	for (size_t i = 0; i < map->GetHiddenMtl().size(); ++i)
	{
		map->GetHiddenMtl()[i]->AddRef();
	}

	m_vecHiddenMtl = map->GetHiddenMtl();
	
	for (size_t i = 0; i < map->GetComMtl().size(); ++i)
	{
		map->GetComMtl()[i]->AddRef();
	}

	m_vecComMtl = map->GetComMtl();
	
	
}

void cMap::SetRefObj(cMap * map)
{
	for (size_t i = 0; i < map->GetHiddenObj().size(); ++i)
	{
		map->GetHiddenObj()[i]->AddRef();
	}

	m_vecHiddenObj = map->GetHiddenObj();
}

void cMap::SetNewDirection(D3DXVECTOR3 vDirection)
{
	m_vDirection = vDirection;
	
	D3DXVECTOR3 vRight;
	D3DXVECTOR3 vUp;
	
	//Right 재설정
	D3DXVec3Cross(&vRight, &D3DXVECTOR3(0, 1, 0), &m_vDirection);
	D3DXVec3Normalize(&m_vRight, &vRight);
	
	//Up 재설정
	D3DXVec3Cross(&vUp, &m_vDirection, &m_vRight);
	D3DXVec3Normalize(&m_vUp, &vUp);
	
	ChangeLocal();

	//D3DXMATRIXA16 matR;
	//D3DXMatrixIdentity(&matR);
	//D3DXMatrixRotationY(&matR, D3DXToRadian(90));
	//m_matLocal *= matR;

}

void cMap::ChangeLocal()
{
	D3DXVECTOR3 scaledRight = m_vScale.x * m_vRight;
	D3DXVECTOR3 scaledUp = m_vScale.y * m_vUp;
	D3DXVECTOR3 scaledForward = m_vScale.z * m_vDirection;
	
	m_matLocal._11 = m_vRight.x;		m_matLocal._12 = m_vRight.y;		m_matLocal._13 = m_vRight.z;
	m_matLocal._21 = m_vUp.x;			m_matLocal._22 = m_vUp.y;			m_matLocal._23 = m_vUp.z;
	m_matLocal._31 = m_vDirection.x;	m_matLocal._32 = m_vDirection.y;	m_matLocal._33 = m_vDirection.z;
	m_matLocal._41 = 0;					m_matLocal._42 = 0;					m_matLocal._43 = 0;

}



