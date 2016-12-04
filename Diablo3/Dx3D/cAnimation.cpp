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
		m_pAnimController->AdvanceTime(g_pTimeManager->GetDeltaTime(), NULL );
		//m_fAnimDelta = g_pTimeManager->GetDeltaTime();
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
	m_bIsPlay = true;
	m_bIsLoop = false;

	MAP_ANIMSET::iterator find = this->m_mapAnimSet.find(animName);
	if (find != this->m_mapAnimSet.end())
	{

		//���� Animaiton �� ����Ѵ�.
		this->m_pPrevPlayAnimationSet = this->m_pNowPlayAnimationSet;

		//ũ�ν� ���̵� Ÿ�� ���
		m_fCrossFadeTime = inCrossFadeTime;
		m_fLeftCrossFadeTime = inCrossFadeTime;

		//������ ũ�ν����̵� Ÿ�� ���
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

		//���ư� Animation �� ����
		this->m_pPrevPlayAnimationSet = NULL;

		//ũ�ν� ���̵� Ÿ�� ���
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
//	assert(pNextAnimSet != NULL && "�ִϸ��̼��̸��� �����ϴ�");
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
	//�̹� �÷��̵ǰ� �ִ� Animation �̶�� ������ ���� �ʴ´�.
	if (this->m_pNowPlayAnimationSet != NULL &&
		animSet == this->m_pNowPlayAnimationSet)
		return;

	//ũ�ν� ���̵尡 �����Ѵٸ�..
	if (this->m_fCrossFadeTime > 0.0f)
	{
		//���� Animation �� 1 ��Track ���� ����
		m_pAnimController->SetTrackAnimationSet(1, m_pNowPlayAnimationSet);
		m_pAnimController->SetTrackPosition(1, m_Track_Desc_0.Position);	//������ �÷��� �Ǵ� ��ġ�� ����
		m_pAnimController->SetTrackEnable(1, true); //1 �� Track Ȱ��ȭ
		m_pAnimController->SetTrackWeight(1, 1.0f); //1 �� Track ����ġ
		m_pAnimController->SetTrackSpeed(1, m_Track_Desc_0.Speed);		//�ӵ� 

		//���ݼ��õǴ� Animation �� 0 ��Track ���� ����
		m_pNowPlayAnimationSet = animSet;
		m_pAnimController->SetTrackAnimationSet(0, animSet);
		m_pAnimController->SetTrackPosition(0, 0.0f);
		m_pAnimController->SetTrackWeight(0, 0.0f);	//����ġ�� 0 ���� 

		//���� �÷��� �ǰ� �ִ� AnimationSet ����
		this->m_pNowPlayAnimationSet = animSet;
	}

	//ũ�ν� ���̵尡 ���ٸ�...
	else
	{
		//Track �� �������� �Ǿ����� ������.
		this->m_pAnimController->SetTrackPosition(
			0,			//Track ��ȣ 
			0.0			//PlayPosition
			);


		//�Ű������� ���� AnimationSet ���� Animation 0 �� Track�� �÷���
		this->m_pAnimController->SetTrackAnimationSet(0, animSet);

		//���� �÷��� �ǰ� �ִ� AnimationSet ����
		this->m_pNowPlayAnimationSet = animSet;
	}
}
