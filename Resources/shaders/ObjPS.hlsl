#include"Obj.hlsli"



Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);
float4 main(GSOutput input) : SV_TARGET{

float4 target = tex.Sample(smp, input.uv);

float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

float3 ambient = m_ambient;
float3 diffuse = dot(m_lightv, input.normal.xyz) * m_diffuse;

//視点座標
const float3 eye = float3(0, 0, -20);
//光沢度
//const float shininess = 15.0f;
//頂点から視点のベクトル
//反射光edirベクトル
float3 reflect = normalize(-m_lightv + 2 * dot(m_lightv, input.normal.xyz) * input.normal.xyz);
//鏡面反射光
float3 specular = pow(saturate(dot(reflect, eyedir)), m_shininess) * m_specular;

float4 shadecolor=float4(ambientColor*ambient,m_alpha);

for (int i=0;i< DIR_LIGHT_NUM;i++)
{

}

shadecolor.rgb = (ambient + specular + diffuse) * m_lightcolor;
shadecolor.a = m_alpha;


return target *shadecolor;
}
