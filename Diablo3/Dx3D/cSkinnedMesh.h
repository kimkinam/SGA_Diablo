#pragma once
#include "cAllocateHierarchy.h"

#define MOVE_TRANSITION_TIME 1.0f

class cSkinnedMesh
{
private:
	D3DXMATRIX*					m_pRootParent;
	float						m_fBlendTime;
	float						m_fPassedBlendTime;
	bool						m_isAnimBlend;
	SYNTHESIZE(ST_BONE*, m_pRoot, Root);
	SYNTHESIZE(LPD3DXANIMATIONCONTROLLER, m_pAnimController, AniController);
	SYNTHESIZE(D3DXVECTOR3, m_vPosition, Position);
	SYNTHESIZE(bool, m_bIsPlaying, IsPlaying);
public:
	cSkinnedMesh(void);
	virtual ~cSkinnedMesh(void);

	virtual void Load(char* szFolder, char* szFile);
	virtual void Update();
	virtual void Render(D3DXMATRIX* pMat = NULL);
	virtual void SetAnimation(int nIndex);
	virtual void FindFrame(char* szName, char* szFileName);
	virtual D3DXMATRIX* FindWithName(char* szName);

protected:
	virtual D3DXMATRIX* FindWithName(ST_BONE* pFrame, char* szName);
	virtual void Update(ST_BONE* pFrame, ST_BONE* pParent);
	virtual void Render(ST_BONE* pFrame, D3DXMATRIX* pMat = NULL);
	virtual void SetupBoneMatrixPtrs(ST_BONE* pFrame);
	virtual void UpdateSkinnedMesh(ST_BONE* pFrame);
};

