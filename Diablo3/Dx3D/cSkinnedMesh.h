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

	//�ϳ��� ����
	ST_BONE*					m_pRootFrame;
	DWORD						m_dwWorkingPaletteSize;
	D3DXMATRIX*					m_pmWorkingPalette;
	LPD3DXEFFECT				m_pEffect;
	ST_SPHERE					m_stBoundingSphere;

	/* =======�޽� �ε� ���� ������========*/
	LPD3DXMESH							m_pMesh;
	DWORD								m_dwMatNum;			//�ε��� �Ž��� ���� ����
	std::vector<LPDIRECT3DTEXTURE9>		m_vecTex;			//�ε��� �޽����� ����ϴ� Texture
	std::vector<LPDIRECT3DTEXTURE9>		m_vecNorm;			//�ε��� �޽����� ����ϴ� Texture
	std::vector<LPDIRECT3DTEXTURE9>		m_vecSpec;			//�ε��� �޽����� ����ϴ� Texture
	std::vector<LPDIRECT3DTEXTURE9>		m_vecEmi;			//�ε��� �޽����� ����ϴ� Texture
	std::vector<D3DMATERIAL9>			m_vecMat;			//�ε��� �޽����� ����ϴ� ���͸���...


	// ��ü���� ����
	LPD3DXANIMATIONCONTROLLER	m_pAnimController;
	D3DXVECTOR3					m_vPosition;
	SYNTHESIZE(D3DXVECTOR3, m_vMin, Min);
	SYNTHESIZE(D3DXVECTOR3, m_vMax, Max);

public:
	cSkinnedMesh(char* szFolder, char* szFilename);
	~cSkinnedMesh(void);

	void UpdateAndRender(D3DXMATRIX* pMat = NULL);
	void SetAnimationIndex(int nIndex);
	void SetAnimationIndex(char* szStateName);

	D3DXMATRIX* AttachItem(char* szFileName);
	void ChangeItem(char* szName, char* szFileName);

	void SetRandomTrackPosition(); // �׽�Ʈ��
	void SetPosition(D3DXVECTOR3 v)
	{
		m_vPosition = v;
		m_stBoundingSphere.vCenter = v;
	}
	ST_SPHERE* GetBoundingSphere()
	{
		return &m_stBoundingSphere;
	}

	string GetCurAnimationName();
private:
	cSkinnedMesh();
	void Load(char* szFolder, char* szFilename);
	void LoadMultiTexture(char* szFolder, char*szFileName);
	LPD3DXEFFECT LoadEffect(char* szFilename);
	void Update(ST_BONE* pCurrent, D3DXMATRIXA16* pmatParent);
	void Render(ST_BONE* pBone = NULL);
	void SetupBoneMatrixPtrs(ST_BONE* pBone);
	void Destroy();

	
};

