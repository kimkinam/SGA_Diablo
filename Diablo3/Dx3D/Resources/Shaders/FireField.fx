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
string Default_DirectX_Effect_Pass_0_Model : ModelData = "..\\..\\..\\..\\..\\..\\..\\PtT‘Å\\Cube.x";

float4x4 matWorld : World;
float4x4 matWorldViewProjection : WorldViewProjection;
float4   vLightPosition
<
   string UIName = "vLightPosition";
   string UIWidget = "Direction";
   bool UIVisible =  false;
   float4 UIMin = float4( -10.00, -10.00, -10.00, -10.00 );
   float4 UIMax = float4( 10.00, 10.00, 10.00, 10.00 );
   bool Normalize =  false;
> = float4( 0.00, 0.00, 0.00, 0.00 );
float4   vViewPosition : ViewPosition;


float    fTime;
float    fSpeed;
float    fWaveH;
float    fWaveF;
float    uvspeed;

struct VS_INPUT 
{
   float4 Position : POSITION0;
   float2 TexCoord : TEXCOORD0;
   float3 mNormal : NORMAL;
   float3 mBinormal : BINORMAL;
   float3 mTangent : TANGENT;
};

struct VS_OUTPUT 
{
   float4 Position       : POSITION0;
   float2 TexCoord       : TEXCOORD0;
   float3 ViewDirection  : TEXCOORD1;
   float3 LightDir       : TEXCOORD2;
 
  
   float3 T : TEXCOORD3;
   float3 B : TEXCOORD4;
   float3 N : TEXCOORD5;
   
};

VS_OUTPUT Default_DirectX_Effect_Pass_0_Vertex_Shader_vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;
   
    float cosTime = fWaveH*cos(fTime*fSpeed+Input.TexCoord.x*fWaveF);
   // Input.Position.x+=cosTime;
    Output.Position = mul(Input.Position,matWorldViewProjection);
    Output.TexCoord = Input.TexCoord+float2(0,0);
    float4 worldPosition = mul(Input.Position , matWorld); 
    float3 lightDir = worldPosition.xyz - vLightPosition.xyz;
    Output.LightDir = normalize(lightDir);
    
    float3 viewDir = normalize(worldPosition.xyz - vViewPosition.xyz );
    Output.ViewDirection = viewDir;
   
   
   float3 worldNormal = mul(Input.mNormal,(float3x3)matWorld);
   Output.N = normalize(worldNormal);
   
   float3 worldTangent = mul(Input.mTangent,(float3x3)matWorld);
   Output.T = normalize(worldTangent);
   
   float3 worldBinormal = mul(Input.mBinormal,(float3x3)matWorld);
   Output.B = normalize(worldBinormal);
   
 
   
   return Output;
   
}




texture DiffuseMap_Tex
<
   string ResourceName = "..\\..\\..\\..\\..\\..\\..\\PtT‘Å\\a4dun_spire_firewall_groundBurn.dds";
>;
sampler2D Diffuse = sampler_state
{
   Texture = (DiffuseMap_Tex);
   MAGFILTER = LINEAR;
};
texture Normal_Tex
<
   string ResourceName = "..\\..\\..\\..\\..\\..\\..\\PtT‘Å\\normalDefault.png";
>;
sampler2D Normal = sampler_state
{
   Texture = (Normal_Tex);
};

float RedTime
<
   string UIName = "RedTime";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float( 1.00 );
float YellowTime
<
   string UIName = "YellowTime";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float( 0.80 );
float Alpha
<
   string UIName = "Alpha";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float( 1.00 );


struct PS_INPUT
{
   float2 TexCoord       : TEXCOORD0;
   float3 ViewDirection  : TEXCOORD1;
   float3 LightDir       : TEXCOORD2;  
   
   float3 T : TEXCOORD3;
   float3 B : TEXCOORD4;
   float3 N : TEXCOORD5;
};

float4 Default_DirectX_Effect_Pass_0_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR0
{  

   float4 albedo = tex2D(Diffuse, Input.TexCoord);
   float3 tangentNormal = tex2D(Normal,Input.TexCoord).xyz;
   tangentNormal = normalize(tangentNormal * 2 - 1);
   
   float3x3 TBN = float3x3(normalize(Input.T),normalize(Input.B),normalize(Input.N));
   TBN = transpose(TBN);
 
 
   float3 worldNormal = mul(TBN,tangentNormal);
   float3 worldlightDir = normalize(Input.LightDir);
   float3 diffuse = saturate(dot(worldNormal,-worldlightDir));
   diffuse = albedo.rgb * diffuse;

  
   float3 specular = 0;
   
   if(diffuse.x > 0)
   {
      float3 reflection = reflect(worldlightDir , worldNormal);
      float3 viewDir = normalize(Input.ViewDirection);
      
 

      specular = saturate(dot(reflection , -viewDir));
      specular = pow(specular, 20.0f);
     
   }
   
   float3 ambient = float3(RedTime,YellowTime, 0.7f)*albedo*(albedo.a*Alpha);



   
   return (float4(ambient+diffuse,1.0f)*4);
}


//--------------------------------------------------------------//
// Technique Section for Default_DirectX_Effect
//--------------------------------------------------------------//
technique Default_DirectX_Effect
{
   pass Pass_0
   {
      ALPHABLENDENABLE = TRUE;
      BLENDOP = ADD;
      DESTBLEND = INVSRCCOLOR;
      SRCBLEND = ONE;

      VertexShader = compile vs_2_0 Default_DirectX_Effect_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 Default_DirectX_Effect_Pass_0_Pixel_Shader_ps_main();
   }

}

