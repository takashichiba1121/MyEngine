#include "Obj.hlsli"

VSOutput main(float4 pos : POSITION ,float3 normal : NORMAL ,float2 uv : TEXCOORD) {
	////�E�A���A���̕����Ɍ��������C�g
	//float3 lightdir = float3(1, -1, 1);
	//lightdir = normalize(lightdir);
	////���C�g�̐F�i���j
	//float3 lightColor = float3(1, 1, 1);
	float4 wnormal = normalize(mul(world, float4(normal, 0)));
	float4 wpos = mul(world, pos);

	float3 eyedir = normalize(cameraPos - wpos.xyz);

	float3 ambient = m_ambient;
	float3 diffuse = dot(m_lightv, wnormal.xyz) * m_diffuse;

	//���_���W
	const float3 eye = float3(0, 0, -20);
	//����x
	const float shininess = 4.0f;
	//���_���王�_�̃x�N�g��
	//���ˌ�edir�x�N�g��
	float3 reflect = normalize(-m_lightv + 2 * dot(-m_lightv, wnormal.xyz) * wnormal.xyz);
	//���ʔ��ˌ�
	float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

	VSOutput output;//�s�N�Z���V�F�[�_��n���l
	output.svpos = pos;
	output.normal = normal;
	output.color.rgb = (ambient + diffuse + specular) * float4(1,1,1,1);
	output.color.a = m_alpha;
	output.uv = uv;
	return output;

}