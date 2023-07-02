#include"Obj.hlsli"



Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);
float4 main(GSOutput input) : SV_TARGET{

float4 target = tex.Sample(smp, input.uv);

float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

float3 ambient = m_ambient;
float3 diffuse = dot(m_lightv, input.normal.xyz) * m_diffuse;

//_ÀW
const float3 eye = float3(0, 0, -20);
//õòx
//const float shininess = 15.0f;
//¸_©ç_ÌxNg
//½ËõedirxNg
float3 reflect = normalize(-m_lightv + 2 * dot(m_lightv, input.normal.xyz) * input.normal.xyz);
//¾Ê½Ëõ
float3 specular = pow(saturate(dot(reflect, eyedir)), m_shininess) * m_specular;

float4 shadecolor;

shadecolor.rgb = (ambient + specular + diffuse) * m_lightcolor;
shadecolor.a = m_alpha;


return target *shadecolor;
}
