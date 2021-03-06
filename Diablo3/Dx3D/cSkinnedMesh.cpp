#include "StdAfx.h"
#include "cSkinnedMesh.h"
#include "cAllocateHierarchy.h"
#include "cSkinnedMeshManager.h"

static LPD3DXMESH			pBoundingSphereMesh;

cSkinnedMesh::cSkinnedMesh(char* szFolder, char* szFilename)
	: m_pRootFrame(NULL)
	, m_pAnimController(NULL)
	, m_dwWorkingPaletteSize(0)
	, m_pmWorkingPalette(NULL)
	, m_pEffect(NULL)
	, m_fBlendTime(0.1f)
	, m_fPassedBlendTime(0.0f)
	, m_isAnimBlend(false)
	, m_bIsOver(true)
	, m_vMax(0, 0, 0)
	, m_vMin(0, 0, 0)
{
	cSkinnedMesh* pSkinnedMesh = g_pSkinnedMeshManager->GetSkinnedMesh(szFolder, szFilename);

	m_pRootFrame = pSkinnedMesh->m_pRootFrame;
	m_dwWorkingPaletteSize = pSkinnedMesh->m_dwWorkingPaletteSize;
	m_pmWorkingPalette = pSkinnedMesh->m_pmWorkingPalette;
	m_pEffect = pSkinnedMesh->m_pEffect;
	m_stBoundingSphere = pSkinnedMesh->m_stBoundingSphere;
	m_vMin = pSkinnedMesh->GetMin();
	m_vMax = pSkinnedMesh->GetMax();
	m_matWorld = pSkinnedMesh->m_matWorld;

	pSkinnedMesh->m_pAnimController->CloneAnimationController(
		pSkinnedMesh->m_pAnimController->GetMaxNumAnimationOutputs(),
		pSkinnedMesh->m_pAnimController->GetMaxNumAnimationSets(),
		pSkinnedMesh->m_pAnimController->GetMaxNumTracks(),
		pSkinnedMesh->m_pAnimController->GetMaxNumEvents(),
		&m_pAnimController);


}

cSkinnedMesh::cSkinnedMesh()
	: m_pRootFrame(NULL)
	, m_pAnimController(NULL)
	, m_dwWorkingPaletteSize(0)
	, m_pmWorkingPalette(NULL)
	, m_pEffect(NULL)
{
}

cSkinnedMesh::~cSkinnedMesh(void)
{
	SAFE_RELEASE(m_pAnimController);
	SAFE_RELEASE(pBoundingSphereMesh);
}

void cSkinnedMesh::Load(char* szDirectory, char* szFilename)
{
	m_pEffect = LoadEffect("MultiAnimation.hpp");

	D3DXMatrixIdentity(&m_matWorld);

	int nPaletteSize = 0;
	m_pEffect->GetInt("MATRIX_PALETTE_SIZE", &nPaletteSize);

	cAllocateHierarchy ah;
	ah.SetDirectory(szDirectory);
	ah.SetDefaultPaletteSize(nPaletteSize);

	std::string sFullPath(szDirectory);
	sFullPath += std::string(szFilename);

	D3DXLoadMeshHierarchyFromX(sFullPath.c_str(),
		D3DXMESH_MANAGED,
		g_pD3DDevice,
		&ah,
		NULL,
		(LPD3DXFRAME*)&m_pRootFrame,
		&m_pAnimController);

	m_stBoundingSphere = ah.GetBoundingSphere();
	m_vMin = ah.GetMin();
	m_vMax = ah.GetMax();
	//m_stBoundingSphere.vCenter = (ah.GetMin() + ah.GetMax()) / 2.0f;
	//m_stBoundingSphere.fRadius = D3DXVec3Length( &(ah.GetMin() - ah.GetMax()) );

	if (pBoundingSphereMesh == NULL)
	{
		D3DXCreateSphere(g_pD3DDevice,
			m_stBoundingSphere.fRadius,
			20,
			20,
			&pBoundingSphereMesh,
			NULL);

		
	}

	if (m_pmWorkingPalette)
		delete[] m_pmWorkingPalette;

	m_dwWorkingPaletteSize = ah.GetMaxPaletteSize();
	m_pmWorkingPalette = new D3DXMATRIX[m_dwWorkingPaletteSize];
	if (m_pmWorkingPalette == NULL)
	{
		m_dwWorkingPaletteSize = 0;
	}

	if (m_pRootFrame)
		SetupBoneMatrixPtrs(m_pRootFrame);
}


void cSkinnedMesh::UpdateAndRender(D3DXMATRIX* pMat)
{
	if (m_pAnimController)
	{
		if (m_isAnimBlend)
		{
			m_fPassedBlendTime += g_pTimeManager->GetDeltaTime();
			if (m_fPassedBlendTime >= m_fBlendTime)
			{
				m_isAnimBlend = false;
				m_pAnimController->SetTrackWeight(0, 1.0f);
				m_pAnimController->SetTrackEnable(1, false);
				//m_pAnimController->SetTrackSpeed(0, 0.5f);
			}
			else
			{
				float fWeight = m_fPassedBlendTime / m_fBlendTime;
				m_pAnimController->SetTrackWeight(0, fWeight);
				m_pAnimController->SetTrackWeight(1, 1.f - fWeight);
			}
		}
	
		m_pAnimController->AdvanceTime(g_pTimeManager->GetDeltaTime(), NULL);
	}

	if (m_pRootFrame)
	{
		D3DXMATRIXA16 matW;
		D3DXMatrixIdentity(&matW);
		D3DXMatrixRotationY(&m_matWorld, D3DXToRadian(180));
		if (pMat)
		{
			m_matWorld = m_matWorld * *pMat;
		}
		

		Update(m_pRootFrame, (D3DXMATRIXA16*)&m_matWorld);
		Render(m_pRootFrame);

		//if (pBoundingSphereMesh)
		//{
		//	D3DXMATRIX matT;
		//	D3DXMatrixTranslation(&matT,
		//		m_stBoundingSphere.vCenter.x,
		//		m_stBoundingSphere.vCenter.y,
		//		m_stBoundingSphere.vCenter.z);
		//	mat *= matT;
		//	g_pD3DDevice->SetTransform(D3DTS_WORLD, &mat);
		//	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		//	pBoundingSphereMesh->DrawSubset(0);
		//	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		//}
	}

	
}

void cSkinnedMesh::Render(ST_BONE* pBone /*= NULL*/)
{
	assert(pBone);

	// 각 프레임의 메시 컨테이너에 있는 pSkinInfo를 이용하여 영향받는 모든 
	// 프레임의 매트릭스를 ppBoneMatrixPtrs에 연결한다.
	if (pBone->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;
		
		
		// get bone combinations
		LPD3DXBONECOMBINATION pBoneCombos =
			(LPD3DXBONECOMBINATION)(pBoneMesh->pBufBoneCombos->GetBufferPointer());

		D3DXMATRIXA16 matViewProj, matView, matProj;
		g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
		g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);
		matViewProj = matView * matProj;

		D3DXMATRIXA16 mView, mInvView;
		g_pD3DDevice->GetTransform(D3DTS_VIEW, &mView);
		D3DXMatrixInverse(&mInvView, 0, &mView);
		D3DXVECTOR3 vEye = D3DXVECTOR3(0, 0, 0);
		D3DXVec3TransformCoord(&vEye, &vEye, &mInvView);

		// for each palette
		for (DWORD dwAttrib = 0; dwAttrib < pBoneMesh->dwNumAttrGroups; ++dwAttrib)
		{
			// set each transform into the palette
			for (DWORD dwPalEntry = 0; dwPalEntry < pBoneMesh->dwNumPaletteEntries; ++dwPalEntry)
			{
				DWORD dwMatrixIndex = pBoneCombos[dwAttrib].BoneId[dwPalEntry];
				if (dwMatrixIndex != UINT_MAX)
				{
					m_pmWorkingPalette[dwPalEntry] =
						pBoneMesh->pBoneOffsetMatrices[dwMatrixIndex] *
						(*pBoneMesh->ppBoneMatrixPtrs[dwMatrixIndex]);
				}
			}

			
			// set the matrix palette into the effect
			m_pEffect->SetMatrixArray("amPalette",
				m_pmWorkingPalette,
				pBoneMesh->dwNumPaletteEntries);

			//m_pEffect->SetMatrix("g_mViewProj", &matViewProj);
			m_pEffect->SetVector("vLightDiffuse", &D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
			m_pEffect->SetMatrix("g_mViewProj", &matViewProj);
			m_pEffect->SetVector("vLightDiffuse", &D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f));
			m_pEffect->SetVector("vWorldLightPos", &D3DXVECTOR4(500.0f, 500.0f, 500.0f, 1.0f));
			m_pEffect->SetVector("vWorldCameraPos", &D3DXVECTOR4(vEye, 1.0f));
			m_pEffect->SetVector("vMaterialAmbient", &D3DXVECTOR4(0.23f, 0.23f, 0.23f, 0.23f));
			m_pEffect->SetVector("vMaterialDiffuse", &D3DXVECTOR4(0.1f, 0.1f, 0.f, 1.0f));

			//m_pEffect->SetVector("vLightDiffuse", &D3DXVECTOR4(0.5f, 1.0f, 1.0f, 1.0f));
			//m_pEffect->SetVector("vMaterialAmbient", &D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
			//m_pEffect->SetVector("vMaterialDiffuse", &D3DXVECTOR4(0.8f, 0.8f, 0.8f, 1.0f));


			// we're pretty much ignoring the materials we got from the x-file; just set
			// the texture here

			if (!pBoneMesh->vecDTexture.empty())
			{
				m_pEffect->SetTexture("Diffuse_Tex",	pBoneMesh->vecDTexture[pBoneCombos[dwAttrib].AttribId]);
				m_pEffect->SetTexture("Normal_Tex",		pBoneMesh->vecNTexture[pBoneCombos[dwAttrib].AttribId]);
				m_pEffect->SetTexture("Specular_Tex",	pBoneMesh->vecSTexture[pBoneCombos[dwAttrib].AttribId]);
				m_pEffect->SetTexture("Emission_Tex",	pBoneMesh->vecETexture[pBoneCombos[dwAttrib].AttribId]);
			}
				

			// set the current number of bones; this tells the effect which shader to use
			m_pEffect->SetInt("CurNumBones", pBoneMesh->dwMaxNumFaceInfls - 1);

			// set the technique we use to draw
			m_pEffect->SetTechnique("Skinning20");

			UINT uiPasses, uiPass;

			if (!pBoneMesh->vecDTexture.empty())
			{
				// run through each pass and draw
				m_pEffect->Begin(&uiPasses, 0);
				for (uiPass = 0; uiPass < uiPasses; ++uiPass)
				{
					//if (uiPass == 0)
					//{
					//	D3DXMATRIX matS, matT;
					//	D3DXMatrixScaling(&matS, 10.0f, 10.f, 10.f);
					//	
					//	//matView, matProj;
					//
					//	m_pEffect->SetMatrix("g_mViewProj", &(matView * matS * matProj));
					//	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
					//	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
					//
					//	m_pEffect->BeginPass(uiPass);
					//	pBoneMesh->pWorkingMesh->DrawSubset(dwAttrib);
					//	m_pEffect->EndPass();
					//
					//}
					if (uiPass == 1)
					{
						m_pEffect->SetMatrix("g_mViewProj", &(matView * matProj));
						g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
						g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
					
						m_pEffect->BeginPass(uiPass);
						pBoneMesh->pWorkingMesh->DrawSubset(dwAttrib);
						m_pEffect->EndPass();
					}
					
					

				}
				m_pEffect->End();
			}
			//m_pEffect->SetBool("g_bIsOver", m_bIsOver);
			//if (!pBoneMesh->vecDTexture.empty())
			//{
			//	// run through each pass and draw
			//	m_pEffect->Begin(&uiPasses, 0);
			//	for (uiPass = 0; uiPass < uiPasses; ++uiPass)
			//	{
			//		m_pEffect->BeginPass(uiPass);
			//		pBoneMesh->pWorkingMesh->DrawSubset(dwAttrib);
			//		m_pEffect->EndPass();
			//	}
			//	m_pEffect->End();
			//}

			
		}

		//m_pEffectOutLine->SetMatrix("matWorld", (D3DXMATRIXA16*)(&pBone->CombinedTransformationMatrix));
		//m_pEffectOutLine->SetMatrix("matViewProjection", &matViewProj);
		//
		//for (DWORD dwAttrib = 0; dwAttrib < pBoneMesh->dwNumAttrGroups; ++dwAttrib)
		//{
		//	UINT numPasses = 0;
		//	m_pEffectOutLine->Begin(&numPasses, 0);
		//	pBoneMesh->pWorkingMesh->DrawSubset(dwAttrib);
		//	m_pEffectOutLine->BeginPass(0);
		//}
		
		
	}

	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);


	//재귀적으로 모든 프레임에 대해서 실행.
	if (pBone->pFrameSibling)
	{
		Render((ST_BONE*)pBone->pFrameSibling);
	}

	if (pBone->pFrameFirstChild)
	{
		Render((ST_BONE*)pBone->pFrameFirstChild);
	}
}

LPD3DXEFFECT cSkinnedMesh::LoadEffect(char* szFilename)
{
	LPD3DXEFFECT pEffect = NULL;

	D3DXMACRO mac[2] =
	{
		{ "MATRIX_PALETTE_SIZE_DEFAULT", "35" },
		{ NULL,                          NULL }
	};

	D3DCAPS9 caps;
	D3DXMACRO *pmac = NULL;
	g_pD3DDevice->GetDeviceCaps(&caps);
	if (caps.VertexShaderVersion > D3DVS_VERSION(1, 1))
		pmac = mac;

	DWORD dwShaderFlags = 0;

#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DXSHADER_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

#ifdef DEBUG_VS
	dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
#endif
#ifdef DEBUG_PS
	dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
#endif

	ID3DXBuffer* pBuffer = NULL;
	if (FAILED(D3DXCreateEffectFromFile(g_pD3DDevice,
		szFilename,
		pmac,
		NULL,
		dwShaderFlags,
		NULL,
		&pEffect,
		&pBuffer)))
	{
		// if creation fails, and debug information has been returned, output debug info
		if (pBuffer)
		{
			OutputDebugStringA((char*)pBuffer->GetBufferPointer());
			SAFE_RELEASE(pBuffer);
		}

		return NULL;
	}

	return pEffect;
}

void cSkinnedMesh::Update(ST_BONE* pCurrent, D3DXMATRIXA16* pmatParent)
{
	pCurrent->CombinedTransformationMatrix = pCurrent->TransformationMatrix;
	if (pmatParent)
	{
		pCurrent->CombinedTransformationMatrix =
			pCurrent->CombinedTransformationMatrix * (*pmatParent);
	}

	if (pCurrent->pFrameSibling)
	{
		Update((ST_BONE*)pCurrent->pFrameSibling, pmatParent);
	}

	if (pCurrent->pFrameFirstChild)
	{
		Update((ST_BONE*)pCurrent->pFrameFirstChild, &(pCurrent->CombinedTransformationMatrix));
	}

	LPD3DXANIMATIONSET pCurAS = NULL;
	m_pAnimController->GetTrackAnimationSet(0, &pCurAS);
	
	D3DXTRACK_DESC td;
	m_pAnimController->GetTrackDesc(0, &td);

	//현재 애니메이션의 전체 길이를 실행하고
	if (td.Position > pCurAS->GetPeriod())
	{
		td.Position = 0.0f;
		m_pAnimController->SetTrackDesc(0, &td);
	}
}

void cSkinnedMesh::SetupBoneMatrixPtrs(ST_BONE* pBone)
{
	assert(pBone);

	// 각 프레임의 메시 컨테이너에 있는 pSkinInfo를 이용하여 영향받는 모든 
	// 프레임의 매트릭스를 ppBoneMatrixPtrs에 연결한다.
	if (pBone->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;
		if (pBoneMesh->pSkinInfo)
		{
			LPD3DXSKININFO pSkinInfo = pBoneMesh->pSkinInfo;
			// pSkinInfo->GetNumBones() 으로 영향받는 본의 개수를 찾음.
			// pSkinInfo->GetBoneName(i) 로 i번 프레임의 이름을 찾음
			// D3DXFrameFind(루트 프레임, 프레임 이름) 로 프레임을 찾음.
			// 찾아서 월드매트릭스를 걸어줘라.
			DWORD dwNumBones = pSkinInfo->GetNumBones();
			for (DWORD i = 0; i < dwNumBones; ++i)
			{
				LPCSTR szBoneName = pSkinInfo->GetBoneName(i);
				if (szBoneName == NULL || strlen(szBoneName) == 0)
					continue;
				ST_BONE* pInfluence = (ST_BONE*)D3DXFrameFind(m_pRootFrame, szBoneName);
				pBoneMesh->ppBoneMatrixPtrs[i] = &(pInfluence->CombinedTransformationMatrix);
			}
		}
	}

	//재귀적으로 모든 프레임에 대해서 실행.
	if (pBone->pFrameSibling)
	{
		SetupBoneMatrixPtrs((ST_BONE*)pBone->pFrameSibling);
	}

	if (pBone->pFrameFirstChild)
	{
		SetupBoneMatrixPtrs((ST_BONE*)pBone->pFrameFirstChild);
	}
}

void cSkinnedMesh::SetAnimationIndex(int nIndex)
{
	assert(nIndex <= m_pAnimController->GetMaxNumAnimationSets() && "애니메이션 인덱스를 벗어났습니다");
	m_isAnimBlend = true;
	m_fPassedBlendTime = 0.0f;

	LPD3DXANIMATIONSET pPrevAnimSet = NULL;
	LPD3DXANIMATIONSET pNextAnimSet = NULL;

	D3DXTRACK_DESC stTrackDesc;
	m_pAnimController->GetTrackDesc(0, &stTrackDesc);

	m_pAnimController->GetTrackAnimationSet(0, &pPrevAnimSet);
	m_pAnimController->SetTrackAnimationSet(1, pPrevAnimSet);

	m_pAnimController->GetAnimationSet(nIndex, &pNextAnimSet);
	m_pAnimController->SetTrackAnimationSet(0, pNextAnimSet);
	m_pAnimController->SetTrackPosition(0, 0.0f);

	m_pAnimController->SetTrackDesc(1, &stTrackDesc);

	m_pAnimController->SetTrackWeight(0, 0.0f);
	m_pAnimController->SetTrackWeight(1, 1.0f);

	SAFE_RELEASE(pPrevAnimSet);
	SAFE_RELEASE(pNextAnimSet);
}

void cSkinnedMesh::SetAnimationIndex(char* szStateName)
{
	
	m_isAnimBlend = true;
	m_fPassedBlendTime = 0.0f;

	LPD3DXANIMATIONSET pPrevAnimSet = NULL;
	LPD3DXANIMATIONSET pNextAnimSet = NULL;

	D3DXTRACK_DESC stTrackDesc;
	m_pAnimController->GetTrackDesc(0, &stTrackDesc);

	m_pAnimController->GetTrackAnimationSet(0, &pPrevAnimSet);
	m_pAnimController->SetTrackAnimationSet(1, pPrevAnimSet);

	m_pAnimController->GetAnimationSetByName(szStateName, &pNextAnimSet);

	assert(pNextAnimSet != NULL && "애니메이션이름이 없습니다");

	m_pAnimController->SetTrackAnimationSet(0, pNextAnimSet);
	m_pAnimController->SetTrackPosition(0, 0.0f);

	m_pAnimController->SetTrackDesc(1, &stTrackDesc);

	m_pAnimController->SetTrackWeight(0, 0.0f);
	m_pAnimController->SetTrackWeight(1, 1.0f);

	SAFE_RELEASE(pPrevAnimSet);
	SAFE_RELEASE(pNextAnimSet);
}

D3DXMATRIX * cSkinnedMesh::AttachItem(char * szFileName)
{
	ST_BONE* pBone = (ST_BONE*)D3DXFrameFind(m_pRootFrame, szFileName);
	return &pBone->CombinedTransformationMatrix;
}

void cSkinnedMesh::ChangeItem(char * szName, char * szFileName)
{
	ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)D3DXFrameFind(m_pRootFrame, szName)->pMeshContainer;
	pBoneMesh->vecDTexture.resize(pBoneMesh->vecDTexture.size() + 1);
	pBoneMesh->vecDTexture[pBoneMesh->vecDTexture.size()-1] = g_pTextureManager->GetTexture(szFileName);
}

void cSkinnedMesh::Destroy()
{
	cAllocateHierarchy ah;
	D3DXFrameDestroy((LPD3DXFRAME)m_pRootFrame, &ah);
	SAFE_DELETE_ARRAY(m_pmWorkingPalette);
	SAFE_RELEASE(m_pEffect);
}

void cSkinnedMesh::SetRandomTrackPosition()
{
	m_pAnimController->SetTrackPosition(0, (rand() % 100) / 10.0f);
}


string cSkinnedMesh::GetCurAnimationName()
{
	LPD3DXANIMATIONSET pCurAS = NULL;

	m_pAnimController->GetTrackAnimationSet(0, &pCurAS);
	
	string name = pCurAS->GetName();
	return name;

}