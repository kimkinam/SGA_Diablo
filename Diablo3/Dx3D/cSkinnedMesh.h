#pragma once
#include "cAllocateHierarchy.h"

#define MOVE_TRANSITION_TIME 1.0f
class cSkinnedMesh
{
private:
	ST_BONE*					m_pRoot;
	D3DXMATRIX*					m_pRootParent;
	float						m_fBlendTime;
	float						m_fPassedBlendTime;
	bool						m_isAnimBlend;
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

protected:
	virtual void Update(ST_BONE* pFrame, ST_BONE* pParent);
	virtual void Render(ST_BONE* pFrame, D3DXMATRIX* pMat = NULL);
	virtual void SetupBoneMatrixPtrs(ST_BONE* pFrame);
	virtual void UpdateSkinnedMesh(ST_BONE* pFrame);
};

