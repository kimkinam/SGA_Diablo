#include "stdafx.h"
#include "cAnimation.h"


cAnimation::cAnimation()
	: m_bIsPlay(false)
	, m_bIsLoop(false)
	, m_bIsAnimBlend(false)
	, m_nAniNum(0)
	, m_pAnimController(NULL)
	, m_pNowPlayAnimationSet(NULL)
	, m_pPrevPlayAnimationSet(NULL)
{
	m_fCrossFadeTime = m_fLeftCrossFadeTime = m_fAnimDelta
		= m_fOutCrossFadeTime = m_AnimationPlayFactor = m_fPassedBlendTime
		= 0.0f;
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
		m_pAnimController->AdvanceTime(g_pTimeManager->GetDeltaTime(), NULL );
		//m_fAnimDelta = g_pTimeManager->GetDeltaTime();
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
	m_bIsPlay = true;
	m_bIsLoop = false;

	MAP_ANIMSET::iterator find = this->m_mapAnimSet.find(animName);
	if (find != this->m_mapAnimSet.end())
	{

		//현재 Animaiton 을 기억한다.
		this->m_pPrevPlayAnimationSet = this->m_pNowPlayAnimationSet;

		//크로스 페이드 타임 기억
		m_fCrossFadeTime = inCrossFadeTime;
		m_fLeftCrossFadeTime = inCrossFadeTime;

		//나갈때 크로스페이드 타입 기억
		m_fOutCrossFadeTime = outCrossFadeTime;

		this->SetAnimation(find->second);
	}
}

void cAnimation::PlayOneShotAfterHold(std::string animName, float crossFadeTime)
{
	m_bIsPlay = true;
	m_bIsLoop = false;

	MAP_ANIMSET::iterator find = this->m_mapAnimSet.find(animName);
	if (find != this->m_mapAnimSet.end())
	{

		//돌아갈 Animation 은 없다
		this->m_pPrevPlayAnimationSet = NULL;

		//크로스 페이드 타임 기억
		m_fCrossFadeTime = crossFadeTime;
		m_fLeftCrossFadeTime = crossFadeTime;

		this->SetAnimation(find->second);
	}
}
//
//void cAnimation::PlayAniByIndex(int nIndex)
//{
//
//}
//
//void cAnimation::PlayAniByName(char * szStateName)
//{
//
//	m_bIsAnimBlend = true;
//	m_fPassedBlendTime = 0.0f;
//
//	LPD3DXANIMATIONSET pPrevAnimSet = NULL;
//	LPD3DXANIMATIONSET pNextAnimSet = NULL;
//
//	D3DXTRACK_DESC stTrackDesc;
//	m_pAnimController->GetTrackDesc(0, &stTrackDesc);
//
//	m_pAnimController->GetTrackAnimationSet(0, &pPrevAnimSet);
//	m_pAnimController->SetTrackAnimationSet(1, pPrevAnimSet);
//
//	m_pAnimController->GetAnimationSetByName(szStateName, &pNextAnimSet);
//
//	assert(pNextAnimSet != NULL && "애니메이션이름이 없습니다");
//
//	m_pAnimController->SetTrackAnimationSet(0, pNextAnimSet);
//	m_pAnimController->SetTrackPosition(0, 0.0f);
//
//	m_pAnimController->SetTrackDesc(1, &stTrackDesc);
//
//	m_pAnimController->SetTrackWeight(0, 0.0f);
//	m_pAnimController->SetTrackWeight(1, 1.0f);
//
//	SAFE_RELEASE(pPrevAnimSet);
//	SAFE_RELEASE(pNextAnimSet);
//}

void cAnimation::SetPlaySpeed(float speed)
{
	m_pAnimController->SetTrackSpeed(0, speed);
}

void cAnimation::ShowAnimationName(int x, int y)
{
}

void cAnimation::SetAnimation(LPD3DXANIMATIONSET animSet)
{
	//이미 플레이되고 있는 Animation 이라면 실행이 되지 않는다.
	if (this->m_pNowPlayAnimationSet != NULL &&
		animSet == this->m_pNowPlayAnimationSet)
		return;

	//크로스 페이드가 존재한다면..
	if (this->m_fCrossFadeTime > 0.0f)
	{
		//현제 Animation 을 1 번Track 으로 셋팅
		m_pAnimController->SetTrackAnimationSet(1, m_pNowPlayAnimationSet);
		m_pAnimController->SetTrackPosition(1, m_Track_Desc_0.Position);	//이전에 플레이 되던 위치로 셋팅
		m_pAnimController->SetTrackEnable(1, true); //1 번 Track 활성화
		m_pAnimController->SetTrackWeight(1, 1.0f); //1 번 Track 가중치
		m_pAnimController->SetTrackSpeed(1, m_Track_Desc_0.Speed);		//속도 

		//지금셋팅되는 Animation 을 0 번Track 으로 셋팅
		m_pNowPlayAnimationSet = animSet;
		m_pAnimController->SetTrackAnimationSet(0, animSet);
		m_pAnimController->SetTrackPosition(0, 0.0f);
		m_pAnimController->SetTrackWeight(0, 0.0f);	//가중치는 0 으로 

		//현재 플레이 되고 있는 AnimationSet 갱신
		this->m_pNowPlayAnimationSet = animSet;
	}

	//크로스 페이드가 없다면...
	else
	{
		//Track 의 포지션을 맨압으로 돌린다.
		this->m_pAnimController->SetTrackPosition(
			0,			//Track 번호 
			0.0			//PlayPosition
			);


		//매개변수로 받은 AnimationSet 으로 Animation 0 번 Track에 플레이
		this->m_pAnimController->SetTrackAnimationSet(0, animSet);

		//현재 플레이 되고 있는 AnimationSet 갱신
		this->m_pNowPlayAnimationSet = animSet;
	}
}
