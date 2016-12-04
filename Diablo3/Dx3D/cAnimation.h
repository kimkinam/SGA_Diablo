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
	MAP_ANIMSET						m_mapAnimSet;				//Map 으로 되어있는 AnimationSet
	VEC_ANIMSET						m_vecAnimSet;				//Vector 로 되어있는 AnimationSet

	LPD3DXANIMATIONSET				m_pNowPlayAnimationSet;		//현제 플레이되고있는 AnimationSet
	D3DXTRACK_DESC					m_Track_Desc_0;				//0번 Track_Desc ( 0 번 Track 의 정보 )

	LPD3DXANIMATIONSET				m_pPrevPlayAnimationSet;	//OneShot 플레이시 한번 Animation 플레이되고 다시 되돌아갈 Animaiton

	float							m_fCrossFadeTime;			//크로스페이드 타임
	float							m_fLeftCrossFadeTime;		//남은크로스페이드 타임
	float							m_fOutCrossFadeTime;		//OneShot Animation 끝나고 되돌아갈때 CrossFade 타임
	double							m_AnimationPlayFactor;		//Animation 진행 Factor ( 0 이면 처음 1 일수록 마지막 )
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

	//Animation 정보를 출력한다.
	void ShowAnimationName(int x, int y);

	//해당 인덱스의 Animation Set 을 얻는다.
	LPD3DXANIMATIONSET GetAnimSet(int index)
	{
		return m_vecAnimSet[index];
	}

	//Animation 을 바꾼다.
	void SetAnimation(LPD3DXANIMATIONSET animSet);

};

