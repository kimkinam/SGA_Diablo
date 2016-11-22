#pragma once
class cRay
{
public:
	enum eRaySpace
	{
		E_NONE,
		E_VIEW,
		E_WORLD,
		E_LOCAL
	};

protected:
	SYNTHESIZE(D3DXVECTOR3, m_vOrg, Org);
	SYNTHESIZE(D3DXVECTOR3, m_vDir, Dir);
	SYNTHESIZE(eRaySpace, m_eRaySpace, RaySpace);

public:
	cRay(void);
	~cRay(void);

	static cRay RayAtViewSpace(int nScreenX, int nScreenY);
	static cRay RayAtWorldSpace(int nScreenX, int nScreenY);

	bool IntersectTri(IN D3DXVECTOR3& v0,
		IN D3DXVECTOR3& v1,
		IN D3DXVECTOR3& v2,
		OUT D3DXVECTOR3& vPickedPosition);

	bool IntersectShpere(ST_SPHERE* pSphere);
};

