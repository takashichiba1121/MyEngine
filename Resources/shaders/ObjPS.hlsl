#include"Obj.hlsli"



Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);
float4 main(GSOutput input) : SV_TARGET{

float4 target = tex.Sample(smp, input.uv);

float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

float3 ambient = m_ambient;;

//‹“_À•W
const float3 eye = float3(0, 0, -20);

float4 shadecolor=float4(ambientColor*ambient,m_alpha);

for (uint i=0;i< DIR_LIGHT_NUM;i++)
{
	if (dirLights[i].active)
	{
		float3 dotlightnormal = dot(dirLights[i].lightv, input.normal.xyz);

		float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal.xyz);

		float3 diffuse = dotlightnormal * m_diffuse;

		float3 specular = pow(saturate(dot(reflect, eyedir)), dirLights[i].shininess) * m_specular;

		shadecolor.rgb += (specular + diffuse) * dirLights[i].lightcolor;		
	}
}


return target *shadecolor;
}
