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
		//���ϸ��̼� �� ����
		LPD3DXANIMATIONSET animSet;

		//i ��° Animation �� ������ ��´�.
		m_pAnimController->GetAnimationSet(i, &animSet);

		//AnimationSet Vector �� ����
		this->m_vecAnimSet.push_back(animSet);

		//AnimationSet Map �� ����
		this->m_mapAnimSet.insert(std::make_pair(
			animSet->GetName(),
			animSet));
	}

	//�ϴ� ù��° Animation ���� ����
	this->Play(0);
}

void cAnimation::Update()
{
	m_pAnimController->GetTrackDesc(0, &m_Track_Desc_0);

	// m_pNowPlayAnimationSet->GetPeriod() Animation �� �ѽð��� ��´�.
	//���� Animation ���� Factor �� ����
	m_AnimationPlayFactor = m_Track_Desc_0.Position / m_pNowPlayAnimationSet->GetPeriod();


	//�������� �����ߴٸ�...
	if (m_AnimationPlayFactor >= 0.95f)
	{
		if (this->m_bIsLoop == false) {

			//���ư� Animation �� �ִٸ�..
			if (this->m_pPrevPlayAnimationSet != NULL)
			{
				m_fCrossFadeTime = m_fOutCrossFadeTime;
				m_fLeftCrossFadeTime = m_fOutCrossFadeTime;
				m_bIsLoop = true;
				this->SetAnimation(m_pPrevPlayAnimationSet);
				this->m_pPrevPlayAnimationSet = NULL;
			}

			//���ư� Animation �� ���ٸ�.
			else
			{
				this->Stop();
			}
		}
	}

	m_AnimationPlayFactor = m_AnimationPlayFactor - (int)m_AnimationPlayFactor;	//�����κ� �Ȼ����....


	if (m_bIsPlay)
	{
		//�ִϸ��̼��� �����Ŵ
		//m_pAnimController->AdvanceTime( timeDelta, NULL );
		m_fAnimDelta = g_pTimeManager->GetDeltaTime();
	}
	else
	{
		m_fAnimDelta = 0.0f;
	}


	//ũ�ν� ���̵尡 �������̶��..
	if (m_fLeftCrossFadeTime > 0.0f)
	{
		//���� ũ�ν����̵� �ð� ����
		this->m_fLeftCrossFadeTime -= g_pTimeManager->GetDeltaTime();

		//ũ�ν����̵� ��������.
		if (m_fLeftCrossFadeTime <= 0.0f)
		{
			m_pAnimController->SetTrackWeight(0, 1);
			m_pAnimController->SetTrackEnable(1, false);
		}

		else
		{
			float w1 = (m_fLeftCrossFadeTime / m_fCrossFadeTime);		//1�� Track ����ġ
			float w0 = 1.0f - w1;										//0�� Track ����ġ

																		//����ġ ����
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

		//ũ�ν� ���̵� Ÿ�� ���
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
		//ũ�ν� ���̵� Ÿ�� ���
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
