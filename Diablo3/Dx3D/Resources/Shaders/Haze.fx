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
string Default_DirectX_Effect_Pass_0_Model : ModelData = "..\\..\\..\\..\\..\\..\\PtT��\\D��tx|.X";

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


float    fTime : Time0_X;
float    fSpeed;
float    fWaveH;
float    fWaveF;
float    uvspeed
<
   string UIName = "uvspeed";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float( 0.20 );

struct VS_INPUT 
{
   float4 Position : POSITION0;
   float2 TexCoord : TEXCOORD0;
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
   
    float cosTime = fWaveH*cos(fTime*fSpeed+Input.TexCoord.x*fWaveF);
   // Input.Position=fTime*fSpeed;
    Output.Position = mul(Input.Position,matWorldViewProjection);
    Output.TexCoord = Input.TexCoord+float2(-fTime*uvspeed,0);
    float4 worldPosition = mul(Input.Position , matWorld); 
    float3 lightDir = worldPosition.xyz - vLightPosition.xyz;
    Output.LightDir = normalize(lightDir);
    
    float3 viewDir = normalize(worldPosition.xyz - vViewPosition.xyz );
    Output.ViewDirection = viewDir;
   
   
 
   
   return Output;
   
}




texture DiffuseMap_Tex
<
   string ResourceName = "..\\..\\..\\..\\..\\..\\PtT��\\a4dun_Diablo_Crystal_Arch_Wings.dds";
>;
sampler2D Diffuse = sampler_state
{
   Texture = (DiffuseMap_Tex);
};


float4 gaussFilter[7] = 
{
0.0,-3.0,0.0,1.0/64.0,
0.0,-2.0,0.0,6.0/64.0,
0.0,-1.0,0.0,15.0/64.0,
0.0,0.0,0.0,20.0/64.0,
0.0,1.0,0.0,15.0/64.0,
0.0,2.0,0.0,6.0/64.0,
0.0,3.0,0.0,1.0/64.0
};

float texScaler = 1.0/128.0;
float texOffset = 0.0;




struct PS_INPUT
{
   float2 TexCoord       : TEXCOORD0;
   float3 ViewDirection  : TEXCOORD1;
   float3 LightDir       : TEXCOORD2;  
};


float4 Default_DirectX_Effect_Pass_0_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR0
{  

float4 color =0.0;
int i;
for(i=0;i<7;i++)
{
color+=tex2D(Diffuse,float2(Input.TexCoord.x+gaussFilter[i].x*texScaler
        +texOffset,Input.TexCoord.y 
        +gaussFilter[i].y*texScaler+texOffset))*gaussFilter[i].w;
}



   float4 albedo = tex2D(Diffuse, Input.TexCoord);
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
     
   }
   
   float3 ambient = float3(1.0f, 1.0f, 1.0f)*albedo*albedo.a;



   
   return color*3.0*albedo.a;
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
      DESTBLEND = ONE;
      BLENDOPALPHA = MAX;
      ZWRITEENABLE = FALSE;
      VertexShader = compile vs_2_0 Default_DirectX_Effect_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 Default_DirectX_Effect_Pass_0_Pixel_Shader_ps_main();
   }

}
