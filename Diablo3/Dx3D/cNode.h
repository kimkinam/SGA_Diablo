#pragma once
#include "cMtlTex.h"

class cNode : public cObject
{
private:
	ST_FRAME_INFO m_stFrameInfo;
	SYNTHESIZE_PASS_BY_REF(std::vector<ST_CONTROL_POS>, m_vecPos, Pos);
	SYNTHESIZE_PASS_BY_REF(std::vector<ST_CONTROL_ROT>, m_vecRot, Rot);
	SYNTHESIZE_PASS_BY_REF(std::vector<ST_PNT_VERTEX>, m_vecVertex, Vertex);
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIXA16, m_matLocalTM, LocalTM);
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIXA16, m_matWorldTM, WorldTM);
	SYNTHESIZE_PASS_BY_REF(ST_BOUNDING_SPHERE, m_stBounding, Bounding);
	SYNTHESIZE_PASS_BY_REF(LPDIRECT3DVERTEXBUFFER9, m_pVB, VB);

	SYNTHESIZE_ADD_REF(cMtlTex*, m_pMtlTex, MtlTex);
	std::vector<cNode*> m_vecChild;
	float nKeyFrame;

	D3DXVECTOR3	m_vecRaycastPos;
	D3DXVECTOR3 m_vecDirection;
	float		m_fHeight;

public:
	cNode();
	~cNode();
	
	void Update(D3DXMATRIXA16* pmatParent);
	void Render(bool isChecked);
	void AddChild(cNode* pChild);
	void Destroy();
	void CalcLocalTM(D3DXMATRIXA16* pmatParent);
	void SetFrameInfo(ST_FRAME_INFO frameInfo);

	void CalcAniPosTM(IN float nKeyFrame, OUT D3DXMATRIXA16* posTM);
	void CalcAniRotTM(IN float nKeyFrame, OUT D3DXMATRIXA16* rotTM);
};

