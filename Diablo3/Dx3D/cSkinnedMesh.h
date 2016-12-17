#pragma once

struct ST_BONE;

#define MOVE_TRANSITION_TIME 1.0f

class cSkinnedMesh
{
	friend class cSkinnedMeshManager;

private:
	float						m_fBlendTime;
	float						m_fPassedBlendTime;
	bool						m_isAnimBlend;

	//하나만 생성
	ST_BONE*					m_pRootFrame;
	DWORD						m_dwWorkingPaletteSize;
	D3DXMATRIX*					m_pmWorkingPalette;
	LPD3DXEFFECT				m_pEffect;
	ST_SPHERE					m_stBoundingSphere;
	D3DXMATRIXA16				m_matWorld;
	
	

	// 객체마다 생성
	SYNTHESIZE(LPD3DXANIMATIONCONTROLLER, m_pAnimController, AnimController);
	SYNTHESIZE(D3DXVECTOR3, m_vMin, Min);
	SYNTHESIZE(D3DXVECTOR3, m_vMax, Max);
	SYNTHESIZE(bool, m_bIsOver, IsOver);

public:
	cSkinnedMesh(char* szFolder, char* szFilename);
	~cSkinnedMesh(void);

	void UpdateAndRender(D3DXMATRIX* pMat = NULL);
	void SetAnimationIndex(int nIndex);
	void SetAnimationIndex(char* szStateName);

	D3DXMATRIX* AttachItem(char* szFileName);
	void ChangeItem(char* szName, char* szFileName);

	void SetRandomTrackPosition(); // 테스트용
	//void SetPosition(D3DXVECTOR3 v)
	//{
	//	m_vPosition = v;
	//	m_stBoundingSphere.vCenter = v;
	//}
	ST_SPHERE* GetBoundingSphere()
	{
		return &m_stBoundingSphere;
	}

	string GetCurAnimationName();
private:
	cSkinnedMesh();
	void Load(char* szFolder, char* szFilename);
	LPD3DXEFFECT LoadEffect(char* szFilename);
	void Update(ST_BONE* pCurrent, D3DXMATRIXA16* pmatParent);
	void Render(ST_BONE* pBone = NULL);
	void SetupBoneMatrixPtrs(ST_BONE* pBone);
	void Destroy();


};

