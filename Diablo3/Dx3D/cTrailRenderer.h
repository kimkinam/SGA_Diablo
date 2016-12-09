#pragma once
class cTrailRenderer
{
private:
	//Trail Line ����ü
	typedef struct tagTrail {
		D3DXVECTOR3 p0;
		D3DXVECTOR3 p1;
		float x;				//�ش� Trail �� UV X
		float leftTime;
		bool bLive;

		tagTrail() {
			bLive = false;
		}

	}Trail, *LPTrail;

	//Trail Vertex
	typedef struct tagTrailVertex {
		D3DXVECTOR3 pos;
		DWORD		color;
		D3DXVECTOR2 uv;

		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 };

		tagTrailVertex() {

			color = 0xff00ffff;
		}
	}TrailVertex, *LPTrailVertex;

	//Trail Index ����ü (index����ü 1���� �ﰢ�� 1��(vertex3��) )
	typedef struct tagTrailIndex {
		DWORD	i0;
		DWORD	i1;
		DWORD	i2;

		tagTrailIndex()
			: i0(0), i1(0), i2(0)
		{
		}

		tagTrailIndex(DWORD _0, DWORD _1, DWORD _2)
			: i0(_0), i1(_1), i2(_2)
		{
		}

	}TrailIndex, *LPTrailIndex;


private:
	D3DXVECTOR3 m_vPos;					//Ʈ���� ���� �̵�
	D3DXVECTOR3 m_vScale;				//Ʈ���� ���� ������
	D3DXVECTOR3 m_vDirection;			//Ʈ���� ���� ��
	D3DXVECTOR3 m_vUp;					//Ʈ���� ���� ��
	D3DXVECTOR3 m_vRight;				//Ʈ���� ���� ����

	D3DXMATRIXA16	m_matWorld;

	SYNTHESIZE(D3DXMATRIXA16*, m_pMatParent, Parent);

	//	D3DXMATRIXA16*	m_pMatBone;

	D3DXVECTOR3 m_vLeftTrailPos;				//Left 	���� ������ ��ġ
	D3DXVECTOR3 m_vRightTrailPos;			//Right ���� ������ ��ġ
	D3DXVECTOR3	m_vPrevLeftTrailPos;			//Left ���� ������ ��ġ 
	D3DXVECTOR3 m_vPrevRightTrailPos;			//Right ���� ������ ��ġ 
	D3DXMATRIXA16 m_matLeftLocal;
	D3DXMATRIXA16 m_matRightLocal;

	int				m_nTrailMaxNum;		//Trail Line ����ü �ִ� ��		
	LPTrail			m_pTrails;			//Trail Line ����ü ȯ��ť�� ����Ҳ���
	LPTrailVertex	m_pTrailVertices;	//���� ����ü �迭
	LPTrailIndex	m_pTrailIndices;	//���� �ε��� �迭

	bool		b_LineGen;				//���� ���� ���� 

	int			m_nHeadIndex;			//��� ��ġ ( ������ġ ) 
	int			m_nTailIndex;			//���� ��ġ
										//	____________________  <- LeftPos  					 	
										// |  |  |  |  |  |  |  |      
										// |  |  |  |  |  |  |  |     ->����
										// |--------------------|<-  RightPos
										// ��					��
										// TailIndex	     HeadIndex ( �߰��� ��ġ ������ �׷����� ���� Head �� ���� ���� �׷����� )

	float		m_fLineTime;						//�ϳ��� ������ ����ִ� �ð�

													//	LPDIRECT3DTEXTURE9		m_pTrailTexture;		//�Ϲ� Texture 
	SYNTHESIZE(LPDIRECT3DTEXTURE9, m_pTrailTexture, TrailTexture)

	LPDIRECT3DTEXTURE9		m_pDistortTexture;		//�ܰ ���Ǵ� Texture


	LPD3DXEFFECT			m_pDistortEffect;		//Distort Effect(���̴�)

	LPDIRECT3DTEXTURE9		m_pGrabTexture;			//(����ȭ���� �����ؼ�..Distort)�Ф� �κкҰ�

													//D3DXCOLOR				m_TrailColor;			//Ʈ���Ϸ����� ����
	SYNTHESIZE(D3DXCOLOR, m_TrailColor, Color);


public:
	cTrailRenderer();
	~cTrailRenderer();

	void		SetUp(float trailLiveTime, float width, LPDIRECT3DTEXTURE9 pTrailTex, D3DXCOLOR color, LPDIRECT3DTEXTURE9 pDistortTex);
	void		Release();
	void		Update(float timeDelta);
	void		Render();
	void		RenderDistort(cCamera* pCam);


	void		TrailGenStart();
	void		TrailGenStop();

};

