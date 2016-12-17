#pragma once
class cShaderManager
{
private:
	LPD3DXMESH			m_pMesh;
	LPD3DXEFFECT		m_pEffect;
	LPDIRECT3DTEXTURE9	m_Texture1;
	LPDIRECT3DTEXTURE9  m_Texture2;
	LPDIRECT3DTEXTURE9  m_Texture3;


	D3DXMATRIXA16 matS;
	D3DXMATRIXA16 matT;
	SYNTHESIZE(D3DXVECTOR3, Scaling, Scaling_xyz);
	SYNTHESIZE(D3DXVECTOR3, Translation, Position_xyz);
	SYNTHESIZE(float, Alpha, Alpha_Down);


public:
	cShaderManager();
	~cShaderManager();

	void Setup(char* Shaderfilename, char* Xfilename, char* Texfilename1, char* Texfilename2, char* Texfilename3);

	LPD3DXEFFECT LoadShader(const char* filename); // �¾��� ���°�

	void LoadModel(const char* filename); // �¾��� ���°� 

	void Shader_info_Set(float fSpeed, float fWaveH, float fWave, float vspeed);// ������ ���°�

	//	void Shader_Matrix_Set(D3DXMATRIXA16 matS); // ������Ʈ�� ���°� 

	void Update();

	void Render();


};
