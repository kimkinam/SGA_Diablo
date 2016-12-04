#pragma once
class cAnimation
{
private:
	typedef std::map< std::string, LPD3DXANIMATIONSET >			MAP_ANIMSET;
	typedef std::vector< LPD3DXANIMATIONSET >					VEC_ANIMSET;

private:
	SYNTHESIZE(bool, m_bIsPlay, IsPlay);
	SYNTHESIZE(bool, m_bIsLoop, IsLoop);
	SYNTHESIZE(UINT, m_nAniNum, AniNum);

private:
	SYNTHESIZE_ADD_REF(LPD3DXANIMATIONCONTROLLER, m_pAnimController, AnimController);
	MAP_ANIMSET						m_mapAnimSet;				//Map ���� �Ǿ��ִ� AnimationSet
	VEC_ANIMSET						m_vecAnimSet;				//Vector �� �Ǿ��ִ� AnimationSet

	LPD3DXANIMATIONSET				m_pNowPlayAnimationSet;		//���� �÷��̵ǰ��ִ� AnimationSet
	D3DXTRACK_DESC					m_Track_Desc_0;				//0�� Track_Desc ( 0 �� Track �� ���� )

	LPD3DXANIMATIONSET				m_pPrevPlayAnimationSet;	//OneShot �÷��̽� �ѹ� Animation �÷��̵ǰ� �ٽ� �ǵ��ư� Animaiton

	float							m_fCrossFadeTime;			//ũ�ν����̵� Ÿ��
	float							m_fLeftCrossFadeTime;		//����ũ�ν����̵� Ÿ��
	float							m_fOutCrossFadeTime;		//OneShot Animation ������ �ǵ��ư��� CrossFade Ÿ��
	double							m_AnimationPlayFactor;		//Animation ���� Factor ( 0 �̸� ó�� 1 �ϼ��� ������ )
	float							m_fAnimDelta;

public:
	cAnimation();
	~cAnimation();

	void Setup();
	void Update();
	void Play(std::string animName, float crossFadeTime = 0.0);
	void Play(int animIndex, float crossFadeTime = 0.0);

	void PlayOneShot(std::string animName, float inCrossFadeTime = 0.0, float outCrossFadeTime = 0.0f);
	void PlayOneShotAfterHold(std::string animName, float crossFadeTime = 0.0);

	void Stop() {
		this->m_bIsPlay = false;
	}

	void SetPlaySpeed(float speed);

	//Animation ������ ����Ѵ�.
	void ShowAnimationName(int x, int y);

	//�ش� �ε����� Animation Set �� ��´�.
	LPD3DXANIMATIONSET GetAnimSet(int index)
	{
		return m_vecAnimSet[index];
	}

	//Animation �� �ٲ۴�.
	void SetAnimation(LPD3DXANIMATIONSET animSet);

};

