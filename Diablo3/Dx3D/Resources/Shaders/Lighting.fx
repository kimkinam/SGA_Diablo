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
// Textured
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
string Textured_Pass_0_Model : ModelData = "..\\..\\..\\..\\..\\..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\LightingBreath.X";

float4x4 matWorld : World;
float4x4 matViewProjection : ViewProjection;

float fFrameFactor
<
   string UIName = "fFrameFactor";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float( 0.25 );
int   nFrameIndex
<
   string UIName = "nFrameIndex";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   int UIMin = -100;
   int UIMax = 100;
> = int( 1 );
float fFrameRate
<
   string UIName = "fFrameRate";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float( 0.00 );


struct VS_INPUT 
{
   float4 Position : POSITION0;
   float2 Texcoord : TEXCOORD0;
   
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
   float2 Texcoord : TEXCOORD0;
   
};

VS_OUTPUT Textured_Pass_0_Vertex_Shader_vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;

   Output.Position = mul(Input.Position, matWorld);
   Output.Position = mul( Input.Position, matViewProjection );
   Output.Texcoord = Input.Texcoord;

   //Output.Texcoord.x *= (nFrameIndex * fFrameFactor);
   
   //int a = ceil(fFrameRate);
   
   //Output.Texcoord.x += fFrameFactor * a;
   Output.Position = mul( Input.Position, matViewProjection );
   Output.Texcoord = Input.Texcoord;

   Output.Texcoord.x *= (nFrameIndex * fFrameFactor);
   
   int a = ceil(fFrameRate);
   
   Output.Texcoord.x += fFrameFactor * a;   
   return( Output );
   
}




texture base_Tex
<
   string ResourceName = "..\\..\\..\\..\\..\\..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Textures\\diablo_lightningForks.dds";
>;
sampler2D baseMap = sampler_state
{
   Texture = (base_Tex);
   ADDRESSU = WRAP;
   ADDRESSV = WRAP;
   MINFILTER = LINEAR;
   MAGFILTER = LINEAR;
};

float fLifeTime;
float fAmbientMax;
float fAlpha;

struct PS_INPUT 
{
   float2 Texcoord : TEXCOORD0;
   
};

float4 Textured_Pass_0_Pixel_Shader_ps_main( PS_INPUT Input ) : COLOR0
{
   return tex2D( baseMap, Input.Texcoord );
   
   float4 albedo = tex2D(baseMap, Input.Texcoord);
   
   
   if(fLifeTime < 0)
   {
      float b = albedo.b;
      albedo.b = albedo.r;
      albedo.r = b;
   }
   
   float3 Ambient = fAmbientMax  * albedo * (albedo.a * fAlpha);
   
   return float4(Ambient,1.0f);
   
}




//--------------------------------------------------------------//
// Technique Section for Textured
//--------------------------------------------------------------//
technique Textured
{
   pass Pass_0
   {
      DESTBLEND = INVSRCCOLOR;
      SRCBLEND = SRCALPHA;
      ALPHABLENDENABLE = TRUE;

      VertexShader = compile vs_1_1 Textured_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 Textured_Pass_0_Pixel_Shader_ps_main();
   }

}

