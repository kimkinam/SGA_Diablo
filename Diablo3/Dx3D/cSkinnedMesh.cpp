#include "StdAfx.h"
#include "cSkinnedMesh.h"
#include "cAllocateHierarchy.h"
#include "cMtlTex.h"

cSkinnedMesh::cSkinnedMesh(void)
	: m_pRoot(NULL)
	, m_pAnimController(NULL)
	, m_vPosition(0, 0, 0)
	, m_bIsPlaying(false)
	, m_fBlendTime(0.3f)
	, m_fPassedBlendTime(0.0f)
	, m_isAnimBlend(false)
{
}


cSkinnedMesh::~cSkinnedMesh(void)
{
	cAllocateHierarchy alloc;
	D3DXFrameDestroy((LPD3DXFRAME)m_pRoot, &alloc);
	SAFE_RELEASE(m_pAnimController);
}

void cSkinnedMesh::Load(char* szFolder, char* szFile)
{
	std::string sFolder(szFolder);
	std::string sFile(szFile);
	std::string sFullPath = sFolder + "/" + sFile;

	cAllocateHierarchy alloc;
	alloc.SetFolder(sFolder);

	D3DXLoadMeshHierarchyFromX(
		sFullPath.c_str(),
		D3DXMESH_MANAGED,
		g_pD3DDevice,
		&alloc,
		NULL,
		(LPD3DXFRAME*)&m_pRoot,
		&m_pAnimController);

	SetupBoneMatrixPtrs(m_pRoot);

	//UINT a = m_pAnimController->GetMaxNumTracks();
	//
	//
	LPD3DXANIMATIONSET pAS;
	m_pAnimController->GetAnimationSet(4, &pAS);
	m_pAnimController->SetTrackAnimationSet(0, pAS);

	SAFE_RELEASE(pAS);
}

void cSkinnedMesh::Update()
{
	if (m_isAnimBlend)
	{
		m_fPassedBlendTime += g_pTimeManager->GetDeltaTime();
		if (m_fPassedBlendTime >= m_fBlendTime)
		{
			m_isAnimBlend = false;
			m_pAnimController->SetTrackWeight(0, 1.0f);
			m_pAnimController->SetTrackEnable(1, false);
		}
		else
		{
			float fWeight = m_fPassedBlendTime / m_fBlendTime;
			m_pAnimController->SetTrackWeight(0, fWeight);
			m_pAnimController->SetTrackWeight(1, 1.f - fWeight);
		}
	}

	m_pAnimController->AdvanceTime(g_pTimeManager->GetDeltaTime(), NULL);
	//if (m_pRoot)
	//{
		Update(m_pRoot, NULL);
		if(m_bIsPlaying)
			UpdateSkinnedMesh(m_pRoot);
	//}
}

void cSkinnedMesh::Update(ST_BONE* pFrame, ST_BONE* pParent)
{
	pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;
	if (pParent)
	{
		pFrame->CombinedTransformationMatrix *=
			pParent->CombinedTransformationMatrix;
	}

	if (pFrame->pFrameFirstChild)
	{
		Update((ST_BONE*)pFrame->pFrameFirstChild, (ST_BONE*)pFrame);
	}

	if (pFrame->pFrameSibling)
	{
		Update((ST_BONE*)pFrame->pFrameSibling, (ST_BONE*)pParent);
	}
}


void cSkinnedMesh::Render(D3DXMATRIX* pMat)
{
	Render(m_pRoot, pMat);
}

void cSkinnedMesh::SetAnimation(int nIndex)
{
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

void cSkinnedMesh::Render(ST_BONE* pFrame, D3DXMATRIX* pMat)
{
	if (pFrame)
	{
		if (pMat)
			g_pD3DDevice->SetTransform(D3DTS_WORLD, &(pFrame->CombinedTransformationMatrix * (*pMat)));
		else
			g_pD3DDevice->SetTransform(D3DTS_WORLD, &pFrame->CombinedTransformationMatrix);
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pFrame->pMeshContainer;
		while (pBoneMesh)
		{
			for (size_t i = 0; i < pBoneMesh->vecMtlTex.size(); ++i)
			{
				g_pD3DDevice->SetMaterial(&pBoneMesh->vecMtlTex[i]->GetMtl());
				g_pD3DDevice->SetTexture(0, pBoneMesh->vecMtlTex[i]->GetTexture());
				pBoneMesh->pWrkMesh->DrawSubset(i);
			}
			pBoneMesh = (ST_BONE_MESH*)pBoneMesh->pNextMeshContainer;
		}
	}

	if (pFrame->pFrameFirstChild)
	{
		Render((ST_BONE*)pFrame->pFrameFirstChild, pMat);
	}

	if (pFrame->pFrameSibling)
	{
		Render((ST_BONE*)pFrame->pFrameSibling, pMat);
	}

}

void cSkinnedMesh::UpdateSkinnedMesh(ST_BONE* pFrame)
{
	// pCurrentBoneMatrices 를 계산하시오
	// pCurrentBoneMatrices = pBoneOffsetMatrices * ppBoneMatrixPtrs 

	ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pFrame->pMeshContainer;
	if (pBoneMesh)
	{
		LPD3DXSKININFO pSkinInfo = pBoneMesh->pSkinInfo;
		if (pSkinInfo)
		{
			DWORD dwNumBones = pSkinInfo->GetNumBones();
			for (DWORD i = 0; i < dwNumBones; ++i)
			{
				pBoneMesh->pCurrentBoneMatrices[i] =
					pBoneMesh->pBoneOffsetMatrices[i] *
					*(pBoneMesh->ppBoneMatrixPtrs[i]);
			}

			BYTE* src = NULL;
			BYTE* dest = NULL;

			pBoneMesh->pOrgMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&src);
			pBoneMesh->pWrkMesh->LockVertexBuffer(0, (void**)&dest);

			//pWrkMesh을 업데이트 시켜준다.
			pBoneMesh->pSkinInfo->UpdateSkinnedMesh(
				pBoneMesh->pCurrentBoneMatrices, NULL, src, dest);

			pBoneMesh->pWrkMesh->UnlockVertexBuffer();
			pBoneMesh->pOrgMesh->UnlockVertexBuffer();
		}
	}

	//재귀적으로 모든 프레임에 대해서 실행.
	if (pFrame->pFrameFirstChild)
	{
		UpdateSkinnedMesh((ST_BONE*)pFrame->pFrameFirstChild);
	}

	if (pFrame->pFrameSibling)
	{
		UpdateSkinnedMesh((ST_BONE*)pFrame->pFrameSibling);
	}


	//재귀적으로 모든 프레임에 대해서 실행.
}

void cSkinnedMesh::SetupBoneMatrixPtrs(ST_BONE* pFrame)
{
	// 각 프레임의 메시 컨테이너에 있는 pSkinInfo를 이용하여 영향받는 모든 
	// 프레임의 매트릭스를 ppBoneMatrixPtrs에 연결한다.
	// pSkinInfo->GetNumBones() 으로 영향받는 본의 개수를 찾음.
	// pSkinInfo->GetBoneName(i) 로 i번 프레임의 이름을 찾음
	// D3DXFrameFind(루트 프레임, 프레임 이름) 로 프레임을 찾음.
	// 찾아서 월드매트릭스를 걸어줘라.
	assert(pFrame != NULL);

	ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pFrame->pMeshContainer;
	if (pBoneMesh)
	{
		LPD3DXSKININFO pSkinInfo = pBoneMesh->pSkinInfo;
		if (pSkinInfo)
		{
			DWORD dwNumBones = pSkinInfo->GetNumBones();
			for (DWORD i = 0; i < dwNumBones; ++i)
			{
				ST_BONE* pBone = (ST_BONE*)D3DXFrameFind(m_pRoot, pSkinInfo->GetBoneName(i));
				pBoneMesh->ppBoneMatrixPtrs[i] = &pBone->CombinedTransformationMatrix;
			}
		}
	}

	//재귀적으로 모든 프레임에 대해서 실행.

	if (pFrame->pFrameFirstChild)
	{
		SetupBoneMatrixPtrs((ST_BONE*)pFrame->pFrameFirstChild);
	}

	if (pFrame->pFrameSibling)
	{
		SetupBoneMatrixPtrs((ST_BONE*)pFrame->pFrameSibling);
	}
}
