#include "MultiAnimation.vsh"


float4		vLightDiffuse;		// = { 1.0f, 1.0f, 1.0f, 1.0f };   // Light Diffuse
float4		vMaterialAmbient;	// : MATERIALAMBIENT = { 1.0f, 0.1f, 0.1f, 1.0f };
float4		vMaterialDiffuse;	// : MATERIALDIFFUSE = { 0.8f, 0.8f, 0.8f, 1.0f };
float4		vWorldLightPos		= float4( 0.00, 500.00, -500.00, 1.00 );
float4		vWorldCameraPos		= float4( -50.00, 50.00, -50.00, 1.00 );

float4x4	g_mWorld			: WORLD;
float4x4	g_mViewProj			: VIEWPROJECTION;
texture		g_txScene;
bool		g_bIsOver = false;

//--------------------------------------------------------------------------------------
// Texture samplers
//--------------------------------------------------------------------------------------
texture Diffuse_Tex;
sampler2D DiffuseSample = sampler_state
{
	Texture = Diffuse_Tex;
MinFilter = Linear;
MagFilter = Linear;
MipFilter = Point;
};

texture Normal_Tex;
sampler2D NormalSample = sampler_state
{
	Texture = Normal_Tex;
MAGFILTER = ANISOTROPIC;
MINFILTER = ANISOTROPIC;
MIPFILTER = ANISOTROPIC;
};

texture Specular_Tex;
sampler2D SpecularSample = sampler_state
{
	Texture = Specular_Tex;
MAGFILTER = ANISOTROPIC;
MINFILTER = ANISOTROPIC;
MIPFILTER = ANISOTROPIC;
};


texture Emission_Tex;
sampler2D EmissionSample = sampler_state
{
	Texture = Emission_Tex;
MAGFILTER = ANISOTROPIC;
MIPFILTER = ANISOTROPIC;
MINFILTER = ANISOTROPIC;
};

texture Shadow_Tex;
sampler2D ShadowSampler = sampler_state
{
	Texture = Shadow_Tex;
ADDRESSU = BORDER;
ADDRESSV = BORDER;
BORDERCOLOR = 0xFFFFFFFF;
};


sampler g_samScene =
	sampler_state
{
	Texture = <Diffuse_Tex>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Point;
};


//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4  Pos         : POSITION;
	float3  BlendWeights: BLENDWEIGHT;
	float4  BlendIndices: BLENDINDICES;
	float3  Normal      : NORMAL;
	float3  TexCoord    : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 Pos			: POSITION;
	float4 Diffuse		: COLOR0;
	float2 TexCoord		: TEXCOORD0;
	float3 fDiffuse		: TEXCOORD1;
	float3 fViewDir		: TEXCOORD2;
	float3 fReflection	: TEXCOORD3;
};

float4 PixScene( 
	float4 Diffuse		: COLOR0,
	float2 TexCoord		: TEXCOORD0,
	float3 fDiffuse		: TEXCOORD1,
	float3 fViewDir		: TEXCOORD2,
	float3 fReflection	: TEXCOORD3) : COLOR0
{
	float3 color		= saturate(Diffuse);
	float3 reflaction	= normalize(fReflection);
	float3 viewDir		= normalize(fViewDir);
	float3 specular		= 0;

	float3 tangentNormal = tex2D(NormalSample, TexCoord);
	float4 Color;

	if (color.x > 0)
	{
			specular = saturate(dot(reflaction, -viewDir));
			specular = pow(specular, 20.0f);

			float4 albedo = tex2D(DiffuseSample, TexCoord) * float4(color + specular, 1.0f);
			float4 albedo1 = tex2D(EmissionSample, TexCoord) * float4(color + specular, 1.0f);
			Color = albedo + albedo1*6.0f;
			
	}
	return Color;
}

float4 PixScene2(
	float4 Diffuse : COLOR0,
	float2 TexCoord : TEXCOORD0,
	float3 fDiffuse : TEXCOORD1,
	float3 fViewDir : TEXCOORD2,
	float3 fReflection : TEXCOORD3) : COLOR0
{
	float3 color = saturate(Diffuse);
	float3 reflaction = normalize(fReflection);
	float3 viewDir = normalize(fViewDir);
	float3 specular = 0;
	
	float3 tangentNormal = tex2D(NormalSample, TexCoord);
	float4 Color;
	
	//if (!g_bIsOver)
	//{
	//	if (color.x > 0)
	//	{
	//		specular = saturate(dot(reflaction, -viewDir));
	//		specular = pow(specular, 20.0f);
	//
	//		float4 albedo = tex2D(DiffuseSample, TexCoord) * float4(color + specular, 1.0f);
	//		float4 albedo1 = tex2D(EmissionSample, TexCoord) * float4(color + specular, 1.0f);
	//		return Color = albedo + albedo1;
	//
	//	}
	//}
	return float4(1.0f, 0.0f, 0.0f, 1.0f);

}


VS_OUTPUT VertSkinning( VS_INPUT Input, uniform int nNumBones )
{
	VS_OUTPUT   Output;
	
	Output.fDiffuse		= float3(0, 0, 0);
	Output.fViewDir		= float3(0, 0, 0);
	Output.fReflection	= float3(0, 0, 0);

	float3	Pos			= 0.0f;
	float3	Normal		= 0.0f;
	float	LastWeight	= 0.0f;

	// skin VB inputs
	VS_SKIN_INPUT vsi = { Input.Pos, Input.BlendWeights, Input.BlendIndices, Input.Normal };
	VS_SKIN_OUTPUT vso = VS_Skin( vsi, nNumBones );

	// transform position from world space into view and then projection space
	Output.Pos = mul( float4( vso.vPos.xyz, 1.0f ), g_mViewProj );
	
	// normalize normals
	Normal = normalize( vso.vNor );

	Normal = mul(Normal, g_mWorld);
	Normal = mul(Normal, g_mViewProj);
	Normal = normalize(Normal);

	Output.Pos.xy += Normal.xy * 20.0f;

	float3 lightDir = vso.vPos.xyz - vWorldLightPos.xyz;
	lightDir = normalize(lightDir);

	// Shade (Ambient + etc.)
	Output.Diffuse = float4( vMaterialAmbient.xyz + saturate( dot( Normal, lightDir.xyz ) ) * vMaterialDiffuse.xyz, 1.0 );

	// copy the input texture coordinate through
	Output.TexCoord  = Input.TexCoord.xy;

	

	float3 ViewDir = vso.vPos.xyz - vWorldCameraPos.xyz;
	Output.fViewDir = normalize(ViewDir);

	float3 worldNormal = normalize(Normal);

	Output.fReflection = reflect(lightDir, worldNormal);

	return Output;
}


int CurNumBones = 10;

VertexShader vsArray20[ 4 ] = { 
	compile vs_2_0 VertSkinning( 1 ),
	compile vs_2_0 VertSkinning( 2 ),
	compile vs_2_0 VertSkinning( 3 ),
	compile vs_2_0 VertSkinning( 4 ) };


//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------

technique Skinning20
{
	//red
	pass p0
	{
		VertexShader = (vsArray20[CurNumBones]);
		PixelShader = compile ps_2_0 PixScene2();
	}

	pass p1
	{
		VertexShader = ( vsArray20[ CurNumBones ] );
		PixelShader = compile ps_2_0 PixScene();
	}

	
}
