#include "stdafx.h"
#include "cAnimation.h"


cAnimation::cAnimation()
	: m_bIsPlay(false)
	, m_bIsLoop(false)
	, m_nAniNum(0)
	, m_pAnimController(NULL)
	, m_pNowPlayAnimationSet(NULL)
	, m_pPrevPlayAnimationSet(NULL)
{
	m_fCrossFadeTime = m_fLeftCrossFadeTime = m_fAnimDelta
		= m_fOutCrossFadeTime = m_AnimationPlayFactor = 0.0f;
}


cAnimation::~cAnimation()
{
	SAFE_RELEASE(m_pAnimController);
	SAFE_RELEASE(m_pNowPlayAnimationSet);
	SAFE_RELEASE(m_pPrevPlayAnimationSet);
}

void cAnimation::Setup()
{
	m_nAniNum = m_pAnimController->GetNumAnimationSets();

	for (UINT i = 0; i < m_nAniNum; i++)
	{
		//에니메이션 셋 정보
		LPD3DXANIMATIONSET animSet;

		//i 번째 Animation 셋 정보를 얻는다.
		m_pAnimController->GetAnimationSet(i, &animSet);

		//AnimationSet Vector 에 저장
		this->m_vecAnimSet.push_back(animSet);

		//AnimationSet Map 에 저장
		this->m_mapAnimSet.insert(std::make_pair(
			animSet->GetName(),
			animSet));
	}

	//일단 첫번째 Animation 으로 시작
	this->Play(0);
}

void cAnimation::Update()
{
	m_pAnimController->GetTrackDesc(0, &m_Track_Desc_0);

	// m_pNowPlayAnimationSet->GetPeriod() Animation 의 총시간을 얻는다.
	//현재 Animation 진행 Factor 를 갱신
	m_AnimationPlayFactor = m_Track_Desc_0.Position / m_pNowPlayAnimationSet->GetPeriod();


	//마지막에 도달했다면...
	if (m_AnimationPlayFactor >= 0.95f)
	{
		if (this->m_bIsLoop == false) {

			//돌아갈 Animation 이 있다면..
			if (this->m_pPrevPlayAnimationSet != NULL)
			{
				m_fCrossFadeTime = m_fOutCrossFadeTime;
				m_fLeftCrossFadeTime = m_fOutCrossFadeTime;
				m_bIsLoop = true;
				this->SetAnimation(m_pPrevPlayAnimationSet);
				this->m_pPrevPlayAnimationSet = NULL;
			}

			//돌아갈 Animation 이 없다면.
			else
			{
				this->Stop();
			}
		}
	}

	m_AnimationPlayFactor = m_AnimationPlayFactor - (int)m_AnimationPlayFactor;	//정수부분 안생기게....


	if (m_bIsPlay)
	{
		//애니메이션을 진행시킴
		//m_pAnimController->AdvanceTime( timeDelta, NULL );
		m_fAnimDelta = g_pTimeManager->GetDeltaTime();
	}
	else
	{
		m_fAnimDelta = 0.0f;
	}


	//크로스 페이드가 진행중이라면..
	if (m_fLeftCrossFadeTime > 0.0f)
	{
		//남은 크로스페이드 시간 뺀다
		this->m_fLeftCrossFadeTime -= g_pTimeManager->GetDeltaTime();

		//크로스페이드 가끝났다.
		if (m_fLeftCrossFadeTime <= 0.0f)
		{
			m_pAnimController->SetTrackWeight(0, 1);
			m_pAnimController->SetTrackEnable(1, false);
		}

		else
		{
			float w1 = (m_fLeftCrossFadeTime / m_fCrossFadeTime);		//1번 Track 가중치
			float w0 = 1.0f - w1;										//0번 Track 가중치

																		//가중치 셋팅
			m_pAnimController->SetTrackWeight(0, w0);
			m_pAnimController->SetTrackWeight(1, w1);
		}
	}
}

void cAnimation::Play(std::string animName, float crossFadeTime)
{
	m_bIsPlay = true;
	m_bIsLoop = true;

	MAP_ANIMSET::iterator find = this->m_mapAnimSet.find(animName);
	if (find != this->m_mapAnimSet.end())
	{

		//크로스 페이드 타임 기억
		m_fCrossFadeTime = crossFadeTime;
		m_fLeftCrossFadeTime = crossFadeTime;

		this->SetAnimation(find->second);
	}
}

void cAnimation::Play(int animIndex, float crossFadeTime)
{
	m_bIsPlay = true;
	m_bIsLoop = true;

	if (animIndex < this->m_nAniNum)
	{
		//크로스 페이드 타임 기억
		m_fCrossFadeTime = crossFadeTime;
		m_fLeftCrossFadeTime = crossFadeTime;

		this->SetAnimation(m_vecAnimSet[animIndex]);
	}
}

void cAnimation::PlayOneShot(std::string animName, float inCrossFadeTime, float outCrossFadeTime)
{
}

void cAnimation::PlayOneShotAfterHold(std::string animName, float crossFadeTime)
{
}

void cAnimation::SetPlaySpeed(float speed)
{
}

void cAnimation::ShowAnimationName(int x, int y)
{
}

void cAnimation::SetAnimation(LPD3DXANIMATIONSET animSet)
{
}
