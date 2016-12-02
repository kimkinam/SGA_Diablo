//**************************************************************//
//  Effect File exported by RenderMonkey 1.6
//
//  - Although many improvements were made to RenderMonkey FX  
//    file export, there are still situations that may cause   
//    compilation problems once the file is exported, such as  
//    occasional naming conflicts for methods, since FX format 
//    does not support any notions of name spaces. You need to 
//    try to create workspaces in such a way as to minimize    
//    potential naming conflicts on export.                    
//    
//  - Note that to minimize resulting name collisions in the FX 
//    file, RenderMonkey will mangle names for passes, shaders  
//    and function names as necessary to reduce name conflicts. 
//**************************************************************//

//--------------------------------------------------------------//
// Default_DirectX_Effect
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
string Default_DirectX_Effect_Pass_0_Model : ModelData = "..\\..\\..\\..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\Sphere.3ds";

float4x4 matWorld : World;
float4x4 matViewProjection : ViewProjection;

float4   vLightPosition
<
   string UIName = "vLightPosition";
   string UIWidget = "Direction";
   bool UIVisible =  false;
   float4 UIMin = float4( -10.00, -10.00, -10.00, -10.00 );
   float4 UIMax = float4( 10.00, 10.00, 10.00, 10.00 );
   bool Normalize =  false;
> = float4( 500.00, 500.00, 500.00, 1.00 );
float4   vViewPosition : ViewPosition;

struct VS_INPUT 
{
   float4 Position : POSITION0;
   float3 Normal : NORMAL;
   float3 Tangent : TANGENT;
   float3 Birnormal : BINORMAL;
   float2 UV       : TEXCOORD0;
   
};

struct VS_OUTPUT 
{
   float4 Position       : POSITION0;
   float2 UV             : TEXCOORD0;
   float3 LightDir       : TEXCOORD1;
   float3 ViewDirection  : TEXCOORD2;
   float3 T              : TEXCOORD3;
   float3 B              : TEXCOORD4;
   float3 N              : TEXCOORD5;
   
};

VS_OUTPUT Default_DirectX_Effect_Pass_0_Vertex_Shader_vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;

   Output.Position = mul( Input.Position, matViewProjection );
   Output.UV = Input.UV;
   
   float4 worldPosition = mul(Input.Position, matWorld);
   float3 lightDir = worldPosition.xyz - vLightPosition.xyz;
   Output.LightDir = normalize(lightDir);
   
   float3 viewDir = normalize(worldPosition.xyz - vViewPosition.xyz);
   Output.ViewDirection = viewDir;
   
   float3 worldNormal = mul(Input.Normal, (float3x3)matWorld);
   Output.N = normalize(worldNormal);
   
   float3 worldTangent = mul(Input.Tangent, (float3x3)matWorld);
   Output.T = normalize(worldTangent);
   
   float3 worldBinormal = mul(Input.Birnormal, (float3x3)matWorld);
   Output.B = normalize(worldBinormal);
   
   return Output;
   
}




texture DiffuseMap_Tex
<
   string ResourceName = "..\\..\\..\\shaderTest\\Fieldstone_DM.tga";
>;
sampler2D DiffuseSampler = sampler_state
{
   Texture = (DiffuseMap_Tex);
};
texture SpecularMap_Tex
<
   string ResourceName = "..\\..\\..\\shaderTest\\Fieldstone_SM.tga";
>;
sampler2D SpecularSampler = sampler_state
{
   Texture = (SpecularMap_Tex);
};
texture NormalMap_Tex
<
   string ResourceName = "..\\..\\..\\shaderTest\\Fieldstone_NM.tga";
>;
sampler2D NormalSampler = sampler_state
{
   Texture = (NormalMap_Tex);
};

float3 gLightColor
<
   string UIName = "gLightColor";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float3( 1.00, 1.00, 1.00 );

struct PS_INPUT
{
   float2 UV             : TEXCOORD0;
   float3 LightDir       : TEXCOORD1;
   float3 viewDir        : TEXCOORD2;
   float3 T              : TEXCOORD3;
   float3 B              : TEXCOORD4;
   float3 N              : TEXCOORD5;
   
};

float4 Default_DirectX_Effect_Pass_0_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR0
{  
  float3 tangentNormal = tex2D(NormalSampler, Input.UV).xyz;
  tangentNormal = normalize(tangentNormal * 2 -1);
  
  float3x3 TBN = float3x3(normalize(Input.T), normalize(Input.B), normalize(Input.N));
  TBN = transpose(TBN);
  
  float3 worldNormal = mul(TBN, tangentNormal);
  
  float4 albedo = tex2D(DiffuseSampler, Input.UV);
  float3 lightDir = normalize(Input.LightDir);
  float3 diffuse = saturate(dot(worldNormal, -lightDir));
  
  diffuse = gLightColor * albedo + diffuse;
  
  float3 specular = 0.0f;
  
  if(diffuse.x > 0)
  {
     float3 reflection = reflect(lightDir, worldNormal);
     float3 viewDir = normalize(Input.viewDir);
     
     specular = saturate(dot(reflection, -viewDir));
     specular = pow(specular, 20.0f);
     
     float4 specularIntensity = tex2D(SpecularSampler, Input.UV);
     specular *= specularIntensity * gLightColor;
  }
  
  float3 ambient = float3(0.1f, 0.1f, 0.1f) * albedo;
  
  return float4(ambient + diffuse + specular, 1.0f);
}



//--------------------------------------------------------------//
// Technique Section for Default_DirectX_Effect
//--------------------------------------------------------------//
technique Default_DirectX_Effect
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 Default_DirectX_Effect_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 Default_DirectX_Effect_Pass_0_Pixel_Shader_ps_main();
   }

}

