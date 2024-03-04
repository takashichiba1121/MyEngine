#include"Obj.hlsli"



Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);
float4 main(GSOutput input) : SV_TARGET{

float4 target = tex.Sample(smp, input.uv)* float4(color, alpha);

float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

float3 ambient = m_ambient;

float4 shadecolor=float4(ambientColor*ambient,m_alpha);

for (uint i=0;i< DIRLIGHT_NUM;i++)
{
	if (dirLights[i].active)
	{

		float3 dotlightnormal = saturate(dot(dirLights[i].lightv, input.normal.xyz));

		float3 reflect = normalize(-dirLights[i].lightv + 2 * input.normal.xyz * dot(input.normal.xyz, dirLights[i].lightv));

		float3 diffuse = dotlightnormal * m_diffuse;

		float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

		shadecolor.rgb += (specular + diffuse) * dirLights[i].lightcolor;
	}
}

for (i = 0; i < POINTLIGHT_NUM; i++)
{
	if (PointLights[i].active)
	{
		//���C�g�ւ̃x�N�g��
		float3 lightv =PointLights[i].lightpos-input.worldpos;
		//�x�N�g���̒���
		float d = length(lightv);
		//���K�����A�P�ʃx�N�g���Ƃ���
		lightv = normalize(lightv);

		//���������W��
		float atten = 1.0f / (PointLights[i].lightAtten.x + PointLights[i].lightAtten.y * d + PointLights[i].lightAtten.z * d * d);

		//���C�g�Ɍ������x�N�g���Ɩ@���̂Ȃ���
		float3 dotlightnormal = dot(lightv, input.normal);
		//���ˌ��x�N�g��
		float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
		//�g�U���ˌ�
		float3 diffuse = dotlightnormal * m_diffuse;
		//���ʔ��ˌ�
		float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

		//���ׂĉ��Z����
		shadecolor.rgb += abs(atten * (diffuse + specular) * PointLights[i].lightcolor);
	}
}
return target*shadecolor;
}
