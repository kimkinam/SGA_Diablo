#include "StdAfx.h"
#include "cParticleEmitter.h"


cParticleEmitter::cParticleEmitter(void)
	:m_matParent(NULL)
	, m_IsActvieAll(false)
{
}


cParticleEmitter::~cParticleEmitter(void)
{
}


void cParticleEmitter::Init(
		DWORD partcleNum,					//�� ��ƼŬ ��
		float emission,						//�ʴ� �߻���
		float liveTimeMin,					//��ƼŬ�ϳ��� �ð�
		float liveTimeMax,
		const D3DXVECTOR3& velocityMin,		//��ƼŬ ���� �ӵ�
		const D3DXVECTOR3& velocityMax,
		const D3DXVECTOR3& accelMin,		//��ƼŬ ����
		const D3DXVECTOR3& accelMax,
		const VEC_COLOR& colors,			//��ƼŬ �÷� ���͹迭
		const VEC_SCALE& scales,			//��ƼŬ ������ �÷��迭
		float scaleMin,						//��ƼŬ ������
		float scaleMax,
		LPDIRECT3DTEXTURE9 pPaticleTexture,	//��ƼŬ Texture
		bool bLocal  				//�̰� true �� ��ƼŬ�� �������� ����Transform Local �������� �����δ�.
		)
{
	//�ش� ��ƼŬ ������ �� ��ƼŬ ��
	m_PaticleNum = partcleNum;

	//����ƼŬ ����ŭ ���ؽ� �迭�� �����.
	m_ParticleVerticles = new PARTICLE_VERTEX[ m_PaticleNum ];

	//��ƼŬ ��ü ����
	m_pPaticles = new cParticle[ m_PaticleNum ];

	//�ʴ� ������
	m_fEmissionPerSec = emission;

	//�ʴ� ������ ���� �߻� ����
	m_fEmisionIntervalTime = 1.0f / m_fEmissionPerSec;

	//���� �ð��� 0
	m_fEmisionDeltaTime = 0.0f;

	//�߻� ���� false
	m_bEmission = false;


	//�÷� ����
	//m_Colors = colors;
	m_Colors.clear();
	for( int i = 0 ; i < colors.size() ;i++ )
		m_Colors.push_back( colors[i] );


	//������ ����
	//m_Scales = scales;
	m_Scales.clear();
	for( int i = 0 ; i < scales.size() ;i++ )
		m_Scales.push_back( scales[i] );
	

	//���� ���̺� Ÿ�� ����
	m_fStartLiveTimeMin = liveTimeMin;
	m_fStartLiveTimeMax = liveTimeMax;

	//���� �ӵ� ����
	m_StartVelocityMin = velocityMin;
	m_StartVelocityMax = velocityMax;

	//���� ���� ����
	m_StartAccelateMin = accelMin;
	m_StartAccelateMax = accelMax;

	
	//���� ������ ����
	m_fStartScaleMin = scaleMin;
	m_fStartScaleMax = scaleMax;


	//���ۼ��� �ʱ�ȭ
	m_dwParticleCount = 0;

	//Texture ����
	m_pTex = pPaticleTexture;


	//m_bLocal = bLocal;


	EmissionType = PZERO;


}

void cParticleEmitter::Release()
{
	SAFE_DELETE_ARRAY( m_pPaticles );
	SAFE_DELETE_ARRAY( m_ParticleVerticles );
}


//��� �ȹ����� ���� ��Ʈ����.
void cParticleEmitter::Burst( int num, float minSpeed, float maxSpeed, float maxLife, float minLife )
{
	for( int i = 0 ; i < num ; i++ )
	{
		D3DXVECTOR3 randVec( 
			RandomFloatRange( -1.0f, 1.0f ),
			RandomFloatRange( -1.0f, 1.0f ),
			RandomFloatRange( -1.0f, 1.0f ) );

		D3DXVec3Normalize( &randVec, &randVec  );
		randVec*= RandomFloatRange( minSpeed, maxSpeed );


		StartOneParticle( randVec, RandomFloatRange(maxLife,minLife ) );
	}
}

void cParticleEmitter::Fire(int num, D3DXVECTOR3 startPos, float minSpeed, float maxSpeed, float maxLife, float minLife)
{
	m_fEmisionDeltaTime += g_pTimeManager->GetDeltaTime();

	while (m_fEmisionDeltaTime >= 1.0f / num) {

		m_fEmisionDeltaTime -= 1.0f / num;
	//for (int i = 0; i < num; i++)
	//{
		D3DXVECTOR3 randVec(
			RandomFloatRange(-0.05f, 0.05f),
			RandomFloatRange(0.08f, 1.0f),
			RandomFloatRange(-0.05f, 0.05f));

		D3DXVec3Normalize(&randVec, &randVec);
		randVec *= RandomFloatRange(minSpeed, maxSpeed);

		D3DXVECTOR3 randVec2(
			RandomFloatRange(-0.01f, 0.01f),
			RandomFloatRange(0.1f, 1.0f),
			RandomFloatRange(-0.01f, 0.01f));

		randVec2 *= RandomFloatRange(minSpeed, maxSpeed);

		StartOneParticle(randVec, startPos,randVec2, RandomFloatRange(maxLife, minLife));
	}
	//}
}

void cParticleEmitter::FireTail(int num, D3DXVECTOR3 startPos, float minSpeed, float maxSpeed, float maxLife, float minLife)
{
	m_fEmisionDeltaTime += g_pTimeManager->GetDeltaTime();

	while (m_fEmisionDeltaTime >= 1.0f /num) {

		m_fEmisionDeltaTime -= 1.0f / num;
		
	

	//for (int i = 0; i < num; i++)
	//{
		//D3DXVECTOR3 randVec(
		//	RandomFloatRange(-0.05f, 0.05f),
		//	RandomFloatRange(-0.01f, 0.01f),
		//	RandomFloatRange(-0.05f, 0.05f));
		
		D3DXVECTOR3 randVec(
			RandomFloatRange(0.0f, 0.0f),
			RandomFloatRange(0.0f, 0.0f),
			RandomFloatRange(0.0f, 0.0f));

		D3DXVec3Normalize(&randVec, &randVec);
		randVec *= RandomFloatRange(minSpeed, maxSpeed);

		D3DXVECTOR3 randVec2(
			RandomFloatRange(-0.001f, 0.001f),
			RandomFloatRange(-0.001f, 0.001f),
			RandomFloatRange(-0.001f, 0.001f));

		randVec2 *= RandomFloatRange(minSpeed, maxSpeed);

		StartOneParticle(randVec, startPos, randVec2, RandomFloatRange(maxLife, minLife));

	}
	//}
}

void cParticleEmitter::LighteningBreath(int num, D3DXVECTOR3 startPos, D3DXVECTOR3 dir, float minSpeed, float maxSpeed, float minLife, float maxLife)
{
	m_fEmisionDeltaTime += g_pTimeManager->GetDeltaTime();

	while (m_fEmisionDeltaTime >= 1.0f / num) {

		m_fEmisionDeltaTime -= 1.0f/num;

		D3DXVECTOR3 vNormDir, vNormUp, vNormRight;
		D3DXVec3Normalize(&vNormDir, &dir);

		D3DXVec3Cross(&vNormRight, &D3DXVECTOR3(0, 1, 0), &vNormDir);
		D3DXVec3Normalize(&vNormRight, &vNormRight);

		D3DXVec3Cross(&vNormUp, &vNormDir, &vNormRight);
		D3DXVec3Normalize(&vNormUp, &vNormUp);

		D3DXMATRIXA16 matWorld;
		D3DXMatrixIdentity(&matWorld);

		memcpy(&matWorld._11, &vNormRight, sizeof(D3DXVECTOR3));
		memcpy(&matWorld._21, &vNormUp, sizeof(D3DXVECTOR3));
		memcpy(&matWorld._31, &vNormDir, sizeof(D3DXVECTOR3));

		//for (int i = 0; i < num; i++)
		//{
		D3DXVECTOR3 velocity = dir*3.0f;

		D3DXVECTOR3 randVec2(
			RandomFloatRange(0, 0),
			RandomFloatRange(0.3f, -0.3f),
			RandomFloatRange(-0.1f, 7.0));

		D3DXVec3TransformCoord(&randVec2, &randVec2, &matWorld);
		//life time = 2.3, 3.0f;
		StartOneParticle(dir, startPos, randVec2, RandomFloatRange(maxLife, minLife));
	}
	//}
}



void cParticleEmitter::BaseObjectUpdate( float timeDelta )	
{
		//��� ��ƼŬ ������Ʈ

	m_IsActvieAll = false;
	for( int i = 0 ; i < m_PaticleNum ; i++ ){
		if (m_pPaticles[i].isLive())
		{
			if (!m_IsActvieAll)
				m_IsActvieAll = true;
		}
		m_pPaticles[i].Update( timeDelta );
	}

	//�ʰ� ���� �߻� ���´�?
	if( m_bEmission ){

		//�ϳ� �߻��ϰ� �����ð�
 		m_fEmisionDeltaTime += timeDelta;

		while( m_fEmisionDeltaTime >= m_fEmisionIntervalTime){	

			m_fEmisionDeltaTime -= m_fEmisionIntervalTime;
			//��ƼŬ �ϳ� �߻�
			StartOneParticle();
		}

	}





}
void cParticleEmitter::BaseObjectRender()
{
	//if (!m_bEmission)return;

	if (!m_IsActvieAll) return;


	//�׸� ��ƼŬ ��
	DWORD drawParticleNum = 0;

	for( int i = 0 ; i < m_PaticleNum ; i++ ){

		//�ش���ƼŬ�� Ȱ��ȭ ���̴�?
		if( m_pPaticles[i].isLive() ){
		
			//�ش� ��ƼŬ�� ������ ��´�.
			m_pPaticles[i].GetParticleVertex(
				m_ParticleVerticles + drawParticleNum,
				m_Colors, m_Scales );


			drawParticleNum++;

		}



	}
	

	g_pD3DDevice->SetRenderState( D3DRS_LIGHTING, false );		//�������� ����.
	g_pD3DDevice->SetTexture(0, NULL);

	g_pD3DDevice->SetRenderState( D3DRS_POINTSPRITEENABLE, true );	//����Ʈ ���ö���Ʈ Ȱ��ȭ
	g_pD3DDevice->SetRenderState( D3DRS_POINTSCALEENABLE, true );	//����Ʈ�� �����ϰ� ���̰ڴ�.
	g_pD3DDevice->SetRenderState( D3DRS_POINTSIZE_MIN, FloatToDWORD( 0.0f ) );		//����Ʈ�� �ּ� ũ�� ( ȭ����� )
	g_pD3DDevice->SetRenderState( D3DRS_POINTSIZE_MAX, FloatToDWORD( 250.0f ) );		//����Ʈ�� �ִ� ũ�� ( ȭ����� )
	//g_pD3DDevice->SetRenderState( D3DRS_POINTSIZE, FloatToDWORD( 10.0f ) );			//����Ʈ ���� ������ ( ������ ����Ʈ ����� ������ ���õǴµ� );

	g_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, false );	//z ������ ���⸦ ���´�.



	
	//��µǴ� POINT size 
	//finalSize = viewportHeight * pointSize *  sqrt( 1 / ( A + B(D) + C(D^2) )  );
	//�Ʒ��� ���� �ϸ� �ڿ������� �Ÿ��� ���� �������� ��Ÿ��
	g_pD3DDevice->SetRenderState( D3DRS_POINTSCALE_A, FloatToDWORD( 0.0f ) );
	g_pD3DDevice->SetRenderState( D3DRS_POINTSCALE_B, FloatToDWORD( 0.0f ) );
	g_pD3DDevice->SetRenderState( D3DRS_POINTSCALE_C, FloatToDWORD( 1.0f ) );

	//���� ���� ����
	g_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
	g_pD3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	g_pD3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE);

	//Texture �� ���� Diffuse ���⼭�� �����÷��� ���İ� �� ���� ���� ����� �Ѵ�.
	g_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
	g_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );
	g_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE );

	D3DXMATRIXA16 matWorld;
	
	if( m_matParent )
		matWorld = *m_matParent;
	else
		D3DXMatrixIdentity( &matWorld );

	g_pD3DDevice->SetTransform( D3DTS_WORLD, &matWorld );


	//��ƼŬ Texture ����
	//g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->SetTexture( 0, m_pTex );

	//��ƼŬ ���� ���
	g_pD3DDevice->SetFVF( PARTICLE_VERTEX::FVF );
	g_pD3DDevice->DrawPrimitiveUP( 
		D3DPT_POINTLIST, 
		drawParticleNum, 
		m_ParticleVerticles, 
		sizeof( PARTICLE_VERTEX ) );


	//��ƼŬ �׸��� ���� ��ó��
	g_pD3DDevice->SetRenderState( D3DRS_LIGHTING, true );
	g_pD3DDevice->SetRenderState( D3DRS_POINTSPRITEENABLE, false );
	g_pD3DDevice->SetRenderState( D3DRS_POINTSCALEENABLE, false );
	g_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, false );
	g_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, true );	

	g_pD3DDevice->SetTexture( 0, NULL );

}

//��ƼŬ ���� ����
void cParticleEmitter::StartEmission()
{
	m_bEmission = true;
}

//��ƼŬ ���� ����
void cParticleEmitter::StopEmission()
{
	m_bEmission = false;
}





///////////////////////////////////////////////////




//��ƼŬ �ϳ� ����
void cParticleEmitter::StartOneParticle()
{
	if (m_dwParticleCount == this->m_PaticleNum)
	{
		return;
	}
	//���̺� Ÿ�� ����
	float liveTime = RandomFloatRange(
		m_fStartLiveTimeMin, m_fStartLiveTimeMax );

//	float liveTime = RandomFloatRange(
//		1, 3);
	//��ƼŬ�� ������ ��ġ
	D3DXVECTOR3 position;
	
	//������ �ƴѰ�� �ڽ��� ���� ��ġ���� �����ϰ� 
	if( m_matParent)
		position = D3DXVECTOR3(m_matParent->_41, m_matParent->_42,m_matParent->_43);

	//������ ��� 0 ���� �����Ѵ�.
	else
		position = D3DXVECTOR3( 0, 0, 0 );

	//���� ������ ���� �߰� ��ġ....
	if( EmissionType == PATICLE_EMISSION_TYPE::SPHERE )
	{
		//��������
		D3DXVECTOR3 randDir(
			RandomFloatRange( -1.0f, 1.0f ),
			RandomFloatRange( -1.0f, 1.0f ),
			RandomFloatRange( -1.0f, 1.0f ) );
	
		D3DXVec3Normalize( &randDir, &randDir );
	
		//�����Ÿ�
		float randDistance = RandomFloatRange( 0, SphereEmissionRange );
	
		//�߰���ġ
		position += randDir * randDistance;
	}
	
	else if( EmissionType == PATICLE_EMISSION_TYPE::SPHERE_OUTLINE )
	{
		//��������
		D3DXVECTOR3 randDir(
			RandomFloatRange( -1.0f, 1.0f ),
			RandomFloatRange( -1.0f, 1.0f ),
			RandomFloatRange( -1.0f, 1.0f ) );
		//D3DXVECTOR3 randDir(
		//	0,0,-1);
	
	
		D3DXVec3Normalize( &randDir, &randDir );
	
		//�߰���ġ
		position += randDir * SphereEmissionRange;
	}
	
	else if( EmissionType == PATICLE_EMISSION_TYPE::BOX )
	{
		//��������
		D3DXVECTOR3 randDir(
			RandomFloatRange( MinEmissionRangeX, MaxEmissionRangeX ),
			RandomFloatRange( MinEmissionRangeY, MaxEmissionRangeY ),
			RandomFloatRange( MinEmissionRangeZ, MaxEmissionRangeZ ) );
	
	
		//�߰���ġ
		position += randDir;
	}





	

	//���� ����
	D3DXVECTOR3 velocity;
//	velocity.x =RandomFloatRange( 0, -0 );
//	velocity.y =RandomFloatRange(0, -0);
//	velocity.z =RandomFloatRange(0, 0);
	velocity.x = RandomFloatRange(m_StartVelocityMin.x, m_StartVelocityMax.x);
	velocity.y = RandomFloatRange(m_StartVelocityMin.y, m_StartVelocityMax.y);
	velocity.z = RandomFloatRange(m_StartVelocityMin.z, m_StartVelocityMax.z);


	D3DXVECTOR3 accelation;
	accelation.x = RandomFloatRange(m_StartAccelateMin.x, m_StartAccelateMax.x);
	accelation.y = RandomFloatRange(m_StartAccelateMin.y, m_StartAccelateMax.y);
	accelation.z = RandomFloatRange(m_StartAccelateMin.z, m_StartAccelateMax.z);

	//accelation.x =  RandomFloatRange(0.4f,-0.4f);
	//accelation.y = RandomFloatRange( 0.4f,-0.4f );
	//accelation.z = RandomFloatRange( -1, 0 );
	


	//�ڽ��� ���� ��Ʈ������ ������ �´�.
	if(m_matParent !=NULL )
	{
		//D3DXMATRIXA16 matWorld = this->pTransform->GetFinalMatrix();
		D3DXVec3TransformNormal( &velocity, &velocity, m_matParent);
		D3DXVec3TransformNormal( &accelation, &accelation, m_matParent);
	}

	//position.x += *RandomFloatRange(0.3, -0.1);
	//position.y += *RandomFloatRange(0.3, -0.1f);
	//position.z += *RandomFloatRange(-0.3, 1);


	//�����ϵ� ����
	float scale = RandomFloatRange( m_fStartScaleMin, m_fStartScaleMax );


	//������� �߻���Ų��
	m_pPaticles[ m_dwParticleCount ].Start(
		liveTime,
		&position, &velocity, &accelation, scale );

	//���� ��ƼŬ�� ���� ���� ����
	m_dwParticleCount++;
	if( m_dwParticleCount == this->m_PaticleNum )
		m_dwParticleCount = 0;



}

//��ƼŬ �ϳ� ���� ( ���� �־ )
void cParticleEmitter::StartOneParticle( D3DXVECTOR3 dir, float life )
{
	
	float liveTime = life*0.1f;

	//��ƼŬ�� ������ ��ġ
	D3DXVECTOR3 position;
	if( m_matParent )
		position = D3DXVECTOR3( m_matParent->_41,m_matParent->_42,m_matParent->_43);
	else
		position = D3DXVECTOR3( 0, 0, 0 );


	//���� ����
	D3DXVECTOR3 velocity;
	velocity.x = dir.x;
	velocity.y = dir.y;
	velocity.z = dir.z;

	D3DXVECTOR3 accelation;
	accelation.x = RandomFloatRange( m_StartAccelateMin.x, m_StartAccelateMax.x );
	accelation.y = RandomFloatRange( m_StartAccelateMin.y, m_StartAccelateMax.y );
	accelation.z = RandomFloatRange( m_StartAccelateMin.z, m_StartAccelateMax.z );



	//�ڽ��� ���� ��Ʈ������ ������ �´�.
	if( m_matParent)
	{
		D3DXVec3TransformNormal( &velocity, &velocity, m_matParent);
		D3DXVec3TransformNormal( &accelation, &accelation, m_matParent);
	}

	//�����ϵ� ����
	float scale = RandomFloatRange( m_fStartScaleMin, m_fStartScaleMax );


	//�߻���Ų��
	m_pPaticles[ m_dwParticleCount ].Start(
		liveTime,
		&position, &velocity, &accelation, scale );


	m_dwParticleCount++;
	if( m_dwParticleCount == this->m_PaticleNum )
		m_dwParticleCount = 0;

}



//��ƼŬ �ϳ� ���� ( ����,�ӵ� �־ )
void cParticleEmitter::StartOneParticle(D3DXVECTOR3 dir, D3DXVECTOR3 accelSpeed, float life)
{

	float liveTime = life;

	//��ƼŬ�� ������ ��ġ
	D3DXVECTOR3 position;
	if (m_matParent)
		position = D3DXVECTOR3(m_matParent->_41, m_matParent->_42, m_matParent->_43);
	else
		position = D3DXVECTOR3(0, 0, 0);


	//���� ����
	D3DXVECTOR3 velocity;
	velocity.x = dir.x;
	velocity.y = dir.y;
	velocity.z = dir.z;

	D3DXVECTOR3 accelation;
	accelation.x = accelSpeed.x;
	accelation.y = accelSpeed.y;
	accelation.z = accelSpeed.z;



	//�ڽ��� ���� ��Ʈ������ ������ �´�.
	if (m_matParent)
	{
		D3DXVec3TransformNormal(&velocity, &velocity, m_matParent);
		D3DXVec3TransformNormal(&accelation, &accelation, m_matParent);
	}

	//�����ϵ� ����
	float scale = RandomFloatRange(m_fStartScaleMin, m_fStartScaleMax);


	//�߻���Ų��
	m_pPaticles[m_dwParticleCount].Start(
		liveTime,
		&position, &velocity, &accelation, scale);


	m_dwParticleCount++;
	if (m_dwParticleCount == this->m_PaticleNum)
		m_dwParticleCount = 0;

}

void cParticleEmitter::StartOneParticle(D3DXVECTOR3 dir, D3DXVECTOR3 pos, D3DXVECTOR3 accelSpeed, float life)
{
	float liveTime = life;

	//��ƼŬ�� ������ ��ġ
	D3DXVECTOR3 position;
	
	position = pos;


	//���� ����
	D3DXVECTOR3 velocity;
	velocity.x = dir.x;
	velocity.y = dir.y;
	velocity.z = dir.z;

	D3DXVECTOR3 accelation;
	accelation.x = accelSpeed.x;
	accelation.y = accelSpeed.y;
	accelation.z = accelSpeed.z;



	//�ڽ��� ���� ��Ʈ������ ������ �´�.
	if (m_matParent)
	{
		D3DXVec3TransformNormal(&velocity, &velocity, m_matParent);
		D3DXVec3TransformNormal(&accelation, &accelation, m_matParent);
	}

	//�����ϵ� ����
	float scale = RandomFloatRange(m_fStartScaleMin, m_fStartScaleMax);


	//�߻���Ų��
	m_pPaticles[m_dwParticleCount].Start(
		liveTime,
		&position, &velocity, &accelation, scale);


	m_dwParticleCount++;
	if (m_dwParticleCount == this->m_PaticleNum)
		m_dwParticleCount = 0;
}
