#include "StdAfx.h"
#include "cAllocateHierarchy.h"
#include "cMtlTex.h"

cAllocateHierarchy::cAllocateHierarchy(void)
{
}

cAllocateHierarchy::~cAllocateHierarchy(void)
{
}

STDMETHODIMP cAllocateHierarchy::CreateFrame(THIS_ LPCSTR Name,
	LPD3DXFRAME *ppNewFrame)
{
	ST_BONE* pBone = new ST_BONE;
	D3DXMatrixIdentity(&pBone->TransformationMatrix);
	D3DXMatrixIdentity(&pBone->CombinedTransformationMatrix);

	// TODO : �̸��� �������ּ���.
	pBone->Name = NULL;
	if (Name)
	{
		pBone->Name = new char[strlen(Name) + 1];
		strcpy(pBone->Name, Name);
	}

	pBone->pFrameFirstChild = NULL;
	pBone->pFrameSibling = NULL;
	pBone->pMeshContainer = NULL;

	*ppNewFrame = pBone;

	return S_OK;
}

STDMETHODIMP cAllocateHierarchy::CreateMeshContainer(THIS_ LPCSTR Name,
	CONST D3DXMESHDATA *pMeshData,
	CONST D3DXMATERIAL *pMaterials,
	CONST D3DXEFFECTINSTANCE *pEffectInstances,
	DWORD NumMaterials,
	CONST DWORD *pAdjacency,
	LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	ST_BONE_MESH* pBoneMesh = new ST_BONE_MESH;

	pBoneMesh->pOrgMesh = pMeshData->pMesh;
	pBoneMesh->Name = NULL;
	pBoneMesh->pAdjacency = NULL;
	pBoneMesh->pNextMeshContainer = NULL;
	pBoneMesh->pEffects = NULL;
	pBoneMesh->pMaterials = NULL;

	// step 1. pSkinInfo ����
	SAFE_ADDREF(pSkinInfo);
	pBoneMesh->pSkinInfo = pSkinInfo;

	pBoneMesh->NumMaterials = 0;

	assert(pMeshData->Type == D3DXMESHTYPE_MESH);

	// step 2. ���� �޽� ����

	if (pMeshData->pMesh)
	{
		pMeshData->pMesh->CloneMeshFVF(
			pMeshData->pMesh->GetOptions(),
			pMeshData->pMesh->GetFVF(),
			g_pD3DDevice,
			&pBoneMesh->pWrkMesh);
	}

	SAFE_ADDREF(pMeshData->pMesh);

	pBoneMesh->vecMtlTex.resize(NumMaterials);
	for (DWORD i = 0; i < NumMaterials; ++i)
	{
		pBoneMesh->vecMtlTex[i] = new cMtlTex;

		D3DMATERIAL9& stMtl = pBoneMesh->vecMtlTex[i]->GetMtl();
		stMtl = pMaterials[i].MatD3D;

		if (pMaterials[i].pTextureFilename)
		{
			std::string sFile(pMaterials[i].pTextureFilename);
			std::string sFullPath = sFile;
			if (m_sFolder.length() > 0)
			{
				sFullPath = m_sFolder + "/" + sFile;
			}
			pBoneMesh->vecMtlTex[i]->SetTexture(g_pTextureManager->GetTexture(sFullPath));
		}
	}

	// step 3. pSkinInfo->GetNumBones()�� ����
	// ������� ��ġ�� ��� ���� ���� ��Ʈ���� ���� ����
	// ppBoneMatrixPtrs, pBoneOffsetMatrices, pCurrentBoneMatrices�� �����Ҵ�
	DWORD dwNumBones = pSkinInfo->GetNumBones();
	pBoneMesh->pBoneOffsetMatrices = new D3DXMATRIX[dwNumBones];
	pBoneMesh->pCurrentBoneMatrices = new D3DXMATRIX[dwNumBones];
	pBoneMesh->ppBoneMatrixPtrs = new D3DXMATRIX*[dwNumBones];

	// step 4. ���� �Ҵ�� pBoneOffsetMatrices ��Ʈ������ �� ����.
	// pSkinInfo->GetBoneOffsetMatrix(i)
	for (DWORD i = 0; i < dwNumBones; ++i)
	{
		pBoneMesh->pBoneOffsetMatrices[i] = *(pSkinInfo->GetBoneOffsetMatrix(i));
	}

	*ppNewMeshContainer = pBoneMesh;
	return S_OK;
}

STDMETHODIMP cAllocateHierarchy::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{
	// TODO : ���� �� �սô�.
	SAFE_DELETE_ARRAY(pFrameToFree->Name);
	SAFE_DELETE(pFrameToFree);
	return S_OK;
}

STDMETHODIMP cAllocateHierarchy::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	// TODO : ���� �� �սô�.
	ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pMeshContainerToFree;

	SAFE_RELEASE(pBoneMesh->pOrgMesh);
	for (size_t i = 0; i < pBoneMesh->vecMtlTex.size(); ++i)
	{
		SAFE_RELEASE(pBoneMesh->vecMtlTex[i]);
	}

	SAFE_DELETE_ARRAY(pBoneMesh->pBoneOffsetMatrices);
	SAFE_DELETE_ARRAY(pBoneMesh->pCurrentBoneMatrices);
	SAFE_DELETE_ARRAY(pBoneMesh->ppBoneMatrixPtrs);

	SAFE_RELEASE(pBoneMesh->pWrkMesh);

	SAFE_DELETE(pMeshContainerToFree);
	return S_OK;
}
