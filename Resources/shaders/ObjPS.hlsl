#include"Obj.hlsli"



Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);
float4 main(GSOutput input) : SV_TARGET{

float4 target = tex.Sample(smp, input.uv);

float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

float3 ambient = m_ambient;

//���_���W
const float3 eye = float3(0, 0, -20);

float4 shadecolor=float4(ambientColor*ambient,m_alpha);

for (uint i=0;i< DIR_LIGHT_NUM;i++)
{
	if (dirLights[i].active)
	{

		float3 dotlightnormal = dot(dirLights[i].lightv, input.normal.xyz);

		float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal.xyz);

		float3 diffuse = dotlightnormal * m_diffuse;

		float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

		shadecolor.rgb += (specular + diffuse) * dirLights[i].lightcolor;
	}
}

for (uint i = 0; i < DIR_LIGHT_NUM; i++)
{
	if (PointLights[i].active)
	{
		//���C�g�ւ̃x�N�g��
		float3 lightv = PointLights[i].lightpos - input.worldpos.xyz;
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
		shadecolor.rgb += atten * (diffuse + specular) * PointLights[i].lightcolor;
	}
}


return target;
}
