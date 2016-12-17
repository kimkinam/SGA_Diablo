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
string Default_DirectX_Effect_Pass_0_Model : ModelData = "..\\..\\..\\..\\..\\..\\..\\..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\??X";

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
> = float4( 500.00, 500.00, 500.00, 1.00 );
float4   vViewPosition : ViewPosition;


float    fTime : Time0_X;
float	 alpha
<
   string UIName = "alpha";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float( 0.00 );
float    fSpeed
<
   string UIName = "fSpeed";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float( 2.00 );
float    fWaveH
<
   string UIName = "fWaveH";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float( 0.10 );
float    fWaveF
<
   string UIName = "fWaveF";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float( 10.00 );
float    uvspeed
<
   string UIName = "uvspeed";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float( 0.03 );

struct VS_INPUT 
{
   float4 Position : POSITION0;
   float2 TexCoord : TEXCOORD0;
   float2 TexCoord1: TEXCOORD0; 
};

struct VS_OUTPUT 
{
   float4 Position       : POSITION0;
   float2 TexCoord       : TEXCOORD0;
   float3 ViewDirection  : TEXCOORD1;
   float3 LightDir       : TEXCOORD2;
 
  
   

};

VS_OUTPUT Default_DirectX_Effect_Pass_0_Vertex_Shader_vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;
   
   float cosTime = fWaveH*cos(fTime*fSpeed+Input.TexCoord.y*fWaveF);
   Input.Position +=cosTime; 
    Output.Position = mul(Input.Position,matWorldViewProjection);
    
    Output.TexCoord = Input.TexCoord+float2(fTime*uvspeed,fTime*uvspeed);
    float4 worldPosition = mul(Input.Position , matWorld); 
    float3 lightDir = worldPosition.xyz - vLightPosition.xyz;
    Output.LightDir = normalize(lightDir);
    
    float3 viewDir = normalize(worldPosition.xyz - vViewPosition.xyz );
    Output.ViewDirection = viewDir;
   
   
 
   
   return Output;
   
}




texture DiffuseMap_Tex
<
   string ResourceName = "..\\..\\..\\..\\..\\..\\..\\..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Textures\\cloud.tga";
>;
sampler2D Diffuse = sampler_state
{
   Texture = (DiffuseMap_Tex);
};
texture Fieldstone_SM_Tex
<
   string ResourceName = "..\\..\\..\\..\\..\\..\\..\\..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Textures\\cloud.tga";
>;
sampler2D Specular = sampler_state
{
   Texture = (Fieldstone_SM_Tex);
};
texture Diablo_glow_Tex
<
   string ResourceName = "..\\..\\..\\..\\..\\..\\..\\..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Textures\\cloud.tga";
>;
sampler2D Glow = sampler_state
{
   Texture = (Diablo_glow_Tex);
};




struct PS_INPUT
{
   float2 TexCoord       : TEXCOORD0;
   float3 ViewDirection  : TEXCOORD1;
   float3 LightDir       : TEXCOORD2;  
};

float4 Default_DirectX_Effect_Pass_0_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR0
{  

   float4 albedo = tex2D(Diffuse, Input.TexCoord);
   float4 albedo1 = tex2D(Glow,Input.TexCoord);



   float3 worldNormal = (0.1f,0.1f,0.1f);
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
      float4 specularIntensity = tex2D(Specular,Input.TexCoord);
      specular *= specularIntensity.rgb;
   }
   
   float3 ambient = float3(0.7f, 0.7f, 0.7f) * albedo;//.a*alpha;
   float3 ambient1 = float3(1.0f, 1.0f, 1.0f) * albedo1.a*0.7;


   
   return (float4(ambient+ diffuse + specular,0.5f)+float4(ambient1,0.5f)); 
}



//--------------------------------------------------------------//
// Technique Section for Default_DirectX_Effect
//--------------------------------------------------------------//
technique Default_DirectX_Effect
{
   pass Pass_0
   {
      BLENDOP = MAX;
      ALPHABLENDENABLE = TRUE;
      ANTIALIASEDLINEENABLE = TRUE;
      ALPHATESTENABLE = TRUE;
      ALPHAFUNC = GREATEREQUAL;
      MULTISAMPLEANTIALIAS = FALSE;
      SHADEMODE = FLAT;
      DESTBLEND = SRCCOLOR;
      CCW_STENCILFAIL = ZERO;

      VertexShader = compile vs_2_0 Default_DirectX_Effect_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 Default_DirectX_Effect_Pass_0_Pixel_Shader_ps_main();
   }

}

