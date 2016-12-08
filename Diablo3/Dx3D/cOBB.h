#pragma once

class cOBB
{
private:
	D3DXVECTOR3		m_vOrgCenterPos;
	D3DXVECTOR3		m_vOrgAxisDir[3];
	SYNTHESIZE(D3DXVECTOR3, m_vMax, Max);
	SYNTHESIZE(D3DXVECTOR3, m_vMin, Min);

	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vCenterPos, CenterPos);		// ���� �߾��� ��ǥ
	D3DXVECTOR3		m_vAxisDir[3];		// ���ڿ� ������ �� ���� ��������
	float			m_fAxisLen[3];		// ������ ������ �� ���� ���� fAxisLen[n]�� vAxisDir[n]�� ���� �����Ѵ�.
	float			m_fAxisHalfLen[3];

	D3DXMATRIXA16	m_matWorldTM;
public:
	cOBB(void);
	~cOBB(void);

	void Setup(cSkinnedMesh* pSkinnedMesh);
	void Setup(D3DXVECTOR3 vMin, D3DXVECTOR3 vMax);
	void Update(D3DXMATRIX* pmatWorld);
	void DebugRender(D3DCOLOR c);

	static bool IsCollision(cOBB* pOBB1, cOBB* pOBB2);
};

